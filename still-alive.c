#include "still-alive.h"

#include <station/plugin.def.h>
#include <station/plugin.typ.h>

#include <station/fsm.typ.h>
#include <station/fsm.def.h>

#include <station/parallel.fun.h>
#include <station/parallel.typ.h>
#include <station/parallel.def.h>

#include <station/sdl.fun.h>
#include <station/sdl.typ.h>

#include <station/font.fun.h>
#include <station/font.typ.h>

#include <station/buffer.typ.h>

#ifndef STATION_IS_SDL_SUPPORTED
#  error SDL support is required.
#endif

#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

///////////////////////////////////////////////////////////////////////////////

extern const unsigned char _binary_font_psf_start[];
extern const unsigned char _binary_font_psf_end;

extern const unsigned char _binary_song_wav_start[];
extern const unsigned char _binary_song_wav_end;

///////////////////////////////////////////////////////////////////////////////

struct plugin_resources {
    station_parallel_processing_context_t *parallel_processing_context;

    SDL_Event event;
    station_sdl_window_context_t sdl_window;

    station_font_psf2_t *font;

    SDL_AudioSpec wav_spec;
    uint8_t *wav_buffer;
    uint32_t wav_length;
    SDL_AudioDeviceID snd_device_id;

    struct timespec start_time;

    char screen[SCREEN_SIZE_Y][SCREEN_SIZE_X];
    int screen_x, screen_y;

    int song_line_idx;
    int song_line_char_idx;
    bool song_line_event_processed;
};

///////////////////////////////////////////////////////////////////////////////

static struct timespec get_time(void)
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts;
}

static int elapsed_ms(struct timespec start)
{
    struct timespec current = get_time();
    current.tv_sec -= start.tv_sec;
    current.tv_nsec -= start.tv_nsec;
    return current.tv_sec * 1000 + current.tv_nsec / 1000000;
}

///////////////////////////////////////////////////////////////////////////////

static STATION_SFUNC(sfunc_init);

static STATION_SFUNC(sfunc_song);

static STATION_PFUNC(pfunc_draw_background);
static STATION_PFUNC(pfunc_draw_characters);
static STATION_SFUNC(sfunc_loop);

///////////////////////////////////////////////////////////////////////////////

static STATION_SFUNC(sfunc_init)
{
    struct plugin_resources *resources = fsm_data;

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

    int comment_pos_x = SCREEN_SIZE_X - PICTURE_SIZE_X;
    int comment_pos_y = (SCREEN_SIZE_Y - PICTURE_SIZE_Y) / 2 - 1;

    memcpy(&resources->screen[comment_pos_y + 0][comment_pos_x], COMMENT0, strlen(COMMENT0));
    memcpy(&resources->screen[comment_pos_y + 1][comment_pos_x], COMMENT1, strlen(COMMENT1));
    memcpy(&resources->screen[comment_pos_y + 2][comment_pos_x], COMMENT2, strlen(COMMENT2));

    resources->screen_x = 0;
    resources->screen_y = 0;

    resources->song_line_idx = 0;
    resources->song_line_char_idx = 0;
    resources->song_line_event_processed = false;

    resources->start_time = get_time();

    state->sfunc = sfunc_loop;
}

static STATION_SFUNC(sfunc_song)
{
    struct plugin_resources *resources = fsm_data;

    int elapsed = elapsed_ms(resources->start_time);
    const struct song_line *current_line = &song[resources->song_line_idx];

    resources->screen[1 + resources->screen_y][1 + resources->screen_x] =
        ((elapsed / CURSOR_FLICKERING_PERIOD) % 2) ? '_' : ' '; // cursor

    if (current_line->text == NULL)
    {
        if (elapsed < current_line->end)
            state->sfunc = sfunc_loop;
        else
            state->sfunc = NULL;

        return;
    }

    const struct song_line *next_line = &song[resources->song_line_idx + 1];

    if (!resources->song_line_event_processed)
    {
        int pposx = SCREEN_SIZE_X - PICTURE_SIZE_X;
        int pposy = SCREEN_SIZE_Y - PICTURE_SIZE_Y;
        int picidx;

        switch (current_line->event)
        {
            case EVENT_NONE:
                break;

            case EVENT_START_MUSIC:
                SDL_PauseAudioDevice(resources->snd_device_id, 0);
                break;

            case EVENT_CLEAR_SCREEN:
                resources->screen[1 + resources->screen_y][1 + resources->screen_x] = ' ';

                for (int y = 0; y < TEXT_SIZE_Y; y++)
                    for (int x = 0; x < TEXT_SIZE_X; x++)
                        resources->screen[1 + y][1 + x] = ' ';

                resources->screen_x = 0;
                resources->screen_y = 0;
                break;

            default:
                picidx = current_line->event - EVENT_DRAW_PICTURE;

                for (int y = 0; y < PICTURE_SIZE_Y; y++)
                    for(int x = 0; x < PICTURE_SIZE_X; x++)
                        resources->screen[pposy + y][pposx + x] = song_picture[picidx][y][x];
                break;
        }

        resources->song_line_event_processed = true;
    }

    int line_length = strlen(current_line->text);
    int line_visible_chars = line_length;

    if ((elapsed < current_line->end) && (current_line->start < current_line->end))
    {
        float line_progress = 1.0f * (elapsed - current_line->start) /
            (current_line->end - current_line->start);
        line_visible_chars = line_progress * line_length;
    }

    for (int x = resources->song_line_char_idx; x < line_visible_chars; x++)
    {
        char current_chr = current_line->text[x];

        if (current_chr != '\n')
        {
            resources->screen[1 + resources->screen_y][1 + resources->screen_x + 1] =
                resources->screen[1 + resources->screen_y][1 + resources->screen_x];
            resources->screen[1 + resources->screen_y][1 + resources->screen_x] = current_chr;
            resources->screen_x++;
        }
        else
        {
            resources->screen[1 + resources->screen_y][1 + resources->screen_x] = ' ';
            resources->screen_x = 0;
            resources->screen_y++;
        }
    }

    if (resources->song_line_char_idx < line_visible_chars)
        resources->song_line_char_idx = line_visible_chars;

    if ((line_visible_chars == line_length) && (elapsed >= next_line->start))
    {
        resources->song_line_idx++;
        resources->song_line_char_idx = 0;
        resources->song_line_event_processed = false;
    }

    state->sfunc = sfunc_loop;
}

///////////////////////////////////////////////////////////////////////////////

static STATION_PFUNC(pfunc_draw_background)
{
    (void) thread_idx;

    struct plugin_resources *resources = data;

    station_task_idx_t y = task_idx / resources->sdl_window.texture.width;

    uint32_t pixel = BG_COLOR0;
    if ((y - (elapsed_ms(resources->start_time)) / BG_FLICKERING_PERIOD) % BG_FLICKERING_MOD == 0)
        pixel = BG_COLOR1;

    resources->sdl_window.texture.lock.pixels[task_idx] = pixel;
}

static STATION_PFUNC(pfunc_draw_characters)
{
    (void) thread_idx;

    struct plugin_resources *resources = data;

    station_task_idx_t x = task_idx % SCREEN_SIZE_X;
    station_task_idx_t y = task_idx / SCREEN_SIZE_X;

    char str[2] = {resources->screen[y][x], '\0'};
    const unsigned char *glyph = station_font_psf2_glyph(str, 1, NULL, resources->font);

    station_sdl_window_texture_draw_glyph(&resources->sdl_window,
            x * resources->font->header->width, y * resources->font->header->height,
            true, false, TEXT_COLOR, 0,
            glyph, resources->font->header->width, resources->font->header->height,
            0, 0, resources->font->header->width, resources->font->header->height);
}

static STATION_SFUNC(sfunc_loop)
{
    struct plugin_resources *resources = fsm_data;

    SDL_PollEvent(&resources->event);

    if ((resources->event.type == SDL_QUIT) ||
            ((resources->event.type == SDL_KEYDOWN) &&
             (resources->event.key.keysym.sym == SDLK_ESCAPE)))
        exit(0);

    {
        if (station_sdl_window_lock_texture(&resources->sdl_window,
                    true, 0, 0, 0, 0) != 0)
        {
            printf("station_sdl_window_lock_texture() failure\n");
            exit(1);
        }

        station_parallel_processing_execute(resources->parallel_processing_context,
                pfunc_draw_background, resources,
                resources->sdl_window.texture.width * resources->sdl_window.texture.height, 0);

        station_parallel_processing_execute(resources->parallel_processing_context,
                pfunc_draw_characters, resources,
                SCREEN_SIZE_X * SCREEN_SIZE_Y, 0);

        if (station_sdl_window_unlock_texture_and_render(&resources->sdl_window) != 0)
        {
            printf("station_sdl_window_unlock_texture_and_render() failure\n");
            exit(1);
        }
    }

    SDL_Delay(10); // don't overheat the computer

    state->sfunc = sfunc_song;
}

///////////////////////////////////////////////////////////////////////////////

static STATION_PLUGIN_HELP_FUNC(plugin_help)
{
    (void) argc;
    (void) argv;

    printf("usage: still-alive [no arguments]\n");
}

static STATION_PLUGIN_CONF_FUNC(plugin_conf)
{
    (void) argc;
    (void) argv;

    args->sdl_is_used = true;
#ifdef STATION_IS_SDL_SUPPORTED
    args->sdl_init_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
#endif
}

static STATION_PLUGIN_INIT_FUNC(plugin_init)
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

    outputs->fsm_initial_state.sfunc = sfunc_init;
    outputs->fsm_data = resources;

    resources->parallel_processing_context = inputs->parallel_processing_context;

    // Load the font
    {
        station_buffer_t font_buffer = {
            .num_bytes = &_binary_font_psf_end - _binary_font_psf_start,
            .bytes = (void*)_binary_font_psf_start,
        };

        resources->font = station_load_font_psf2_from_buffer(&font_buffer);
        if (resources->font == NULL)
        {
            printf("Couldn't load built-in font\n");
            goto failure;
        }
        step = 2;
    }

    // Load the song
    {
        if (SDL_LoadWAV_RW(SDL_RWFromConstMem(_binary_song_wav_start,
                        &_binary_song_wav_end - _binary_song_wav_start),
                    1, &resources->wav_spec, &resources->wav_buffer, &resources->wav_length) == NULL)
        {
            printf("Couldn't load built-in .wav file\n");
            goto failure;
        }
        step = 3;

        resources->snd_device_id = SDL_OpenAudioDevice(NULL, 0, &resources->wav_spec, NULL, 0);
        if (resources->snd_device_id == 0)
        {
            printf("Couldn't open audio device\n");
            goto failure;
        }
        step = 4;

        if (SDL_QueueAudio(resources->snd_device_id, resources->wav_buffer, resources->wav_length) < 0)
        {
            printf("Couldn't queue WAV to play\n");
            exit(1);
        }
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

    return;

failure:
    if (step >= 5)
        station_sdl_destroy_window_context(&resources->sdl_window);

    if (step >= 4)
        SDL_CloseAudioDevice(resources->snd_device_id);

    if (step >= 3)
        SDL_FreeWAV(resources->wav_buffer);

    if (step >= 2)
        station_unload_font_psf2(resources->font);

    if (step >= 1)
        free(resources);

    exit(1);
}

static STATION_PLUGIN_FINAL_FUNC(plugin_final)
{
    (void) quick;

    struct plugin_resources *resources = plugin_resources;

    station_sdl_destroy_window_context(&resources->sdl_window);
    SDL_CloseAudioDevice(resources->snd_device_id);
    SDL_FreeWAV(resources->wav_buffer);
    station_unload_font_psf2(resources->font);
    free(resources);

    return 0;
}

STATION_PLUGIN("Portal credits song, \"Still Alive\"", plugin_help, plugin_conf, plugin_init, plugin_final)

