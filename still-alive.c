#include "still-alive.h"

#include "archi/app/context.typ.h" // Archipelago plugin interface
#include "archi/fsm/state.fun.h" // state operations for finite state machine
#include "archi/util/error.def.h" // error codes
#include "archi/util/print.fun.h" // logging

#include "archi/util/os/signal.fun.h" // signal management
#include "archi/util/os/threads.fun.h" // concurrent processing
#include "sdl/window.fun.h" // window operations
#include "sdl/font.fun.h" // font operations

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <stdatomic.h>
#include <time.h>
#include <signal.h>

#include <SDL.h>

#define M "GLaDOS" // module name for logging

/*****************************************************************************/

#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#define INCBIN_PREFIX binary_
#include "incbin.h"

// FIXME: use #embed when it's implemented in gcc
INCBIN(song_wav, "song.wav");

/*****************************************************************************/

struct glados_signal_handler_data {
    long start_time; // time of the application start
    long last_time;  // time of the last update
    atomic_flag spinlock; // spinlock for time access
};

typedef struct glados_data {
    /// external resources ///
    archi_signal_flags_t *signal_flags; // states of signals

    struct plugin_font_psf2 *font; // text font
    struct archi_thread_group_context *thread_group; // concurrent processing context
    struct plugin_sdl_window_context *window; // window context

    /// internal resources ///
    struct glados_signal_handler_data signal_handler_data; // signal handler data
    archi_signal_handler_t signal_handler; // signal handler

    char screen[SCREEN_SIZE_Y][SCREEN_SIZE_X]; // screen characters
    int cursor_x, cursor_y; // cursor coordinates

    int song_line_idx; // index of the current song line
    int song_line_char_idx; // index of the current character in the song line
    bool song_line_event_processed; // whether the song line event has been already processed

    char song_line_buffer[TEXT_SIZE_X + 1]; // current line between linebreaks
    int song_line_buffer_size; // size of current line between linebreaks

    plugin_sdl_pixel_t *texture_copy; // copy of the texture buffer
    int texture_width;  // texture width in pixels
    int texture_height; // texture height in pixels

    // audio
    SDL_AudioSpec wav_spec;
    uint8_t *wav_buffer;
    uint32_t wav_length;
    SDL_AudioDeviceID snd_device_id;
} glados_data_t;

/*****************************************************************************/

// Get current time in milliseconds
static
long get_time(void)
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

/*****************************************************************************/

// Handle incoming signals
static
ARCHI_SIGNAL_HANDLER_FUNC(glados_signal_handler)
{
    (void) siginfo;
    (void) signals;

    struct glados_signal_handler_data *shdata = data;

    if (signo == SIGCONT) // continue after freezing
    {
        while (atomic_flag_test_and_set_explicit(&shdata->spinlock, memory_order_acquire))
            ; // lock
        {
            shdata->start_time += get_time() - shdata->last_time; // adjust the start time
        }
        atomic_flag_clear_explicit(&shdata->spinlock, memory_order_release); // unlock

        return false;
    }

    return true;
}

/*****************************************************************************/

// Draw background pixels in multiple threads
static
ARCHI_THREADS_TASK_FUNC(glados_draw_background)
{
    (void) thread_idx; // all threads are doing the same

    glados_data_t *glados = data;

    // Compute coordinates of the current pixel
    size_t y = task_idx / glados->texture_width;

    // Draw the flickering background
    long elapsed;

    while (atomic_flag_test_and_set_explicit(&glados->signal_handler_data.spinlock, memory_order_acquire))
        ; // lock
    {
        elapsed = get_time() - glados->signal_handler_data.start_time;
    }
    atomic_flag_clear_explicit(&glados->signal_handler_data.spinlock, memory_order_release); // unlock

    plugin_sdl_pixel_t pixel = BG_COLOR0;
    if ((y - elapsed / BG_FLICKERING_PERIOD) % BG_FLICKERING_MOD == 0)
        pixel = BG_COLOR1;

    // Update the texture
    plugin_sdl_pixel_t *texture = plugin_sdl_window_get_texture_lock(
            glados->window, NULL, NULL, NULL, NULL, NULL);
    texture[task_idx] = pixel;
}

// Draw foreground pixels (text) in multiple threads
static
ARCHI_THREADS_TASK_FUNC(glados_draw_foreground)
{
    (void) thread_idx; // all threads are doing the same

    glados_data_t *glados = data;

    // Compute coordinates of the current character
    size_t x = task_idx % SCREEN_SIZE_X;
    size_t y = task_idx / SCREEN_SIZE_X;

    // Extract the glyph of the current character
    char character[2] = {glados->screen[y][x], '\0'};
    const unsigned char *glyph = plugin_font_psf2_glyph(glados->font, character, 1, NULL);

    // Draw the glyph
    plugin_sdl_window_texture_draw_glyph(glados->window,
            x * glados->font->header->width, y * glados->font->header->height,
            glyph, glados->font->header->width, glados->font->header->height,
            0, 0, glados->font->header->width, glados->font->header->height,
            TEXT_COLOR, 0,
            true, false);
}

// Blur the window texture
static
ARCHI_THREADS_TASK_FUNC(glados_draw_blur)
{
    (void) thread_idx; // all threads are doing the same

    glados_data_t *glados = data;

    int texture_width = glados->texture_width;

    // Compute coordinates of the current pixel
    int x = task_idx % texture_width;
    int y = task_idx / texture_width;

    // Obtain the neighbourhood of pixels
    plugin_sdl_pixel_t *texture = plugin_sdl_window_get_texture_lock(
            glados->window, NULL, NULL, NULL, NULL, NULL);

    plugin_sdl_pixel_t local[3][3] = {0};

    if (y > 0)
    {
        if (x > 0)
            local[0][0] = texture[(y - 1) * texture_width + (x - 1)];

        local[0][1] = texture[(y - 1) * texture_width + x];

        if (x < (texture_width - 1))
            local[0][2] = texture[(y - 1) * texture_width + (x + 1)];
    }
    {
        if (x > 0)
            local[1][0] = texture[y * texture_width + (x - 1)];

        local[1][1] = texture[task_idx];

        if (x < (texture_width - 1))
            local[1][2] = texture[y * texture_width + (x + 1)];
    }
    if (y < (glados->texture_height - 1))
    {
        if (x > 0)
            local[2][0] = texture[(y + 1) * texture_width + (x - 1)];

        local[2][1] = texture[(y + 1) * texture_width + x];

        if (x < (texture_width - 1))
            local[2][2] = texture[(y + 1) * texture_width + (x + 1)];
    }

    // Apply the blur kernel
    static const float kernel[3][3] = {
        {0.100f, 0.100f, 0.100f},
        {0.100f, 0.800f, 0.100f},
        {0.100f, 0.100f, 0.100f},
    };

    float alpha = 0, blue = 0, green = 0, red = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            plugin_sdl_pixel_t pixel = local[i][j];

            alpha += ((pixel >> 8*3) & 0xFF) * kernel[i][j];
            blue  += ((pixel >> 8*2) & 0xFF) * kernel[i][j];
            green += ((pixel >> 8*1) & 0xFF) * kernel[i][j];
            red   += ((pixel >> 8*0) & 0xFF) * kernel[i][j];

            if (alpha > 255.0f)
                alpha = 255.0f;
            if (blue > 255.0f)
                blue = 255.0f;
            if (green > 255.0f)
                green = 255.0f;
            if (red > 255.0f)
                red = 255.0f;
        }
    }

    plugin_sdl_pixel_t pixel = 0;
    pixel |= ((plugin_sdl_pixel_t)alpha) << 8*3;
    pixel |= ((plugin_sdl_pixel_t)blue)  << 8*2;
    pixel |= ((plugin_sdl_pixel_t)green) << 8*1;
    pixel |= ((plugin_sdl_pixel_t)red)   << 8*0;

    glados->texture_copy[task_idx] = pixel;
}

/*****************************************************************************/

static
ARCHI_FSM_STATE_FUNCTION(glados_state_loop);

static
ARCHI_FSM_STATE_FUNCTION(glados_state_draw);

static
ARCHI_FSM_STATE_FUNCTION(glados_state_sing);

// The initialization state
static
ARCHI_FSM_STATE_FUNCTION(glados_state_init)
{
    glados_data_t *glados = ARCHI_FSM_CURRENT_DATA(glados_data_t*);

    // Check if everything is available and correct
    if (glados->thread_group == NULL)
    {
        archi_log_error(M, "Concurrent processing context is unavailable.");
        ARCHI_FSM_ABORT(ARCHI_ERROR_UNAVAIL);
    }
    else if (glados->window == NULL)
    {
        archi_log_error(M, "Window is unavailable.");
        ARCHI_FSM_ABORT(ARCHI_ERROR_UNAVAIL);
    }

    // Initialize the screen
    for (int y = 0; y < SCREEN_SIZE_Y; y++)
    {
        for (int x = 0; x < SCREEN_SIZE_X; x++)
        {
            if ((y == 0) || ((y == SCREEN_SIZE_Y - ASCIIART_SIZE_Y - 1) &&
                        (x > SCREEN_SIZE_X - ASCIIART_SIZE_X - 1)) ||
                    ((y == SCREEN_SIZE_Y - 1) && (x <= SCREEN_SIZE_X - ASCIIART_SIZE_X - 1)))
                glados->screen[y][x] = '-';
            else if ((x == 0) || (x == SCREEN_SIZE_X - ASCIIART_SIZE_X - 1) ||
                    ((x == SCREEN_SIZE_X - 1) && (y < SCREEN_SIZE_Y - ASCIIART_SIZE_Y)))
                glados->screen[y][x] = '|';
            else
                glados->screen[y][x] = ' ';
        }
    }

    // Write the permanent text to the screen
    {
        int comment_pos_x = SCREEN_SIZE_X - ASCIIART_SIZE_X;
        int comment_pos_y = (SCREEN_SIZE_Y - ASCIIART_SIZE_Y) / 2 - 1;

        memcpy(&glados->screen[comment_pos_y + 0][comment_pos_x], COMMENT0, strlen(COMMENT0));
        memcpy(&glados->screen[comment_pos_y + 1][comment_pos_x], COMMENT1, strlen(COMMENT1));
        memcpy(&glados->screen[comment_pos_y + 2][comment_pos_x], COMMENT2, strlen(COMMENT2));
    }

    // Set execution start time
    while (atomic_flag_test_and_set_explicit(&glados->signal_handler_data.spinlock, memory_order_acquire))
        ; // lock
    {
        glados->signal_handler_data.last_time =
            glados->signal_handler_data.start_time = get_time();
    }
    atomic_flag_clear_explicit(&glados->signal_handler_data.spinlock, memory_order_release); // unlock

    // Go to the main state
    ARCHI_FSM_PROCEED(0, ARCHI_FSM_STATE(glados_state_loop, glados));
}

// The GUI loop state
static
ARCHI_FSM_STATE_FUNCTION(glados_state_loop)
{
    glados_data_t *glados = ARCHI_FSM_CURRENT_DATA(glados_data_t*);

    // Check signal states
    if (ARCHI_SIGNAL_IS_FLAG_SET(glados->signal_flags->f_SIGINT))
    {
        archi_log_info(M, "Caught SIGINT.");
        ARCHI_FSM_FINISH(1);
    }
    else if (ARCHI_SIGNAL_IS_FLAG_SET(glados->signal_flags->f_SIGTERM))
    {
        archi_log_info(M, "Caught SIGTERM.");
        exit(0);
    }
    else if (ARCHI_SIGNAL_IS_FLAG_SET(glados->signal_flags->f_SIGQUIT))
    {
        archi_log_info(M, "Caught SIGQUIT.");
        quick_exit(0);
    }

    // Process window events
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        // Exit when the window is closed or <Escape> is pressed
        if ((event.type == SDL_QUIT) ||
                ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)))
            ARCHI_FSM_FINISH(1);
    }

    // Prevent excess CPU usage
    SDL_Delay(10);

    // Proceed with the singing and drawing
    ARCHI_FSM_PROCEED(0,
            ARCHI_FSM_STATE(glados_state_sing, glados),
            ARCHI_FSM_STATE(glados_state_draw, glados),
            ARCHI_FSM_CURRENT());
}

// The drawing state
static
ARCHI_FSM_STATE_FUNCTION(glados_state_draw)
{
    glados_data_t *glados = ARCHI_FSM_CURRENT_DATA(glados_data_t*);

    // Update the window texture

    // step 1: lock the texture
    if (plugin_sdl_window_lock_whole_texture(glados->window) != 0)
    {
        archi_log_error(M, "Couldn't lock the window texture.");
        ARCHI_FSM_ABORT(ARCHI_ERROR_OPERATION);
    }

    int texture_width = glados->texture_width;
    int texture_height = glados->texture_height;

    // step 2: draw background (pixels) using multiple threads
    while (archi_thread_group_execute(glados->thread_group, (archi_thread_group_job_t){
                .function = glados_draw_background, .data = glados,
                .num_tasks = texture_width * texture_height},
                (archi_thread_group_callback_t){0},
                (archi_thread_group_exec_config_t){0}) != 0); // retry if the threads are not ready

    // step 3: draw foreground (characters) using multiple threads
    while (archi_thread_group_execute(glados->thread_group, (archi_thread_group_job_t){
                .function = glados_draw_foreground, .data = glados,
                .num_tasks = SCREEN_SIZE_X * SCREEN_SIZE_Y},
                (archi_thread_group_callback_t){0},
                (archi_thread_group_exec_config_t){0}) != 0); // retry if the threads are not ready

    // step 4: blur the window
    while (archi_thread_group_execute(glados->thread_group, (archi_thread_group_job_t){
                .function = glados_draw_blur, .data = glados,
                .num_tasks = texture_width * texture_height},
                (archi_thread_group_callback_t){0},
                (archi_thread_group_exec_config_t){0}) != 0); // retry if the threads are not ready

    plugin_sdl_pixel_t *texture = plugin_sdl_window_get_texture_lock(
            glados->window, NULL, NULL, NULL, NULL, NULL);

    memcpy(texture, glados->texture_copy, sizeof(*glados->texture_copy) * texture_width * texture_height);

    // step 5: unlock the texture and render
    if (plugin_sdl_window_unlock_texture_and_render(glados->window) != 0)
    {
        archi_log_error(M, "Couldn't unlock the window texture and render it.");
        ARCHI_FSM_ABORT(ARCHI_ERROR_OPERATION);
    }
}

// The singing state
static
ARCHI_FSM_STATE_FUNCTION(glados_state_sing)
{
    glados_data_t *glados = ARCHI_FSM_CURRENT_DATA(glados_data_t*);

    // Get current elapsed time
    long elapsed;

    while (atomic_flag_test_and_set_explicit(&glados->signal_handler_data.spinlock, memory_order_acquire))
        ; // lock
    {
        glados->signal_handler_data.last_time = get_time();
        elapsed = glados->signal_handler_data.last_time - glados->signal_handler_data.start_time;
    }
    atomic_flag_clear_explicit(&glados->signal_handler_data.spinlock, memory_order_release); // unlock

    // Get pointer to the current song line
    const struct song_line *current_line = &song[glados->song_line_idx];

    // Draw cursor
    glados->screen[1 + glados->cursor_y][1 + glados->cursor_x] =
        ((elapsed / CURSOR_FLICKERING_PERIOD) % 2) ? '_' : ' '; // cursor

    // Check if the song ended
    if (current_line->text == NULL)
    {
        if (elapsed >= current_line->end)
        {
            // exit now
            ARCHI_FSM_FINISH(1);
        }
        else
            return;
    }

    // Get pointer to the next song line
    const struct song_line *next_line = &song[glados->song_line_idx + 1];

    // Check if song event of the current song line was processed already
    if (!glados->song_line_event_processed)
    {
        int pposx = SCREEN_SIZE_X - ASCIIART_SIZE_X; // picture coordinate X
        int pposy = SCREEN_SIZE_Y - ASCIIART_SIZE_Y; // picture coordinate Y
        int picidx; // picture index

        switch (current_line->event)
        {
            case EVENT_NONE: // do nothing
                break;

            case EVENT_START_MUSIC: // start playing the WAV
                SDL_PauseAudioDevice(glados->snd_device_id, 0);
                break;

            case EVENT_CLEAR_SCREEN: // clear the text area
                glados->screen[1 + glados->cursor_y][1 + glados->cursor_x] = ' ';

                for (int y = 0; y < TEXT_SIZE_Y; y++)
                    for (int x = 0; x < TEXT_SIZE_X; x++)
                        glados->screen[1 + y][1 + x] = ' ';

                // Reset the cursor to the top-left corner
                glados->cursor_x = 0;
                glados->cursor_y = 0;

                archi_log_notice(M, "");
                archi_log_notice(M, "------------------------------------");
                archi_log_notice(M, "");
                break;

            default: // draw a picture
                picidx = current_line->event - EVENT_DRAW_ASCIIART;

                for (int y = 0; y < ASCIIART_SIZE_Y; y++)
                    for(int x = 0; x < ASCIIART_SIZE_X; x++)
                        glados->screen[pposy + y][pposx + x] = song_asciiart[picidx][y][x];
                break;
        }

        glados->song_line_event_processed = true;
    }

    int line_length = strlen(current_line->text); // number of character in the current line
    int line_visible_chars = line_length; // number of characters visible at the current time

    // Update number of visible characters if needed
    if ((elapsed < current_line->end) && (current_line->start < current_line->end))
    {
        float line_progress = 1.0f * (elapsed - current_line->start) /
            (current_line->end - current_line->start);
        line_visible_chars = line_progress * line_length;
    }

    // Draw the new visible characters
    for (int x = glados->song_line_char_idx; x < line_visible_chars; x++)
    {
        char current_chr = current_line->text[x];

        if (current_chr == '\n')
        {
            // Hide cursor and move to the next line
            glados->screen[1 + glados->cursor_y][1 + glados->cursor_x] = ' ';
            glados->cursor_x = 0;
            glados->cursor_y++;

            // Log the current line
            glados->song_line_buffer[glados->song_line_buffer_size] = '\0';
            archi_log_notice(M, "%s", glados->song_line_buffer);
            glados->song_line_buffer_size = 0;
        }
        else
        {
            // Move cursor to the right
            glados->screen[1 + glados->cursor_y][1 + glados->cursor_x + 1] =
                glados->screen[1 + glados->cursor_y][1 + glados->cursor_x];
            glados->cursor_x++;

            // Draw the character
            glados->screen[1 + glados->cursor_y][1 + glados->cursor_x - 1] = current_chr;

            // Copy the character to the line buffer
            if (glados->song_line_buffer_size < TEXT_SIZE_X)
                glados->song_line_buffer[glados->song_line_buffer_size++] = current_chr;
        }
    }

    // Update character counter
    if (glados->song_line_char_idx < line_visible_chars)
        glados->song_line_char_idx = line_visible_chars;

    // Move to the next song line if the current is fully processed
    if ((line_visible_chars == line_length) && (elapsed >= next_line->start))
    {
        glados->song_line_idx++;
        glados->song_line_char_idx = 0;
        glados->song_line_event_processed = false;
    }
}

/*****************************************************************************/

// Initialize the app context
static
ARCHI_CONTEXT_INIT_FUNC(glados_init)
{
    (void) metadata;
    (void) config;

    glados_data_t *glados = malloc(sizeof(*glados));
    if (glados == NULL)
        return ARCHI_ERROR_ALLOC;

    *glados = (glados_data_t){
        .signal_handler_data = {.spinlock = ATOMIC_FLAG_INIT},
        .signal_handler = {.function = glados_signal_handler, .data = &glados->signal_handler_data},
    };

    *context = glados;
    return 0;
}

// Finalize the app context
static
ARCHI_CONTEXT_FINAL_FUNC(glados_final)
{
    (void) metadata;

    glados_data_t *glados = context;

    if (glados->snd_device_id != 0)
        SDL_CloseAudioDevice(glados->snd_device_id);

    if (glados->wav_buffer != NULL)
        SDL_FreeWAV(glados->wav_buffer);

    free(glados->texture_copy);
    free(glados);
}

// Receive initialized contexts from Archipelago (app context fields setter)
static
ARCHI_CONTEXT_SET_FUNC(glados_set)
{
    (void) metadata;

    glados_data_t *glados = context;

#define CHECK_VALUE() do { \
    if ((value->type != ARCHI_VALUE_DATA) || (value->ptr == NULL) || (value->num_of == 0)) \
        return ARCHI_ERROR_MISUSE; \
} while (0)

    if (strcmp(slot, "signal_flags") == 0)
    {
        CHECK_VALUE();
        glados->signal_flags = value->ptr;
    }
    else if (strcmp(slot, "font") == 0)
    {
        CHECK_VALUE();
        glados->font = value->ptr;

        if (glados->font != NULL)
        {
            glados->texture_width = glados->font->header->width * SCREEN_SIZE_X;
            glados->texture_height = glados->font->header->height * SCREEN_SIZE_Y;

            // Allocate texture copy
            plugin_sdl_pixel_t *new_texture_copy = realloc(glados->texture_copy,
                    sizeof(*glados->texture_copy) * glados->texture_width * glados->texture_height);
            if (new_texture_copy == NULL)
            {
                archi_log_error(M, "Couldn't allocate memory for texture copy.");
                return ARCHI_ERROR_ALLOC;
            }

            glados->texture_copy = new_texture_copy;
        }
        else
        {
            glados->texture_width = 0;
            glados->texture_height = 0;

            free(glados->texture_copy);
            glados->texture_copy = NULL;
        }
    }
    else if (strcmp(slot, "thread_group") == 0)
    {
        CHECK_VALUE();
        glados->thread_group = value->ptr;
    }
    else if (strcmp(slot, "sdl_window") == 0)
    {
        CHECK_VALUE();
        glados->window = value->ptr;
    }
    else
        return ARCHI_ERROR_CONFIG;

    return 0;
}

// Give internal stuff to Archipelago (app context fields getter)
static
ARCHI_CONTEXT_GET_FUNC(glados_get)
{
    (void) metadata;

    glados_data_t *glados = context;

    if (strcmp(slot, "signal_handler") == 0)
    {
        *value = (archi_value_t){
            .ptr = &glados->signal_handler,
            .size = sizeof(glados->signal_handler),
            .num_of = 1,
            .type = ARCHI_VALUE_DATA,
        };
    }
    else if (strcmp(slot, "entry_state_function") == 0)
    {
        *value = (archi_value_t){
            .fptr = (archi_function_t)glados_state_init,
            .num_of = 1,
            .type = ARCHI_VALUE_FUNCTION,
        };
    }
    else if (strcmp(slot, "texture_width") == 0)
    {
        *value = (archi_value_t){
            .ptr = (void*)&glados->texture_width,
            .size = sizeof(glados->texture_width),
            .num_of = 1,
            .type = ARCHI_VALUE_SINT,
        };
    }
    else if (strcmp(slot, "texture_height") == 0)
    {
        *value = (archi_value_t){
            .ptr = (void*)&glados->texture_height,
            .size = sizeof(glados->texture_height),
            .num_of = 1,
            .type = ARCHI_VALUE_SINT,
        };
    }
    else
        return ARCHI_ERROR_CONFIG;

    return 0;
}

// Perform initialization actions
static
ARCHI_CONTEXT_ACT_FUNC(glados_act)
{
    (void) metadata;
    (void) params;

    glados_data_t *glados = context;

    if (strcmp(action, "init_sound") == 0)
    {
        // Load the WAV from memory
        if (SDL_LoadWAV_RW(SDL_RWFromConstMem(binary_song_wav_data, binary_song_wav_size),
                    1, &glados->wav_spec, &glados->wav_buffer, &glados->wav_length) == NULL)
        {
            archi_log_error(M, "Couldn't load built-in .wav file.");
            return ARCHI_ERROR_OPERATION;
        }

        // Open an audio device
        glados->snd_device_id = SDL_OpenAudioDevice(NULL, 0, &glados->wav_spec, NULL, 0);
        if (glados->snd_device_id == 0)
        {
            archi_log_error(M, "Couldn't open audio device.");
            return ARCHI_ERROR_OPERATION;
        }

        // Queue the song to play
        if (SDL_QueueAudio(glados->snd_device_id, glados->wav_buffer, glados->wav_length) < 0)
        {
            archi_log_error(M, "Couldn't queue WAV to play.");
            return ARCHI_ERROR_OPERATION;
        }
    }
    else
        return ARCHI_ERROR_CONFIG;

    return 0;
}

// Archipelago plugin (app context interface)
const archi_context_interface_t glados_interface = {
    .init_fn = glados_init,
    .final_fn = glados_final,
    .set_fn = glados_set,
    .get_fn = glados_get,
    .act_fn = glados_act,
};

