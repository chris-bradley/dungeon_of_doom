#include <SDL.h>
#include <SDL_ttf.h>

#include "dungeon_lib.h"

Uint8 BG$ [4][4] {
    {0x00, 0x00, 0x00, 0},
    {0x88, 0x20, 0x00, 0},
    {0xf0, 0xe8, 0x58, 0},
    {0xff, 0xff, 0xff, 0}
};

void print_text(screen_t *screen, cursor_t *cursor, const char *message) {
    TTF_Font *c64_font = TTF_OpenFont(
        "fonts/dungeon_of_doom.ttf",
        8 * screen->zoom
    );
    if (!c64_font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // Do some proper error handling.
        return;
    }
    int message_length = (int) strlen(message);
    SDL_Rect text_pos = {
        .x = cursor->curs_x * 8 * screen->zoom,
        .y = cursor->curs_y * 8 * screen->zoom,
        .w = message_length * 8 * screen->zoom,
        .h = 8 * screen->zoom
    };
    cursor->curs_x += message_length;

    Uint8 r, g, b;
    // Background
    int error = SDL_SetRenderDrawColor(
        screen->ren,
        cursor->background_colour[0],
        cursor->background_colour[1],
        cursor->background_colour[2],
        cursor->background_colour[3]
    );
    if (error) {
        printf("SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
    }
    error = SDL_RenderFillRect(screen->ren, &text_pos);
    if (error) {
        printf("SDL_RenderFillRect error: %s\n", SDL_GetError());
    }
    // Foreground
    r = cursor->foreground_colour[0];
    g = cursor->foreground_colour[1];
    b = cursor->foreground_colour[2];

    SDL_Color text_color = {
        .r = r,
        .g = g,
        .b = b
    };

    SDL_Surface *text_surface = TTF_RenderText_Solid(
        c64_font,
        message,
        text_color
    );

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(
        screen->ren,
        text_surface
    );

    text_pos.y += 1;
    text_pos.h += 1;

    error = SDL_RenderCopy(
        screen->ren,
        text_texture,
        NULL,
        &text_pos
    );
    if (error) {
        printf("SDL_RenderCopy error: %s\n", SDL_GetError());
        fflush(stdout);
    }
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    TTF_CloseFont(c64_font);
}

void ink(cursor_t *cursor, int c_num) {
    cursor->foreground_colour[0] = BG$[c_num][0];
    cursor->foreground_colour[1] = BG$[c_num][1];
    cursor->foreground_colour[2] = BG$[c_num][2];
    cursor->foreground_colour[3] = BG$[c_num][3];
}

void paper(cursor_t *cursor, int c_num) {
    cursor->background_colour[0] = BG$[c_num][0];
    cursor->background_colour[1] = BG$[c_num][1];
    cursor->background_colour[2] = BG$[c_num][2];
    cursor->background_colour[3] = BG$[c_num][3];
}
