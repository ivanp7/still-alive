#include "still-alive.h"

#include <station/plugin.def.h>
#include <station/plugin.typ.h>

#include <station/application.def.h>
#include <station/application.fun.h>

#include <station/fsm.typ.h>
#include <station/fsm.def.h>

#include <station/signal.typ.h>
#include <station/signal.def.h>

#include <station/concurrent.fun.h>
#include <station/concurrent.typ.h>
#include <station/concurrent.def.h>

#include <station/sdl.fun.h>
#include <station/sdl.typ.h>

#include <station/font.fun.h>
#include <station/font.typ.h>

#include <station/buffer.typ.h>

#ifndef STATION_IS_CONCURRENT_PROCESSING_SUPPORTED
#  error Concurrent processing support is required.
#endif

#ifndef STATION_IS_SDL_SUPPORTED
#  error SDL support is required.
#endif

#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <threads.h>
#include <time.h>

///////////////////////////////////////////////////////////////////////////////

extern const unsigned char _binary_font_psf_start[]; // contents of 'font.psf' file (text font)
extern const unsigned char _binary_font_psf_end; // first address beyond 'font.psf' contents

extern const unsigned char _binary_song_wav_start[]; // contents of 'song.wav' file (music)
extern const unsigned char _binary_song_wav_end; // first address beyond 'song.wav' contents

///////////////////////////////////////////////////////////////////////////////

struct plugin_signal_handler_data {
    long start_time; // time of execution start
    long last_time;  // time of the last update
    mtx_t time_mtx;  // mutex for time access
};

struct plugin_resources {
    station_std_signal_set_t *std_signals; // states of standard signals
    struct plugin_signal_handler_data *shdata; // time management is affected by signals

    station_concurrent_processing_context_t *concurrent_processing_context; // for multithreaded rendering
    station_concurrent_processing_context_t dummy_concurrent_processing_context; // used when the real context wasn't provided

    SDL_Event event; // for window events
    station_sdl_window_context_t sdl_window; // window context

    station_font_psf2_t *font; // font context

    // audio
    SDL_AudioSpec wav_spec;
    uint8_t *wav_buffer;
    uint32_t wav_length;
    SDL_AudioDeviceID snd_device_id;

    char screen[SCREEN_SIZE_Y][SCREEN_SIZE_X]; // screen characters
    int cursor_x, cursor_y; // cursor coordinates

    // state of song processing
    int song_line_idx;
    int song_line_char_idx;
    bool song_line_event_processed;
};

///////////////////////////////////////////////////////////////////////////////

// Get current time in milliseconds
static long get_time(void)
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

///////////////////////////////////////////////////////////////////////////////

static STATION_SIGNAL_HANDLER_FUNC(signal_handler);

static STATION_SFUNC(sfunc_init); // initialize resources

static STATION_SFUNC(sfunc_song); // process the song

static STATION_PFUNC(pfunc_draw_background); // draw background pixels
static STATION_PFUNC(pfunc_draw_foreground); // draw foreground pixels (characters)
static STATION_SFUNC(sfunc_loop); // main loop

///////////////////////////////////////////////////////////////////////////////

// Signal handler function
static STATION_SIGNAL_HANDLER_FUNC(signal_handler) // implicit arguments: signo, siginfo, std_signals, rt_signals, data
{
    (void) siginfo;
    (void) std_signals;
    (void) rt_signals;

    struct plugin_signal_handler_data *shdata = data;

    if (signo == SIGCONT)
    {
        // Adjust the start time
        mtx_lock(&shdata->time_mtx);
        shdata->start_time += get_time() - shdata->last_time;
        mtx_unlock(&shdata->time_mtx);

        return false;
    }

    return true;
}

// Function to initialize the plugin resources
static STATION_SFUNC(sfunc_init) // implicit arguments: state, fsm_data
{
    struct plugin_resources *resources = fsm_data;

    // Proceed to the main loop
    state->sfunc = sfunc_loop;

    // Initialize the screen
    for (int y = 0; y < SCREEN_SIZE_Y; y++)
        for (int x = 0; x < SCREEN_SIZE_X; x++)
        {
            if ((y == 0) || ((y == SCREEN_SIZE_Y - PICTURE_SIZE_Y - 1) &&
                        (x > SCREEN_SIZE_X - PICTURE_SIZE_X - 1)) ||
                    ((y == SCREEN_SIZE_Y - 1) && (x <= SCREEN_SIZE_X - PICTURE_SIZE_X - 1)))
                resources->screen[y][x] = '-';
            else if ((x == 0) || (x == SCREEN_SIZE_X - PICTURE_SIZE_X - 1) ||
                    ((x == SCREEN_SIZE_X - 1) && (y < SCREEN_SIZE_Y - PICTURE_SIZE_Y)))
                resources->screen[y][x] = '|';
            else
                resources->screen[y][x] = ' ';
        }

    resources->cursor_x = 0;
    resources->cursor_y = 0;

    // Draw comments
    {
        int comment_pos_x = SCREEN_SIZE_X - PICTURE_SIZE_X;
        int comment_pos_y = (SCREEN_SIZE_Y - PICTURE_SIZE_Y) / 2 - 1;

        memcpy(&resources->screen[comment_pos_y + 0][comment_pos_x], COMMENT0, strlen(COMMENT0));
        memcpy(&resources->screen[comment_pos_y + 1][comment_pos_x], COMMENT1, strlen(COMMENT1));
        memcpy(&resources->screen[comment_pos_y + 2][comment_pos_x], COMMENT2, strlen(COMMENT2));
    }

    // Initialize song state
    resources->song_line_idx = 0;
    resources->song_line_char_idx = 0;
    resources->song_line_event_processed = false;

    // Get execution start time
    mtx_lock(&resources->shdata->time_mtx);
    resources->shdata->last_time = resources->shdata->start_time = get_time();
    mtx_unlock(&resources->shdata->time_mtx);
}

// Function to process the song
static STATION_SFUNC(sfunc_song) // implicit arguments: state, fsm_data
{
    struct plugin_resources *resources = fsm_data;

    // Proceed to the main loop
    state->sfunc = sfunc_loop;

    // Get current elapsed time
    mtx_lock(&resources->shdata->time_mtx);
    resources->shdata->last_time = get_time();
    long elapsed = resources->shdata->last_time - resources->shdata->start_time;
    mtx_unlock(&resources->shdata->time_mtx);

    // Get pointer to the current song line
    const struct song_line *current_line = &song[resources->song_line_idx];

    // Draw cursor
    resources->screen[1 + resources->cursor_y][1 + resources->cursor_x] =
        ((elapsed / CURSOR_FLICKERING_PERIOD) % 2) ? '_' : ' '; // cursor

    // Check if the song ended
    if (current_line->text == NULL)
    {
        if (elapsed >= current_line->end)
            state->sfunc = NULL; // exit now

        return;
    }

    // Get pointer to the next song line
    const struct song_line *next_line = &song[resources->song_line_idx + 1];

    // Check if song event of the current song line was processed already
    if (!resources->song_line_event_processed)
    {
        int pposx = SCREEN_SIZE_X - PICTURE_SIZE_X; // picture coordinate X
        int pposy = SCREEN_SIZE_Y - PICTURE_SIZE_Y; // picture coordinate Y
        int picidx; // picture index

        switch (current_line->event)
        {
            case EVENT_NONE: // do nothing
                break;

            case EVENT_START_MUSIC: // start playing the WAV
                SDL_PauseAudioDevice(resources->snd_device_id, 0);
                break;

            case EVENT_CLEAR_SCREEN: // clear the text area
                resources->screen[1 + resources->cursor_y][1 + resources->cursor_x] = ' ';

                for (int y = 0; y < TEXT_SIZE_Y; y++)
                    for (int x = 0; x < TEXT_SIZE_X; x++)
                        resources->screen[1 + y][1 + x] = ' ';

                // Reset the cursor to the top-left corner
                resources->cursor_x = 0;
                resources->cursor_y = 0;

                // Print to terminal
                if (!current_line->no_echo)
                {
                    printf("\n------------------------------------\n");
                    fflush(stdout);
                }
                break;

            default: // draw a picture
                picidx = current_line->event - EVENT_DRAW_PICTURE;

                for (int y = 0; y < PICTURE_SIZE_Y; y++)
                    for(int x = 0; x < PICTURE_SIZE_X; x++)
                        resources->screen[pposy + y][pposx + x] = song_picture[picidx][y][x];
                break;
        }

        resources->song_line_event_processed = true;
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
    for (int x = resources->song_line_char_idx; x < line_visible_chars; x++)
    {
        char current_chr = current_line->text[x];

        if (current_chr == '\n')
        {
            // Hide cursor and move to the next line
            resources->screen[1 + resources->cursor_y][1 + resources->cursor_x] = ' ';
            resources->cursor_x = 0;
            resources->cursor_y++;
        }
        else
        {
            // Move cursor to the right
            resources->screen[1 + resources->cursor_y][1 + resources->cursor_x + 1] =
                resources->screen[1 + resources->cursor_y][1 + resources->cursor_x];
            resources->cursor_x++;

            // Draw a character
            resources->screen[1 + resources->cursor_y][1 + resources->cursor_x - 1] = current_chr;
        }

        // Print to terminal
        if (!current_line->no_echo)
        {
            printf("%c", current_chr);
            fflush(stdout);
        }
    }

    // Update character counter
    if (resources->song_line_char_idx < line_visible_chars)
        resources->song_line_char_idx = line_visible_chars;

    // Move to the next song line if the current is fully processed
    if ((line_visible_chars == line_length) && (elapsed >= next_line->start))
    {
        resources->song_line_idx++;
        resources->song_line_char_idx = 0;
        resources->song_line_event_processed = false;
    }
}

///////////////////////////////////////////////////////////////////////////////

// Function to draw background pixels
static STATION_PFUNC(pfunc_draw_background) // implicit arguments: data, task_idx, thread_idx
{
    (void) thread_idx;

    struct plugin_resources *resources = data;

    // Compute coordinates of the current pixel
    station_task_idx_t y = task_idx / resources->sdl_window.texture.width;

    // Draw the flickering background
    mtx_lock(&resources->shdata->time_mtx);
    long elapsed = get_time() - resources->shdata->start_time;
    mtx_unlock(&resources->shdata->time_mtx);

    uint32_t pixel = BG_COLOR0;
    if ((y - elapsed / BG_FLICKERING_PERIOD) % BG_FLICKERING_MOD == 0)
        pixel = BG_COLOR1;

    // Update the texture
    resources->sdl_window.texture.lock.pixels[task_idx] = pixel;
}

// Function to draw foreground characters
static STATION_PFUNC(pfunc_draw_foreground) // implicit arguments: data, task_idx, thread_idx
{
    (void) thread_idx;

    struct plugin_resources *resources = data;

    // Compute coordinates of the current character
    station_task_idx_t x = task_idx % SCREEN_SIZE_X;
    station_task_idx_t y = task_idx / SCREEN_SIZE_X;

    // Extract the glyph of the current character
    char str[2] = {resources->screen[y][x], '\0'};
    const unsigned char *glyph = station_font_psf2_glyph(str, 1, NULL, resources->font);

    // Draw the glyph
    station_sdl_window_texture_draw_glyph(&resources->sdl_window,
            x * resources->font->header->width, y * resources->font->header->height,
            true, false, TEXT_COLOR, 0,
            glyph, resources->font->header->width, resources->font->header->height,
            0, 0, resources->font->header->width, resources->font->header->height);
}

// Main loop
static STATION_SFUNC(sfunc_loop) // implicit arguments: state, fsm_data
{
    struct plugin_resources *resources = fsm_data;

    // Proceed to the song processing
    state->sfunc = sfunc_song;

    // Check signal states
    /*
    if (STATION_SIGNAL_IS_FLAG_SET(&resources->std_signals->signal_SIGINT) ||
            STATION_SIGNAL_IS_FLAG_SET(&resources->std_signals->signal_SIGTERM))
        exit(EXIT_SUCCESS);
    else if (STATION_SIGNAL_IS_FLAG_SET(&resources->std_signals->signal_SIGQUIT))
        quick_exit(EXIT_SUCCESS);
    */

    // Poll window events
    while (SDL_PollEvent(&resources->event))
    {
        // Exit when the window is closed or <Escape> is pressed
        if ((resources->event.type == SDL_QUIT) ||
                ((resources->event.type == SDL_KEYDOWN) &&
                 (resources->event.key.keysym.sym == SDLK_ESCAPE)))
            exit(EXIT_SUCCESS);
    }

    // Update the window texture
    {
        // step 1: lock the texture
        if (station_sdl_window_lock_texture(&resources->sdl_window,
                    true, 0, 0, 0, 0) != 0)
        {
            printf("station_sdl_window_lock_texture() failure\n");
            exit(EXIT_FAILURE);
        }

        bool succeed;

        // step 2: draw background (pixels) using multiple threads
        do
        {
            succeed = station_concurrent_processing_execute(resources->concurrent_processing_context,
                    resources->sdl_window.texture.width * resources->sdl_window.texture.height, 0,
                    pfunc_draw_background, resources, NULL, NULL, false);
        }
        while (!succeed); // threads may be busy for a short amount of time, try again

        // step 3: draw foreground (characters) using multiple threads
        do
        {
            succeed = station_concurrent_processing_execute(resources->concurrent_processing_context,
                    SCREEN_SIZE_X * SCREEN_SIZE_Y, 0,
                    pfunc_draw_foreground, resources, NULL, NULL, false);
        }
        while (!succeed); // threads may be busy for a short amount of time, try again

        // step 4: unlock the texture and render
        if (station_sdl_window_unlock_texture_and_render(&resources->sdl_window) != 0)
        {
            printf("station_sdl_window_unlock_texture_and_render() failure\n");
            exit(EXIT_FAILURE);
        }
    }

    SDL_Delay(10); // don't overheat the processor
}

///////////////////////////////////////////////////////////////////////////////

// Plugin help function
static STATION_PLUGIN_HELP_FUNC(plugin_help) // implicit arguments: argc, argv
{
    (void) argc;
    (void) argv;

    printf("just do it\n");
}

// Plugin configuration function
static STATION_PLUGIN_CONF_FUNC(plugin_conf) // implicit arguments: args, argc, argv
{
    (void) argc;
    (void) argv;

    args->std_signals_used->signal_SIGCONT = true;
    args->signal_handler = signal_handler;

    struct plugin_signal_handler_data *shdata = malloc(sizeof(*shdata));
    if (shdata == NULL)
    {
        printf("Couldn't allocate signal handler data\n");
        exit(EXIT_FAILURE);
    }

    if (mtx_init(&shdata->time_mtx, mtx_plain) != thrd_success)
    {
        printf("Couldn't create a mutex\n");
        exit(EXIT_FAILURE);
    }

    mtx_lock(&shdata->time_mtx);
    shdata->last_time = shdata->start_time = 0;
    mtx_unlock(&shdata->time_mtx);

    args->signal_handler_data = shdata;

    args->num_concurrent_processing_contexts_used = 1;

    args->sdl_is_used = true;
#ifdef STATION_IS_SDL_SUPPORTED
    args->sdl_init_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
#endif
}

// Plugin initialization function
static STATION_PLUGIN_INIT_FUNC(plugin_init) // implicit arguments: inputs, outputs
{
    int step = 0;

    if (!inputs->sdl_is_available)
    {
        printf("SDL is required, but not available\n");
        goto failure;
    }

    struct plugin_resources *resources = malloc(sizeof(*resources));
    if (resources == NULL)
    {
        printf("malloc() failed\n");
        goto failure;
    }
    step = 1;

    outputs->plugin_resources = resources;

    outputs->fsm_initial_state.sfunc = sfunc_init; // begin from resource initialization state function
    outputs->fsm_data = resources;

    resources->std_signals = inputs->std_signals;
    resources->shdata = inputs->signal_handler_data;

    resources->dummy_concurrent_processing_context = (station_concurrent_processing_context_t){0};

    if (inputs->concurrent_processing_contexts->num_contexts > 0)
        resources->concurrent_processing_context = &inputs->concurrent_processing_contexts->contexts[0];
    else
    {
        station_concurrent_processing_initialize_context(&resources->dummy_concurrent_processing_context, 0, false);
        resources->concurrent_processing_context = &resources->dummy_concurrent_processing_context;
    }

    // Load the song
    {
        // Load the WAV from memory
        if (SDL_LoadWAV_RW(SDL_RWFromConstMem(_binary_song_wav_start,
                        &_binary_song_wav_end - _binary_song_wav_start),
                    1, &resources->wav_spec, &resources->wav_buffer, &resources->wav_length) == NULL)
        {
            printf("Couldn't load built-in .wav file\n");
            goto failure;
        }
        step = 2;

        // Open an audio device
        resources->snd_device_id = SDL_OpenAudioDevice(NULL, 0, &resources->wav_spec, NULL, 0);
        if (resources->snd_device_id == 0)
        {
            printf("Couldn't open audio device\n");
            goto failure;
        }
        step = 3;

        // Queue the song to play
        if (SDL_QueueAudio(resources->snd_device_id, resources->wav_buffer, resources->wav_length) < 0)
        {
            printf("Couldn't queue WAV to play\n");
            exit(EXIT_FAILURE);
        }
    }

    // Load the font
    {
        // Create the buffer
        station_buffer_t font_buffer = {
            .num_bytes = &_binary_font_psf_end - _binary_font_psf_start,
            .bytes = (void*)_binary_font_psf_start,
        };

        // Load the font from the buffer
        resources->font = station_load_font_psf2_from_buffer(&font_buffer);
        if (resources->font == NULL)
        {
            printf("Couldn't load built-in font\n");
            goto failure;
        }
        step = 4;
    }

    // Create the window
    {
        uint32_t texture_width = resources->font->header->width * SCREEN_SIZE_X;
        uint32_t texture_height = resources->font->header->height * SCREEN_SIZE_Y;

        station_sdl_window_properties_t properties = {
            .texture = {.width = texture_width, .height = texture_height},
            .window = {.width = texture_width * WINDOW_SCALE,
                .height = texture_height * WINDOW_SCALE,
                .title = "Still Alive"},
        };

        if (station_sdl_initialize_window_context(&resources->sdl_window, &properties) != 0)
        {
            printf("Couldn't create window\n");
            goto failure;
        }
        step = 5;
    }

    // In case of success, proceed
    return;

    // In case of failure, release created resources and exit with error
failure:
    if (step >= 5)
        station_sdl_destroy_window_context(&resources->sdl_window);

    if (step >= 4)
        station_unload_font_psf2(resources->font);

    if (step >= 3)
        SDL_CloseAudioDevice(resources->snd_device_id);

    if (step >= 2)
        SDL_FreeWAV(resources->wav_buffer);

    if (step >= 1)
        free(resources);

    exit(EXIT_FAILURE);
}

// Plugin finalization function
static STATION_PLUGIN_FINAL_FUNC(plugin_final) // implicit arguments: plugin_resources, quick
{
    struct plugin_resources *resources = plugin_resources;

    station_sdl_destroy_window_context(&resources->sdl_window);

    if (!quick)
        station_unload_font_psf2(resources->font);

    SDL_CloseAudioDevice(resources->snd_device_id);
    SDL_FreeWAV(resources->wav_buffer);

    if (!quick)
        station_concurrent_processing_destroy_context(&resources->dummy_concurrent_processing_context);

    mtx_destroy(&resources->shdata->time_mtx);

    if (!quick)
        free(resources->shdata);

    if (!quick)
        free(resources);

    return 0; // success
}

// Define the plugin
STATION_PLUGIN("still-alive", "Portal credits song, \"Still Alive\"",
        plugin_help, plugin_conf, plugin_init, plugin_final);
STATION_APP_PLUGIN_MAIN()

