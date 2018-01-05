#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

#include "dungeon_lib.h"

Uint8 BG$ [4][4] {
    {0x00, 0x00, 0x00, 0},
    {0x88, 0x20, 0x00, 0},
    {0xf0, 0xe8, 0x58, 0},
    {0xff, 0xff, 0xff, 0}
};

void print_text(screen_t *screen, const char *message) {
    TTF_Font *c64_font = TTF_OpenFont(
        "fonts/dungeon_of_doom.ttf",
        8 * screen->zoom
    );
    if (!c64_font) {
        fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
        // Do some proper error handling.
        return;
    }
    int message_length = (int) strlen(message);
    SDL_Rect text_pos = {
        .x = screen->cursor->curs_x * 8 * screen->zoom,
        .y = screen->cursor->curs_y * 8 * screen->zoom,
        .w = message_length * 8 * screen->zoom,
        .h = 8 * screen->zoom
    };
    screen->cursor->curs_x += message_length;

    Uint8 r, g, b;
    // Background
    int error = SDL_SetRenderDrawColor(
        screen->ren,
        screen->cursor->background_colour[0],
        screen->cursor->background_colour[1],
        screen->cursor->background_colour[2],
        screen->cursor->background_colour[3]
    );
    if (error) {
        fprintf(stderr, "SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
    }
    error = SDL_RenderFillRect(screen->ren, &text_pos);
    if (error) {
        fprintf(stderr, "SDL_RenderFillRect error: %s\n", SDL_GetError());
    }
    // Foreground
    r = screen->cursor->foreground_colour[0];
    g = screen->cursor->foreground_colour[1];
    b = screen->cursor->foreground_colour[2];

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
        fprintf(stderr, "SDL_RenderCopy error: %s\n", SDL_GetError());
        fflush(stderr);
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

void print_left$_b$(screen_t *screen, int width) {
    int error = SDL_SetRenderDrawColor(
        screen->ren,
        screen->cursor->background_colour[0],
        screen->cursor->background_colour[1],
        screen->cursor->background_colour[2],
        screen->cursor->background_colour[3]
    );
    if (error) {
        fprintf(stderr, "SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
    }
    const SDL_Rect rect = {
        .x = screen->cursor->curs_x * 8 * screen->zoom,
        .y = screen->cursor->curs_y * 8 * screen->zoom,
        .w = width * 8 * screen->zoom,
        .h = 8 * screen->zoom
    };
    screen->cursor->curs_x += width;
    error = SDL_RenderFillRect(screen->ren, &rect);
    if (error) {
        fprintf(stderr, "SDL_RenderFillRect!: %s\n", SDL_GetError());
    }
}

void tab(cursor_t *cursor, int x, int y) {
    cursor->curs_x = x;
    cursor->curs_y = y;
}

void newline(cursor_t *cursor) {
    cursor->curs_x = 0;
    cursor->curs_y += 1;
}

int init_screen(screen_t **screen) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init error:" << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    TTF_Init();
    *screen = (screen_t *) malloc(sizeof(screen_t));
    if (*screen == NULL) {
       fprintf(stderr, "*screen is NULL!\n");
       exit(1);
    }
    (*screen)->zoom = 4;
    (*screen)->win = SDL_CreateWindow(
        "Dungeon of Doom",
        100 * (*screen)->zoom,
        100 * (*screen)->zoom,
        320 * (*screen)->zoom,
        176 * (*screen)->zoom,
        SDL_WINDOW_SHOWN
    );
    (*screen)->ren = SDL_CreateRenderer(
        (*screen)->win,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if ((*screen)->ren == NULL) {
        SDL_DestroyWindow((*screen)->win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() <<
            std::endl;
        SDL_Quit();
        return -1;
    }
    SDL_RenderClear((*screen)->ren);
    SDL_RenderPresent((*screen)->ren);

    (*screen)->cursor = (cursor_t *) malloc(sizeof(cursor_t));
    if ((*screen)->cursor == NULL) {
       fprintf(stderr, "*(screen)->cursor is NULL!\n");
       exit(1);
    }
    (*screen)->cursor->curs_x = 0;
    (*screen)->cursor->curs_y = 0;
    return 0;
}

void destroy_screen(screen_t *screen) {
    free(screen->cursor);

    TTF_Quit();
    SDL_DestroyRenderer(screen->ren);
    SDL_DestroyWindow(screen->win);
    SDL_Quit();
}

void clear_screen(screen_t *screen) {
    SDL_SetRenderDrawColor(
        screen->ren,
        screen->cursor->background_colour[0],
        screen->cursor->background_colour[1],
        screen->cursor->background_colour[2],
        screen->cursor->background_colour[3]
    );
    SDL_RenderClear(screen->ren);
    SDL_RenderPresent(screen->ren);
}

char inkey$() {
    int text_entered = 0;
    char key;
    SDL_Event event;
    while (!text_entered) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_TEXTINPUT:
                    key = event.text.text[0];
                    text_entered = 1;
                    break;
                case SDL_QUIT:
                    fprintf(stderr, "Quit through quit event.");
                    exit(1);
            }
        }
    }
    return key;
}