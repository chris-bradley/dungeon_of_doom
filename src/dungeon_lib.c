#include <SDL.h>
#include <SDL_ttf.h>

#include "dungeon_lib.h"

uint8_t colours [4][4] = {
    {0x00, 0x00, 0x00, 0},
    {0x88, 0x20, 0x00, 0},
    {0xf0, 0xe8, 0x58, 0},
    {0xff, 0xff, 0xff, 0}
};

SDL_Rect * print_text(screen_t * screen, const char * message) {
    TTF_Font * c64_font = TTF_OpenFont(
        "fonts/dungeon_of_doom.ttf",
        8 * screen->zoom
    );
    if (!c64_font) {
        fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
        exit(1);
    }
    int message_length = (int) strlen(message);
    SDL_Rect * text_pos;
    text_pos = malloc(sizeof(SDL_Rect));
    if (text_pos == NULL) {
        fprintf(stderr, "text_pos is NULL!\n");
        exit(1);
    }
    *text_pos = (SDL_Rect) {
        .x = screen->cursor->curs_x * 8 * screen->zoom,
        .y = screen->cursor->curs_y * 8 * screen->zoom,
        .w = message_length * 8 * screen->zoom,
        .h = 8 * screen->zoom
    };
    screen->cursor->curs_x += message_length;

    uint8_t r, g, b;
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
    error = SDL_RenderFillRect(screen->ren, text_pos);
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

    SDL_Surface * text_surface = TTF_RenderText_Solid(
        c64_font,
        message,
        text_color
    );

    SDL_Texture * text_texture = SDL_CreateTextureFromSurface(
        screen->ren,
        text_surface
    );

    text_pos->h += 1;

    error = SDL_RenderCopy(
        screen->ren,
        text_texture,
        NULL,
        text_pos
    );
    if (error) {
        fprintf(stderr, "SDL_RenderCopy error: %s\n", SDL_GetError());
        fflush(stderr);
    }
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    TTF_CloseFont(c64_font);

    text_pos->h -= 1;
    return text_pos;
}

void ink(cursor_t * cursor, enum ColourNum c_num) {
    cursor->foreground_colour[0] = colours[c_num][0];
    cursor->foreground_colour[1] = colours[c_num][1];
    cursor->foreground_colour[2] = colours[c_num][2];
    cursor->foreground_colour[3] = colours[c_num][3];
}

void paper(cursor_t * cursor, enum ColourNum c_num) {
    cursor->background_colour[0] = colours[c_num][0];
    cursor->background_colour[1] = colours[c_num][1];
    cursor->background_colour[2] = colours[c_num][2];
    cursor->background_colour[3] = colours[c_num][3];
}

void print_left$_b$(screen_t * screen, int width) {
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

void tab(cursor_t * cursor, int x, int y) {
    cursor->curs_x = x;
    cursor->curs_y = y;
}

void newline(cursor_t * cursor) {
    cursor->curs_x = 0;
    cursor->curs_y += 1;
}

screen_t * init_screen() {
    screen_t * screen;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL_Init error:%s\n", SDL_GetError());
        SDL_Quit();
        exit(1);;
    }
    TTF_Init();
    screen = (screen_t *) malloc(sizeof(screen_t));
    if (screen == NULL) {
        fprintf(stderr, "screen is NULL!\n");
        exit(1);
    }
    screen->zoom = 3;
    screen->win = SDL_CreateWindow(
        "Dungeon of Doom",
        100 * screen->zoom,
        100 * screen->zoom,
        320 * screen->zoom,
        176 * screen->zoom,
        SDL_WINDOW_SHOWN
    );
    screen->ren = SDL_CreateRenderer(
        screen->win,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (screen->ren == NULL) {
        SDL_DestroyWindow(screen->win);
        fprintf(stdout, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);;
    }
    SDL_RenderClear(screen->ren);
    SDL_RenderPresent(screen->ren);

    screen->cursor = (cursor_t *) malloc(sizeof(cursor_t));
    if (screen->cursor == NULL) {
        fprintf(stderr, "screen->cursor is NULL!\n");
        exit(1);
    }
    screen->cursor->curs_x = 0;
    screen->cursor->curs_y = 0;
    return screen;
}

void destroy_screen(screen_t * screen) {
    free(screen->cursor);

    TTF_Quit();
    SDL_DestroyRenderer(screen->ren);
    SDL_DestroyWindow(screen->win);
    SDL_Quit();
    free(screen);
}

void clear_screen(screen_t * screen) {
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
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_RETURN ||
                        event.key.keysym.sym == SDLK_RETURN2) {
                        key = '\n';
                        text_entered = 1;
                    }
                    break;
                case SDL_QUIT:
                    fprintf(stderr, "Quit through quit event.");
                    exit(1);
            }
        }
    }
    return key;
}

void draw_bordered_box(screen_t * screen, int top_row, int left_col, int rows,
                       int cols, enum ColourNum background_colour,
                       enum ColourNum border_colour) {
    int error;
    SDL_Rect rect;
    error = SDL_SetRenderDrawColor(
        screen->ren,
        colours[border_colour][0],
        colours[border_colour][1],
        colours[border_colour][2],
        colours[border_colour][3]
    );
    if (error) {
        fprintf(stderr, "SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
    }
    rect = (SDL_Rect) {
        .x = left_col * 8 * screen->zoom,
        .y = top_row * 8 * screen->zoom,
        .w = (cols + 2) * 8 * screen->zoom,
        .h = (rows + 2) * 8 * screen->zoom
    };
    error = SDL_RenderFillRect(screen->ren, &rect);
    if (error) {
        fprintf(stderr, "SDL_RenderFillRect!: %s\n", SDL_GetError());
    }
    error = SDL_SetRenderDrawColor(
        screen->ren,
        colours[background_colour][0],
        colours[background_colour][1],
        colours[background_colour][2],
        colours[background_colour][3]
    );
    if (error) {
        fprintf(stderr, "SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
    }
    rect = (SDL_Rect) {
        .x = (left_col + 1) * 8 * screen->zoom,
        .y = (top_row  + 1) * 8 * screen->zoom,
        .w = cols * 8 * screen->zoom,
        .h = rows * 8 * screen->zoom
    };
    error = SDL_RenderFillRect(screen->ren, &rect);
    if (error) {
        fprintf(stderr, "SDL_RenderFillRect!: %s\n", SDL_GetError());
    }
}

void clear_rect(screen_t * screen, SDL_Rect * rect, enum ColourNum colour) {
    int error = SDL_SetRenderDrawColor(
        screen->ren,
        colours[colour][0],
        colours[colour][1],
        colours[colour][2],
        colours[colour][3]
    );
    if (error) {
        fprintf(stderr, "SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
    }
    error = SDL_RenderFillRect(screen->ren, rect);
    if (error) {
        fprintf(stderr, "SDL_RenderFillRect!: %s\n", SDL_GetError());
    }
}

uint8_t bitmaps [18][8] = {
    {255, 255, 255, 255, 255, 255, 255, 255},
    {56, 56, 16, 56, 84, 16, 40, 68},
    {28, 28, 8, 30, 40, 40, 20, 18},
    {56, 56, 16, 84, 56, 40, 68, 40},
    {56, 56, 16, 120, 148, 20, 40, 72},
    {0, 0, 0, 0, 8, 209, 254, 221},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {85, 170, 85, 170, 85, 170, 85, 170},
    {0, 60, 24, 60, 126, 126, 126, 60},
    {0, 56, 100, 114, 95, 73, 41, 31},
    {20, 42, 20, 20, 93, 93, 62, 99},
    {60, 126, 255, 255, 255, 253, 255, 255},
    {60, 102, 195, 129, 129, 129, 133, 129},
    {129, 66, 36, 0, 0, 36, 66, 129},
    {0, 60, 66, 66, 66, 66, 60, 0},
    {76, 158, 170, 190, 84, 30, 37, 88},
    {0, 56, 84, 124, 56, 44, 68, 102},
    {0, 8, 28, 42, 127, 85, 65, 34}
};

void render_bitmap(screen_t * screen, int col, int row, int bitmap_num,
                   enum ColourNum foreground_colour,
                   enum ColourNum background_colour) {
    SDL_Surface * surface = SDL_CreateRGBSurface(
        0,
        8 * screen->zoom,
        8 * screen->zoom,
        32,
        0,
        0,
        0,
        0
    );
    if (surface == NULL) {
        fprintf(stderr, "SDL_CreateRGBSurface error: %s\n", SDL_GetError());
    }
    uint32_t foreground_pix_colour = SDL_MapRGBA(
            surface->format,
            colours[foreground_colour][0],
            colours[foreground_colour][1],
            colours[foreground_colour][2],
            colours[foreground_colour][3]
        ),
        background_pix_colour = SDL_MapRGBA(
            surface->format,
            colours[background_colour][0],
            colours[background_colour][1],
            colours[background_colour][2],
            colours[background_colour][3]
        ),
        cur_colour;
    int source_row, bitmask, index, error,
        pixel_index = 0,
        i;
    SDL_Texture * texture;
    SDL_Rect pos = {
        .x = col * 8 * screen->zoom,
        .y = row * 8 * screen->zoom,
        .w = 8 * screen->zoom,
        .h = 8 * screen->zoom
    };
    SDL_LockSurface(surface);
    for (source_row = 0; source_row < 8; source_row += 1) {
        for (index = 0; index < screen->zoom; index += 1) {
            for (bitmask = 7; bitmask >= 0; bitmask -= 1) {
                if ((1 << bitmask) & bitmaps[bitmap_num][source_row]) {
                    cur_colour = foreground_pix_colour;
                } else {
                    cur_colour = background_pix_colour;
                }
                for (i = 0; i < screen->zoom; i += 1) {
                    ((uint32_t *) surface->pixels)[pixel_index] = cur_colour;
                    pixel_index += 1;
                }
            }
        }
    }
    SDL_UnlockSurface(surface);
    texture = SDL_CreateTextureFromSurface(screen->ren, surface);

    error = SDL_RenderCopy(screen->ren, texture, NULL, &pos);
    if (error) {
        fprintf(stderr, "SDL_RenderCopy error: %s\n", SDL_GetError());
        fflush(stderr);
    }
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
