#include <SDL.h>
#include "dungeon_lib.h"

typedef struct {
    int x;
    int y;
} coord_t;

typedef struct {
    int contents[16][16];
    coord_t entrance_coord;
} dungeon_t;


void place_item(coord_t cur_coord, dungeon_t *dungeon, char pressed_key,
                int char_code_blank) {
    int pressed_key_num = atoi(&pressed_key);
    if (pressed_key_num == 9) {
        pressed_key_num = 9 + (rand() % 3);
    }
    else if (pressed_key_num == 5) {
       dungeon->entrance_coord.x = cur_coord.x;
       dungeon->entrance_coord.y = cur_coord.y;
    }
    dungeon->contents[cur_coord.x][cur_coord.y] =
        char_code_blank + pressed_key_num;
}

void draw_help(screen_t *screen, int screen_cols,
               const char * help_lines[10]) {

    int index;
    SDL_Rect * help_line_rect;
    paper(screen->cursor, RED);
    ink(screen->cursor, WHITE);
    for (index = 0; index < 10; index += 1) {
        tab(screen->cursor, 1, 4);
        help_line_rect = print_text(screen, help_lines[index]);
        SDL_RenderPresent(screen->ren);
        inkey$();
        clear_box(screen, help_line_rect, RED);
        free(help_line_rect);
    }
    SDL_RenderPresent(screen->ren);
    ink(screen->cursor, WHITE);
}

void init_level(dungeon_t * dungeon, int char_code_blank) {
    for (int coord_x=0; coord_x<16; coord_x++) {
        for (int coord_y=0; coord_y<16; coord_y++) {
            dungeon->contents[coord_x][coord_y] = char_code_blank;
        }
    }
    dungeon->entrance_coord.x = 0;
    dungeon->entrance_coord.y = 0;
}

void save_level(screen_t *screen, int screen_cols, int *level_num,
                int char_base, dungeon_t * dungeon, int char_code_blank) {
    tab(screen->cursor, 1, 4);
    free(print_text(screen, "ONE MOMENT PLEASE"));
    char save_file_contents[239];
    int coord_x, coord_y;
    for (coord_x = 1; coord_x <= 15; coord_x += 1) {
        for (coord_y = 1; coord_y <= 15; coord_y += 1) {
            save_file_contents[(coord_x - 1) * 15 + coord_y - 1] =
                (char) dungeon->contents[coord_x][coord_y];

        }
    }
    save_file_contents[225] = (char) dungeon->entrance_coord.x + char_base;
    save_file_contents[226] = (char) dungeon->entrance_coord.y + char_base;
    save_file_contents[227] = (char) *level_num + char_base;
    save_file_contents[228] = 0;
    tab(screen->cursor, 1, 4);
    free(print_text(screen, "ANY KEY TO SAVE   "));
    SDL_RenderPresent(screen->ren);
    inkey$();
    FILE *save_file_handle = fopen("LEVEL", "w");
    int error = fputs(save_file_contents, save_file_handle);
    if (error) {
        fprintf(stderr, "Error %i writing the level!", error);
    }
    error = fclose(save_file_handle);
    if (error) {
        fprintf(stderr, "Error %i saving the level!", error);
    }
    tab(screen->cursor, 1, 4);
    print_left$_b$(screen, screen_cols);
    SDL_RenderPresent(screen->ren);
    *level_num = *level_num + 1;
    init_level(dungeon, char_code_blank);
}

void init_platform_vars(int *char_base, int *screen_cols,
                        int *char_code_blank) {
  *char_base = 96;
  *char_code_blank = *char_base + 6;
  *screen_cols = 40;
}

void init_vars(int *screen_cols, int *level_num, int *char_base,
               dungeon_t * dungeon, int *char_code_blank,
               const char * help_lines[10]) {
    init_platform_vars(char_base, screen_cols, char_code_blank);
    *level_num = 1;
    help_lines[0] = "PRESS ANY KEY     ";
    help_lines[1] = "TO MOVE A Z N M   ";
    help_lines[2] = "1 WALL    2 VASE  ";
    help_lines[3] = "3 CHEST 4 * IDOL *";
    help_lines[4] = "5 WAY IN  6 EXIT  ";
    help_lines[5] = "7 TRAP            ";
    help_lines[6] = "8 SAFE PLACE      ";
    help_lines[7] = "9 GUARD           ";
    help_lines[8] = "0 TO ERASE        ";
    help_lines[9] = "S TO SAVE         ";

    init_level(dungeon, *char_code_blank);
}

int main(int argc, char *argv[]) {
    int char_code_blank, level_num, char_base, screen_cols;
    coord_t cur_coord;
    dungeon_t dungeon;
    const char * help_lines[10];
    char pressed_key;

    init_vars(
        &screen_cols, &level_num, &char_base, &dungeon, &char_code_blank,
        help_lines
    );
    screen_t *screen = init_screen();
    draw_bordered_box(screen, 0, 0, 3, screen_cols - 3, YELLOW, RED);
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    tab(screen->cursor, 1, 1);
    free(print_text(screen, "LEVEL GENERATOR"));
    tab(screen->cursor, 1, 2);
    char* outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
       fprintf(stdout, "Allocating outstring failed!");
       exit(1);
    }
    snprintf(outstring, 40, "THIS IS LEVEL: %i", level_num);
    free(print_text(screen, outstring));
    free(outstring);
    tab(screen->cursor, 1, 3);
    free(print_text(screen, "PRESS H FOR HELP"));

    draw_bordered_box(screen, 5, 0, 15, 15, WHITE, YELLOW);
    SDL_RenderPresent(screen->ren);

    cur_coord = (coord_t) {
        .x = 1,
        .y = 1
    };

    int done = 0;
    while (!done) {
        pressed_key = inkey$();

        if (pressed_key == 'h') {
            draw_help(screen, screen_cols, help_lines);
        } else if (pressed_key == 'a' && cur_coord.y > 1) {
            cur_coord.y -= 1;
        } else if (pressed_key == 'z' && cur_coord.y < 15) {
            cur_coord.y += 1;
        } else if (pressed_key == 'n' && cur_coord.x > 1) {
            cur_coord.x -= 1;
        } else if (pressed_key == 'm' && cur_coord.x < 15) {
            cur_coord.x += 1;
        } else if (pressed_key > '/' && pressed_key < ':') {
            place_item(cur_coord, &dungeon, pressed_key, char_code_blank);
        }
        render_bitmap(
            screen, cur_coord.x, cur_coord.y + 5,
            dungeon.contents[cur_coord.x][cur_coord.y] - char_code_blank + 6,
            BLACK, WHITE
        );

        SDL_RenderPresent(screen->ren);
        if (pressed_key == 's' && dungeon.entrance_coord.x > 0) {
            save_level(
                screen, screen_cols, &level_num, char_base, &dungeon,
                char_code_blank
            );
        }
        if (pressed_key == 'f') {
            done = SDL_TRUE;
        }
    }
    destroy_screen(screen);

    return 0;
}
