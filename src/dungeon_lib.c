#include <errno.h>
#include <sys/stat.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <unistd.h>

#include "dungeon_lib.h"

uint8_t colours[4][4] = {
    {0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE},
    {0x88, 0x20, 0x00, SDL_ALPHA_OPAQUE},
    {0xf0, 0xe8, 0x58, SDL_ALPHA_OPAQUE},
    {0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE}
};

SDL_Rect * print_text(screen_t * screen, const char * message) {
    int message_length = (int) strlen(message);
    if (!message_length) {
        return NULL;
    }
    SDL_Rect * text_pos;
    text_pos = malloc(sizeof(SDL_Rect));
    if (text_pos == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "text_pos is NULL!");
        exit(1);
    }
    *text_pos = (SDL_Rect) {
        .x = screen->cursor->curs_x * 8 * screen->zoom,
        .y = screen->cursor->curs_y * 8 * screen->zoom,
        .w = message_length * 8 * screen->zoom,
        .h = 8 * screen->zoom
    };
    screen->cursor->curs_x += message_length;

    // Background
    int error = SDL_SetRenderDrawColor(
            screen->ren, screen->cursor->background_colour[0],
            screen->cursor->background_colour[1],
            screen->cursor->background_colour[2],
            screen->cursor->background_colour[3]
        );
    if (error) {
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER, "SDL_SetRenderDrawColor error: %s",
            SDL_GetError()
        );
    }
    error = SDL_RenderFillRect(screen->ren, text_pos);
    if (error) {
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER, "SDL_RenderFillRect error: %s",
            SDL_GetError()
        );
    }
    // Foreground
    SDL_Color text_color = {
        .r = screen->cursor->foreground_colour[0],
        .g = screen->cursor->foreground_colour[1],
        .b = screen->cursor->foreground_colour[2],
        .a = screen->cursor->foreground_colour[3]
    };

    TTF_Font * c64_font = TTF_OpenFont(
            "fonts/dungeon_of_doom.ttf", 8 * screen->zoom
        );
    if (!c64_font) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_RENDER, "TTF_OpenFont: %s", TTF_GetError()
        );
        exit(1);
    }
    SDL_Surface * text_surface = TTF_RenderText_Solid(
            c64_font, message, text_color
        );

    SDL_Texture * text_texture = SDL_CreateTextureFromSurface(
            screen->ren, text_surface
        );

    text_pos->h += 1;

    error = SDL_RenderCopy(screen->ren, text_texture, NULL, text_pos);
    if (error) {
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER, "SDL_RenderCopy error: %s", SDL_GetError()
        );
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
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "SDL_Init error:%s", SDL_GetError()
        );
        SDL_Quit();
        exit(1);;
    }
    TTF_Init();
    screen = (screen_t *) malloc(sizeof(screen_t));
    if (screen == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "screen is NULL!");
        exit(1);
    }
    screen->zoom = 3;
    screen->win = SDL_CreateWindow(
        "Dungeon of Doom", 100 * screen->zoom, 100 * screen->zoom,
        320 * screen->zoom, 176 * screen->zoom, SDL_WINDOW_SHOWN
    );
    screen->ren = SDL_CreateRenderer(
        screen->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (screen->ren == NULL) {
        SDL_DestroyWindow(screen->win);
        SDL_LogCritical(
            SDL_LOG_CATEGORY_RENDER, "SDL_CreateRenderer Error: %s",
            SDL_GetError()
        );
        SDL_Quit();
        exit(1);;
    }
    SDL_RenderClear(screen->ren);
    SDL_RenderPresent(screen->ren);

    screen->cursor = (cursor_t *) malloc(sizeof(cursor_t));
    if (screen->cursor == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "screen->cursor is NULL!");
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
        screen->ren, screen->cursor->background_colour[0],
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
                    SDL_LogInfo(
                        SDL_LOG_CATEGORY_APPLICATION,
                        "Quit through quit event."
                    );
                    exit(1);
            }
        }
    }
    return key;
}

void draw_box(screen_t * screen, int top_row, int left_col, int rows, int cols,
              enum ColourNum colour) {
    int error;
    SDL_Rect rect;
    error = SDL_SetRenderDrawColor(
        screen->ren, colours[colour][0], colours[colour][1],
        colours[colour][2], colours[colour][3]
    );
    if (error) {
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER, "SDL_SetRenderDrawColor error: %s\n",
            SDL_GetError()
        );
    }
    rect = (SDL_Rect) {
        .x = left_col * 8 * screen->zoom,
        .y = top_row * 8 * screen->zoom,
        .w = cols * 8 * screen->zoom,
        .h = rows * 8 * screen->zoom
    };
    error = SDL_RenderFillRect(screen->ren, &rect);
    if (error) {
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER, "SDL_RenderFillRect!: %s", SDL_GetError()
        );
    }
}

void draw_bordered_box(screen_t * screen, int top_row, int left_col, int rows,
                       int cols, enum ColourNum background_colour,
                       enum ColourNum border_colour) {
    draw_box(screen, top_row, left_col, rows + 2, cols + 2, border_colour);
    draw_box(screen, top_row + 1, left_col + 1, rows, cols, background_colour);
}

void clear_rect(screen_t * screen, SDL_Rect * rect, enum ColourNum colour) {
    int error = SDL_SetRenderDrawColor(
            screen->ren, colours[colour][0], colours[colour][1],
            colours[colour][2], colours[colour][3]
        );
    if (error) {
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER, "SDL_SetRenderDrawColor error: %s",
            SDL_GetError()
        );
    }
    error = SDL_RenderFillRect(screen->ren, rect);
    if (error) {
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER, "SDL_RenderFillRect!: %s", SDL_GetError()
        );
    }
}

uint8_t bitmaps[18][8] = {
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
            0, 8 * screen->zoom, 8 * screen->zoom, 32, 0, 0, 0, 0
        );
    if (surface == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER, "SDL_CreateRGBSurface error: %s",
            SDL_GetError()
        );
    }
    uint32_t foreground_pix_colour = SDL_MapRGBA(
            surface->format, colours[foreground_colour][0],
            colours[foreground_colour][1], colours[foreground_colour][2],
            colours[foreground_colour][3]
        ),
        background_pix_colour = SDL_MapRGBA(
            surface->format, colours[background_colour][0],
            colours[background_colour][1], colours[background_colour][2],
            colours[background_colour][3]
        ), cur_colour;
    int source_row, bitmask, index, error, pixel_index = 0, i;
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
        SDL_LogError(
            SDL_LOG_CATEGORY_RENDER, "SDL_RenderCopy error: %s", SDL_GetError()
        );
    }
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

#ifdef linux

#include <pwd.h>
#include <unistd.h>

char * get_home_dir() {
    struct passwd * user_info;
    char * home_dir = getenv("HOME");
    if (home_dir != NULL && strlen(home_dir) != 0) {
        return home_dir;
    }
    user_info = getpwuid(getuid());
    if (user_info == NULL) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM,
            "Unable to find user home directory! Exiting."
        );
        exit(1);
    }
    return user_info->pw_dir;
}

#endif

#ifdef WIN32
char * get_home_dir() {
    char * home_dir = getenv("USERPROFILE");
    if (home_dir == NULL || strlen(home_dir) == 0) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM,
            "Unable to find user home directory! Exiting."
        );
        exit(1);
    }
    return home_dir;
}

#endif

#define GAMEDIR "dungeon_of_doom_1.0"

#define CHARACTERDIR "characters"
#define LEVELDIR "levels"

void mkdir_if_not_exists(char * dir) {
    errno = 0;
    struct stat * dir_stat = malloc(sizeof(struct stat));
    if (dir_stat == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "dir_stat is NULL!");
        exit(1);
    }
    int stat_result = stat(dir, dir_stat);
    if (stat_result == -1 && errno != ENOENT) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "Error checking %s existence!", dir
        );
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "Error: %d (%s).", errno, strerror(errno)
        );
        exit(1);
    }
    if (stat_result == 0) {
        if (dir_stat->st_mode & S_IFDIR) {
            // dir already exists and is a directory. Free memory and exit.
            free(dir_stat);
            return;
        }
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s has a non-directory mode of %x!", dir,
            dir_stat->st_mode
        );
        exit(1);
    }
    free(dir_stat);
#ifdef linux
    int mkdir_result = mkdir(dir, S_IRWXU);
#endif
#ifdef WIN32
    int mkdir_result = mkdir(dir);
#endif
    if (mkdir_result == -1 && errno != EEXIST) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "Unable to create game directory!"
        );
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "Error: %d (%s).", errno, strerror(errno)
        );
        exit(1);
    }
}

char * get_game_dir() {
    char * home_dir = get_home_dir();
    char * game_dir = malloc(
            sizeof(char) * (strlen(home_dir) + strlen(GAMEDIR) + 2)
        );
    if (game_dir == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "game_dir is NULL!");
        exit(1);
    }
    sprintf(game_dir, "%s%c%s", home_dir, PATHSEP, GAMEDIR);
    mkdir_if_not_exists(game_dir);
    return game_dir;
}

char * get_character_dir() {
    char * game_dir = get_game_dir();
    char * character_dir = malloc(
            sizeof(char) * (strlen(game_dir) + strlen(CHARACTERDIR) + 2)
        );
    if (character_dir == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "character_dir is NULL!");
        exit(1);
    }
    sprintf(character_dir, "%s%c%s", game_dir, PATHSEP, CHARACTERDIR);
    free(game_dir);
    mkdir_if_not_exists(character_dir);
    return character_dir;
}

char * get_level_dir() {
    char * game_dir = get_game_dir();
    char * level_dir = malloc(
            sizeof(char) * (strlen(game_dir) + strlen(LEVELDIR) + 2)
        );
    if (level_dir == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "level_dir is NULL!");
        exit(1);
    }
    sprintf(level_dir, "%s%c%s", game_dir, PATHSEP, LEVELDIR);
    free(game_dir);
    mkdir_if_not_exists(level_dir);
    return level_dir;
}

void handle_args(int argc, char * argv[]) {
    int next_arg = getopt(argc, argv, "v::");
    SDL_LogPriority priority;
    while (next_arg != -1) {
        if (next_arg == 'v') {
            if (!optarg) {
                priority = SDL_LOG_PRIORITY_ERROR;
            } else if (!strcmp(optarg, "0")) {
                priority = SDL_LOG_PRIORITY_CRITICAL;
            } else if (!strcmp(optarg, "1")) {
                priority = SDL_LOG_PRIORITY_ERROR;
            } else if (!strcmp(optarg, "2")) {
                priority = SDL_LOG_PRIORITY_WARN;
            } else if (!strcmp(optarg, "3")) {
                priority = SDL_LOG_PRIORITY_INFO;
            } else if (!strcmp(optarg, "4")) {
                priority = SDL_LOG_PRIORITY_DEBUG;
            } else if (!strcmp(optarg, "5")) {
                priority = SDL_LOG_PRIORITY_VERBOSE;
            } else {
                SDL_LogCritical(
                    SDL_LOG_CATEGORY_SYSTEM, "invalid verbsoity argument %s!",
                    optarg
                );
                exit(1);
            }
            SDL_LogSetAllPriority(priority);
        }
        next_arg = getopt(argc, argv, "v::");
    }
}
