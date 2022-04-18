#include <errno.h>
#include <SDL.h>
#include "dungeon_lib.h"
#include "libcoord.h"
#include "libmap.h"


void place_item(coord_t cur_coord, dungeon_t * dungeon, char pressed_key) {
    int pressed_key_num = atoi(&pressed_key);
    if (pressed_key_num == 9) {
        pressed_key_num = 9 + (rand() % 3);
    } else if (pressed_key_num == 5) {
        coord_t_set(&dungeon->entrance_coord, cur_coord);
    }
    dungeon->contents[cur_coord.x][cur_coord.y] = pressed_key_num;
}

void draw_help(screen_t * screen, const char * help_lines[10]) {
    int index;
    SDL_Rect * help_line_rect;
    paper(screen->cursor, RED);
    ink(screen->cursor, WHITE);
    for (index = 0; index < 10; index += 1) {
        tab(screen->cursor, 1, 4);
        help_line_rect = print_text(screen, help_lines[index]);
        SDL_RenderPresent(screen->ren);
        inkey$();
        clear_rect(screen, help_line_rect, RED);
        free(help_line_rect);
    }
    SDL_RenderPresent(screen->ren);
}

dungeon_t * init_level(int level_num) {
    dungeon_t * dungeon = malloc(sizeof(dungeon_t));
    if (dungeon == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "dungeon is NULL!");
        exit(1);
    }
    coord_t coord;
    for (coord.x = 0; coord.x < 15; coord.x++) {
        for (coord.y = 0; coord.y < 15; coord.y++) {
            dungeon->contents[coord.x][coord.y] = 0;
        }
    }
    dungeon->entrance_coord.x = -1;
    dungeon->entrance_coord.y = -1;
    dungeon->level_num = level_num;
    return dungeon;
}

dungeon_t * save_level(screen_t * screen, dungeon_t * dungeon) {
    int char_base = 96,
        char_code_blank = char_base + 6;
    SDL_Rect * text_rect;
    dungeon_t * new_dungeon;
    paper(screen->cursor, RED);
    ink(screen->cursor, WHITE);
    coord_t coord;
    int error;
    tab(screen->cursor, 1, 4);
    text_rect = print_text(screen, "ANY KEY TO SAVE");
    SDL_RenderPresent(screen->ren);
    inkey$();
    char * level_dir = get_level_dir();
    char * level_path = malloc(sizeof(char) * (strlen(level_dir) + 7));
    sprintf(level_path, "%s%cLEVEL", level_dir, PATHSEP);
    free(level_dir);
    errno = 0;
    FILE * save_file_handle = fopen(level_path, "w");
    if (save_file_handle == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "save_file_handle is NULL!");
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "Error: %d (%s).", errno, strerror(errno)
        );
        exit(1);
    }
    free(level_path);
    for (coord.y = 0; coord.y < 15; coord.y += 1) {
        for (coord.x = 0; coord.x < 15; coord.x += 1) {
            if (
                    !fputc(
                        (char) dungeon->contents[coord.x][coord.y] +
                            char_code_blank,
                        save_file_handle
                    )
            ) {
                SDL_LogError(
                    SDL_LOG_CATEGORY_SYSTEM,
                    "Error %i writing dungeon contents at %i/%i!",
                    ferror(save_file_handle), coord.x, coord.y
                );
            }
        }
    }
    if (
            !fputc(
                (char) dungeon->entrance_coord.x + char_base + 1,
                save_file_handle
            )
    ) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM,
            "Error %i writing dungeon entrance coord x!",
            ferror(save_file_handle)
        );
    }
    if (
            !fputc(
                (char) dungeon->entrance_coord.y + char_base + 1,
                save_file_handle
            )
    ) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM,
            "Error %i writing dungeon entrance coord y!",
            ferror(save_file_handle)
        );
    }
    if (!fputc((char) dungeon->level_num + char_base, save_file_handle)) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM, "Error %i writing the level number!",
            ferror(save_file_handle)
        );
    }

    error = fclose(save_file_handle);
    if (error) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM, "Error %i saving the level!", error
        );
    }
    tab(screen->cursor, 1, 4);
    clear_rect(screen, text_rect, RED);
    free(text_rect);
    SDL_RenderPresent(screen->ren);
    new_dungeon = init_level(dungeon->level_num + 1);
    free(dungeon);
    return new_dungeon;
}

int init_screen_cols() {
    return 40;
}

const char ** init_help_lines() {
    const char ** help_lines = malloc(sizeof(const char *) * 10);
    if (help_lines == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "help_lines is NULL!");
        exit(1);
    }
    help_lines[0] = "PRESS ANY KEY";
    help_lines[1] = "TO MOVE A Z N M";
    help_lines[2] = "1 WALL    2 VASE";
    help_lines[3] = "3 CHEST 4 * IDOL *";
    help_lines[4] = "5 WAY IN  6 EXIT";
    help_lines[5] = "7 TRAP";
    help_lines[6] = "8 SAFE PLACE";
    help_lines[7] = "9 GUARD";
    help_lines[8] = "0 TO ERASE";
    help_lines[9] = "S TO SAVE";
    return help_lines;
}

int main(int argc, char * argv[]) {
    handle_args(argc, argv);
    int screen_cols;
    coord_t cur_coord;
    const char ** help_lines = init_help_lines();
    char pressed_key;

    screen_cols = init_screen_cols();
    dungeon_t * dungeon = init_level(1);
    screen_t * screen = init_screen();
    draw_bordered_box(screen, 0, 0, 3, screen_cols - 3, YELLOW, RED);
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    tab(screen->cursor, 1, 1);
    free(print_text(screen, "LEVEL GENERATOR"));
    tab(screen->cursor, 1, 2);
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "Allocating outstring failed!"
        );
        exit(1);
    }
    snprintf(outstring, 40, "THIS IS LEVEL: %i", dungeon->level_num);
    free(print_text(screen, outstring));
    free(outstring);
    tab(screen->cursor, 1, 3);
    free(print_text(screen, "PRESS H FOR HELP"));

    draw_bordered_box(screen, 5, 0, 15, 15, WHITE, YELLOW);
    SDL_RenderPresent(screen->ren);

    cur_coord = (coord_t) {
        .x = 0,
        .y = 0
    };

    int done = 0;
    while (!done) {
        pressed_key = inkey$();

        if (pressed_key == 'h') {
            draw_help(screen, help_lines);
        } else if (pressed_key == 'a' && cur_coord.y > 0) {
            cur_coord.y -= 1;
        } else if (pressed_key == 'z' && cur_coord.y < 14) {
            cur_coord.y += 1;
        } else if (pressed_key == 'n' && cur_coord.x > 0) {
            cur_coord.x -= 1;
        } else if (pressed_key == 'm' && cur_coord.x < 14) {
            cur_coord.x += 1;
        } else if (pressed_key > '/' && pressed_key < ':') {
            place_item(cur_coord, dungeon, pressed_key);
        }
        render_bitmap(
            screen, cur_coord.x + 1, cur_coord.y + 6,
            dungeon->contents[cur_coord.x][cur_coord.y] + 6, BLACK, WHITE
        );

        SDL_RenderPresent(screen->ren);
        if (pressed_key == 's' && dungeon->entrance_coord.x > -1) {
            dungeon = save_level(screen, dungeon);
        }
        if (pressed_key == 'f') {
            done = SDL_TRUE;
        }
    }
    free(dungeon);
    destroy_screen(screen);
    free(help_lines);
    return 0;
}
