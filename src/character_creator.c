#include <SDL.h>
#include "dungeon_lib.h"

typedef struct {
    int id;
    const char * name;
} character_class_t;

char * get_player_string(screen_t *screen, int col, int row) {
    int ind = 0;
    char pressed_key, * typed_string;
    typed_string = (char *) malloc(sizeof(char) * 40);
    if (typed_string == NULL) {
        fprintf(stderr, "typed_string is NULL!\n");
        exit(1);
    }
    typed_string[0] = 0;
    while (1) {
        pressed_key = inkey$();
        if (pressed_key == '\n') {
            return typed_string;
        } else if (pressed_key > '/' && pressed_key < ']' && ind < 39) {
            typed_string[ind] = pressed_key;
            ind += 1;
            typed_string[ind] = 0;
            tab(screen->cursor, col, row);
            print_text(screen, typed_string);
            SDL_RenderPresent(screen->ren);
        }
    }
}

void update_header(screen_t *screen, int num_points, const char * point_label,
                   char * message) {
    paper(screen->cursor, 2);
    ink(screen->cursor, 0);
    tab(screen->cursor, 2, 2);
    print_left$_b$(screen, 17);
    tab(screen->cursor, 2, 2);
    print_text(screen, message);
    tab(screen->cursor, 15, 3);
    print_left$_b$(screen, 4);
    tab(screen->cursor, 2, 3);
    print_text(screen, point_label);
    tab(screen->cursor, 15, 3);
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    sprintf(outstring, "%i ", num_points);
    print_text(screen, outstring);
    free(outstring);
}

int can_class_buy_item(character_class_t *character_class, int item_num,
                       char * message,
                       const char * item_char_class_avail[25]) {
    if (item_char_class_avail[item_num][character_class->id] == '1') {
        return 1;
    }
    sprintf(
        message,
        "NOT FOR %s",
        character_class->name
    );
    return 0;
}

void buy_item(int max_accepted_discount, int stage, int *gold_coins,
              int selected_row, int item_num, int offer, int item_for_class,
              int attrs_and_prices[5][9], int * inventory, char * message,
              int item_batch_size[24]) {
    int price;
    if (inventory[item_num] > 0 && item_num < 23) {
        strcpy(message, "YOU HAVE IT SIRE");
    } else {
        price = attrs_and_prices[stage][selected_row] - max_accepted_discount;
        if (*gold_coins < price) {
            strcpy(message, "YOU CANNOT AFFORD");
        } else {
            if (offer >= price && item_for_class == 1) {
                inventory[item_num] += item_batch_size[item_num];
                *gold_coins -= price;
                strcpy(message, "TIS YOURS!");
            }
            if (offer < price && item_for_class == 1) {
                strcpy(message, "OFFER REJECTED");
            }
            if (*gold_coins < 0) {
                *gold_coins = 0;
            }
        }
    }
}

void make_offer_for_item(screen_t *screen, int max_accepted_discount,
                         character_class_t *character_class, int stage,
                         int *gold_coins, int selected_row, int item_num,
                         int attrs_and_prices[5][9], int * inventory,
                         const char * point_label, char * message,
                         int item_batch_size[24],
                         const char * item_char_class_avail[25]) {
    int item_for_class, offer, col, row;
    char * typed_string = NULL;
    strcpy(message, "");
    update_header(screen, *gold_coins, point_label, message);
    tab(screen->cursor, 2, 2);
    print_text(screen, "YOUR OFFER");
    SDL_RenderPresent(screen->ren);
    col = 14;
    row = 2;
    typed_string = get_player_string(screen, col, row);
    offer = atoi(typed_string);
    free(typed_string);
    item_for_class = can_class_buy_item(
        character_class, item_num, message, item_char_class_avail
    );
    buy_item(
        max_accepted_discount, stage, gold_coins, selected_row, item_num,
        offer, item_for_class, attrs_and_prices, inventory, message,
        item_batch_size
    );
}

void get_input_and_select_row(screen_t *screen, int interface_num_rows,
                              int *selected_row, int *selected_row_pos,
                              int top_row, char * pressed_key) {
    *pressed_key = inkey$();
    paper(screen->cursor, 3);
    ink(screen->cursor, 1);
    tab(screen->cursor, 1, *selected_row_pos);
    print_text(screen, " ");
    if (*pressed_key == 'a' && *selected_row > 1) {
        *selected_row -= 1;
    }
    else if (*pressed_key == 'z' && *selected_row < interface_num_rows) {
        *selected_row += 1;
    }
    *selected_row_pos = *selected_row * 2 + top_row - 1;
    tab(screen->cursor, 1, *selected_row_pos);
    print_text(screen, ">");
}

void draw_box(screen_t *screen, int screen_cols, int background_colour,
              int border_colour, int top_row, int rows) {
    int index;
    tab(screen->cursor, 0, top_row);
    paper(screen->cursor, border_colour);
    print_left$_b$(screen, screen_cols);
    newline(screen->cursor);
    paper(screen->cursor, background_colour);
    ink(screen->cursor, border_colour);
    for (index = 1; index <= rows; index += 1) {
        paper(screen->cursor, border_colour);
        print_text(screen, " ");
        paper(screen->cursor, background_colour);
        print_left$_b$(screen, screen_cols - 2);
        paper(screen->cursor, border_colour);
        print_text(screen, " ");
        // print_text() doesn't support wrapping yet, so we do our own newline:
        newline(screen->cursor);
    }
    paper(screen->cursor, border_colour);
    print_left$_b$(screen, screen_cols);
}

void draw_header(screen_t *screen, int stage, int num_points, int *top_row,
                 int screen_cols, const char * point_label, char * message,
                 const char * attr_item_and_stage_names[5][10]) {
    int background_colour, border_colour, rows;
    paper(screen->cursor, 0);
    ink(screen->cursor, 2);
    tab(screen->cursor, 0, 0);
    print_left$_b$(screen, screen_cols);
    tab(screen->cursor, 0, 0);
    print_text(screen, attr_item_and_stage_names[stage][9]);
    background_colour = 2;
    border_colour = 3;
    *top_row = 1;
    rows = 2;
    draw_box(
        screen, screen_cols, background_colour, border_colour, *top_row, rows
    );
    update_header(screen, num_points, point_label, message);
}

void update_main(screen_t *screen, int stage, int top_row,
                 int attrs_and_prices[5][9],
                 const char * attr_item_and_stage_names[5][10]) {
    int index, row;
    paper(screen->cursor, 3);
    ink(screen->cursor, 0);
    for (index = 1; index <= 8; index += 1) {
        row = top_row + (index - 1) * 2 + 1;
        tab(screen->cursor, 15, row);
        print_left$_b$(screen, 5);

        tab(screen->cursor, 2, row);
        print_text(screen, attr_item_and_stage_names[stage][index]);
        tab(screen->cursor, 16, row);
        char * outstring = (char *) malloc(sizeof(char) * 40);
        if (outstring == NULL) {
            fprintf(stderr, "outstring is NULL!\n");
            exit(1);
        }
        sprintf(outstring, "%i ", attrs_and_prices[stage][index]);
        print_text(screen, outstring);
        free(outstring);
    }
}

void draw_main(screen_t *screen, int stage, int *top_row, int screen_cols,
               int attrs_and_prices[5][9],
               const char * attr_item_and_stage_names[5][10]) {
    int background_colour, border_colour, rows;
    background_colour = 3;
    border_colour = 2;
    *top_row = 5;
    rows = 15;
    draw_box(
        screen, screen_cols, background_colour, border_colour, *top_row, rows
    );
    update_main(
        screen, stage, *top_row, attrs_and_prices, attr_item_and_stage_names
    );
}

void init_platform_vars(int *screen_cols) {
    *screen_cols = 40;
}

void init_vars(int *char_base, int *interface_num_rows, int *gold_coins,
               int *attr_points, int *screen_cols, int attrs_and_prices[5][9],
               int ** inventory, character_class_t * character_classes[5],
               char ** message, int item_batch_size[24],
               const char * item_char_class_avail[25],
               const char * attr_item_and_stage_names[5][10]) {
    int index;
    init_platform_vars(screen_cols);
    *interface_num_rows = 8;
    *inventory = (int *) malloc(sizeof(int) * (*interface_num_rows) * 3);
    if (*inventory == NULL) {
        fprintf(stderr, "*inventory is NULL!\n");
        exit(1);
    }

    int i;
    for (i = 0; i < *interface_num_rows * 3; i += 1) {
        (*inventory)[i] = 0;
    }

    item_char_class_avail[1] = "00001";
    item_char_class_avail[2] = "00011";
    item_char_class_avail[3] = "10011";
    item_char_class_avail[4] = "10011";
    item_char_class_avail[5] = "10011";
    item_char_class_avail[6] = "00011";
    item_char_class_avail[7] = "11111";
    item_char_class_avail[8] = "10011";
    item_char_class_avail[9] = "00011";
    item_char_class_avail[10] = "00011";
    item_char_class_avail[11] = "10011";
    item_char_class_avail[12] = "11111";
    item_char_class_avail[13] = "00011";
    item_char_class_avail[14] = "11011";
    item_char_class_avail[15] = "11011";
    item_char_class_avail[16] = "11111";
    item_char_class_avail[17] = "11100";
    item_char_class_avail[18] = "00100";
    item_char_class_avail[19] = "11100";
    item_char_class_avail[20] = "10100";
    item_char_class_avail[21] = "11100";
    item_char_class_avail[22] = "11100";
    item_char_class_avail[23] = "11111";
    item_char_class_avail[24] = "11111";
    for (index = 1; index <= 8; index += 1) {
        attrs_and_prices[1][index] = (rand() % 5) + 2;
    }
    attrs_and_prices[1][5] = 1;
    attrs_and_prices[2][1] = 20;
    attrs_and_prices[2][2] = 16;
    attrs_and_prices[2][3] = 12;
    attrs_and_prices[2][4] = 15;
    attrs_and_prices[2][5] = 8;
    attrs_and_prices[2][6] = 10;
    attrs_and_prices[2][7] = 8;
    attrs_and_prices[2][8] = 6;
    attrs_and_prices[3][1] = 18;
    attrs_and_prices[3][2] = 15;
    attrs_and_prices[3][3] = 9;
    attrs_and_prices[3][4] = 9;
    attrs_and_prices[3][5] = 14;
    attrs_and_prices[3][6] = 8;
    attrs_and_prices[3][7] = 6;
    attrs_and_prices[3][8] = 6;
    attrs_and_prices[4][1] = 20;
    attrs_and_prices[4][2] = 15;
    attrs_and_prices[4][3] = 14;
    attrs_and_prices[4][4] = 12;
    attrs_and_prices[4][5] = 10;
    attrs_and_prices[4][6] = 8;
    attrs_and_prices[4][7] = 6;
    attrs_and_prices[4][8] = 6;
    item_batch_size[1] = 5;
    item_batch_size[2] = 4;
    item_batch_size[3] = 3;
    item_batch_size[4] = 3;
    item_batch_size[5] = 2;
    item_batch_size[6] = 2;
    item_batch_size[7] = 1;
    item_batch_size[8] = 1;
    item_batch_size[9] = 5;
    item_batch_size[10] = 4;
    item_batch_size[11] = 3;
    item_batch_size[12] = 1;
    item_batch_size[13] = 2;
    item_batch_size[14] = 1;
    item_batch_size[15] = 3;
    item_batch_size[16] = 1;
    item_batch_size[17] = 4;
    item_batch_size[18] = 3;
    item_batch_size[19] = 2;
    item_batch_size[20] = 2;
    item_batch_size[21] = 3;
    item_batch_size[22] = 1;
    item_batch_size[23] = 1;
    item_batch_size[24] = 1;

    attr_item_and_stage_names[1][1] = "STRENGTH";
    attr_item_and_stage_names[1][2] = "VITALITY";
    attr_item_and_stage_names[1][3] = "AGILITY";
    attr_item_and_stage_names[1][4] = "INTELLIGENCE";
    attr_item_and_stage_names[1][5] = "EXPERIENCE";
    attr_item_and_stage_names[1][6] = "LUCK";
    attr_item_and_stage_names[1][7] = "AURA";
    attr_item_and_stage_names[1][8] = "MORALITY";
    attr_item_and_stage_names[1][9] = "CHARACTER CREATION";
    attr_item_and_stage_names[2][1] = "2 HAND SWORD";
    attr_item_and_stage_names[2][2] = "BROADSWORD";
    attr_item_and_stage_names[2][3] = "SHORTSWORD";
    attr_item_and_stage_names[2][4] = "AXE";
    attr_item_and_stage_names[2][5] = "MACE";
    attr_item_and_stage_names[2][6] = "FLAIL";
    attr_item_and_stage_names[2][7] = "DAGGER";
    attr_item_and_stage_names[2][8] = "GAUNTLET";
    attr_item_and_stage_names[2][9] = "ARMOURY";
    attr_item_and_stage_names[3][1] = "HEAVY ARMOUR";
    attr_item_and_stage_names[3][2] = "CHAIN ARMOUR";
    attr_item_and_stage_names[3][3] = "LEATHER ARMOUR";
    attr_item_and_stage_names[3][4] = "HEAVY ROBE";
    attr_item_and_stage_names[3][5] = "GOLD HELMET";
    attr_item_and_stage_names[3][6] = "HEADPIECE";
    attr_item_and_stage_names[3][7] = "SHIELD";
    attr_item_and_stage_names[3][8] = "TORCH";
    attr_item_and_stage_names[3][9] = "ACCOUTREMENTS";
    attr_item_and_stage_names[4][1] = "NECRONOMICON";
    attr_item_and_stage_names[4][2] = "SCROLLS";
    attr_item_and_stage_names[4][3] = "RING";
    attr_item_and_stage_names[4][4] = "MYSTIC AMULET";
    attr_item_and_stage_names[4][5] = "SASH";
    attr_item_and_stage_names[4][6] = "CLOAK";
    attr_item_and_stage_names[4][7] = "HEALING SALVE";
    attr_item_and_stage_names[4][8] = "POTIONS";
    attr_item_and_stage_names[4][9] = "EMPORIUM";

    for (index = 1; index < 6; index += 1) {
        character_classes[index] = malloc(sizeof(character_class_t));
    }
    character_classes[1]->id = 1;
    character_classes[1]->name = "WANDERER";
    character_classes[2]->id = 2;
    character_classes[2]->name = "CLERIC";
    character_classes[3]->id = 3;
    character_classes[3]->name = "MAGE";
    character_classes[4]->id = 4;
    character_classes[4]->name = "WARRIOR";
    character_classes[5]->id = 5;
    character_classes[5]->name = "BARBARIAN";
    *attr_points = 3 + (rand() % 5);
    *gold_coins = 120 + (rand() % 60);
    *message = (char *) malloc(sizeof(char) * 40);
    if (*message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(*message, "");
    *char_base = 65;
}

int main(int argc, char *argv[]) {
    int char_base, max_accepted_discount, interface_num_rows, gold_coins,
        index, stage, selected_row, attr_points, item_num, num_item_types,
        offer, selected_row_pos, top_row, screen_cols, col, row,
        item_for_class;
    int attrs_and_prices[5][9];
    int * inventory;
    character_class_t *character_class;
    character_class_t *character_classes[5];

    const char * point_label,
               * item_char_class_avail[25];
    char * pressed_key, * typed_string = NULL, * message = NULL,
         * character_name;
    int item_batch_size[24];
    const char * attr_item_and_stage_names[5][10];
    init_vars(
        &char_base, &interface_num_rows, &gold_coins, &attr_points,
        &screen_cols, attrs_and_prices, &inventory, character_classes,
        &message, item_batch_size, item_char_class_avail,
        attr_item_and_stage_names
    );
    screen_t *screen = NULL;
    if (init_screen(&screen) < 0) {
        return 1;
    }
    paper(screen->cursor, 0);
    stage = 1;
    point_label = "POINTS";
    draw_header(
        screen, stage, attr_points, &top_row, screen_cols, point_label,
        message, attr_item_and_stage_names
    );
    draw_main(
        screen, stage, &top_row, screen_cols, attrs_and_prices,
        attr_item_and_stage_names
    );
    selected_row = 1;
    selected_row_pos = top_row + 1;
    tab(screen->cursor, 1, selected_row_pos);
    print_text(screen, ">");
    SDL_RenderPresent(screen->ren);
    pressed_key = (char *) malloc(sizeof(char));
    if (pressed_key == NULL) {
        fprintf(stderr, "pressed_key is NULL!\n");
        exit(1);
    }
    do {
        get_input_and_select_row(
            screen, interface_num_rows, &selected_row, &selected_row_pos,
            top_row, pressed_key
        );
        while (selected_row == 5) {
            get_input_and_select_row(
                screen, interface_num_rows, &selected_row, &selected_row_pos,
                top_row, pressed_key
            );
        }
        if (*pressed_key == ';' && attr_points > 0) {
            attrs_and_prices[stage][selected_row] += 1;
            attr_points -= 1;
            update_main(
                screen, stage, top_row, attrs_and_prices,
                attr_item_and_stage_names
            );
        }
        if (*pressed_key == '-' && attrs_and_prices[stage][selected_row] > 1) {
            attrs_and_prices[stage][selected_row] -=1;
            attr_points += 1;
            update_main(
                screen, stage, top_row, attrs_and_prices,
                attr_item_and_stage_names
            );
        }
        character_class = character_classes[1];
        if (attrs_and_prices[1][4] > 6 && attrs_and_prices[1][8] > 7) {
            character_class = character_classes[2];
        }
        if (attrs_and_prices[1][4] > 8 && attrs_and_prices[1][7] > 7) {
            character_class = character_classes[3];
        }
        if (
                attrs_and_prices[1][1] > 7 && attrs_and_prices[1][8] > 5 &&
                attrs_and_prices[1][1] + attrs_and_prices[1][2] > 10
        ) {
            character_class = character_classes[4];
        }
        if (
                attrs_and_prices[1][1] > 8 &&
                attrs_and_prices[1][2] + attrs_and_prices[1][3] > 12 &&
                attrs_and_prices[1][8] < 6
        ) {
            character_class = character_classes[5];
        }
        strcpy(message, character_class->name);
        update_header(screen, attr_points, point_label, message);
        SDL_RenderPresent(screen->ren);
    } while (*pressed_key != ' ');
    point_label = "GOLD COINS";
    for (stage = 2; stage <= 4; stage += 1) {
        selected_row = 1;
        selected_row_pos = top_row + 1;
        strcpy(message, "CHOOSE WELL SIRE!");
        draw_header(
            screen, stage, gold_coins, &top_row, screen_cols, point_label,
            message, attr_item_and_stage_names
        );
        draw_main(
            screen, stage, &top_row, screen_cols, attrs_and_prices,
            attr_item_and_stage_names
        );
        tab(screen->cursor, 1, selected_row_pos);
        print_text(screen, ">");
        SDL_RenderPresent(screen->ren);
        do {
            get_input_and_select_row(
                screen, interface_num_rows, &selected_row, &selected_row_pos,
                top_row, pressed_key
            );
            item_num = 8 * (stage - 2) + selected_row;
            strcpy(message, "MAKE YOUR CHOICE");
            item_for_class = can_class_buy_item(
                character_class, item_num, message, item_char_class_avail
            );
            max_accepted_discount = 0;
            offer = 0;
            if (*pressed_key == ';') {
                offer = attrs_and_prices[stage][selected_row];
                buy_item(
                    max_accepted_discount, stage, &gold_coins, selected_row,
                    item_num, offer, item_for_class, attrs_and_prices,
                    inventory, message, item_batch_size
                );
            }
            if (*pressed_key == '-') {
                max_accepted_discount = rand() % 3;
                make_offer_for_item(
                    screen, max_accepted_discount, character_class, stage,
                    &gold_coins, selected_row, item_num, attrs_and_prices,
                    inventory, point_label, message, item_batch_size,
                    item_char_class_avail
                );
            }
            update_header(screen, gold_coins, point_label, message);
        } while (*pressed_key != ' ');
    }
    character_name = (char *) malloc(sizeof(char) * 40);
    if (character_name == NULL) {
        fprintf(stderr, "character_name is NULL!\n");
        exit(1);
    }
    do {
        tab(screen->cursor, 1, 2);
        print_text(screen, "NAME THY CHARACTER");
        tab(screen->cursor, 1, 3);
        print_left$_b$(screen, screen_cols - 2);
        SDL_RenderPresent(screen->ren);
        tab(screen->cursor, 1, 3);

        col = 1;
        row = 3;
        typed_string = get_player_string(screen, col, row);
        strcpy(character_name, typed_string);
        free(typed_string);
    } while (strlen(character_name) > 10);
    tab(screen->cursor, 1, 3);
    print_text(screen, "ONE MOMENT PLEASE");
    SDL_RenderPresent(screen->ren);
    tab(screen->cursor, 1, 3);
    num_item_types = interface_num_rows * 3;

    char * save_file_contents = (char *) malloc(
        sizeof(char) * (
            14 + num_item_types + strlen(character_name) + strlen(
                character_class->name
            )
        )
    );
    if (save_file_contents == NULL) {
        fprintf(stderr, "save_file_contents is NULL!\n");
        exit(1);
    }

    save_file_contents[0] = (char) (num_item_types + char_base);
    for (index = 1; index <= 8; index +=1 ) {
        save_file_contents[index] = (char) (
            attrs_and_prices[1][index] + char_base
        );
    }
    for (index = 1; index <= num_item_types; index += 1) {
        save_file_contents[8 + index] = (char) (inventory[index] + char_base);
    }
    save_file_contents[9 + num_item_types] = (char) (gold_coins + char_base);
    save_file_contents[10 + num_item_types] = (char) char_base;
    strcpy(save_file_contents + 11 + num_item_types, character_name);
    strcpy(
        save_file_contents + 11 + num_item_types + strlen(character_name),
        " -"
    );
    strcpy(
        save_file_contents + 13 + num_item_types + strlen(character_name),
        character_class->name
    );
    save_file_contents[
        13 + num_item_types + strlen(character_name) + strlen(
            character_class->name
        )
    ] = 0;
    FILE *save_file_handle = fopen("HERO", "w");
    int error = fputs(save_file_contents, save_file_handle);
    if (error) {
        fprintf(stderr, "Error %i writing the character!", error);
    }

    error = fclose(save_file_handle);
    if (error) {
        fprintf(stderr, "Error %i saving the character!", error);
    }
    free(save_file_contents);

    free(pressed_key);
    free(message);
    free(character_name);
    free(inventory);

    for (index = 1; index < 6; index += 1) {
        free(character_classes[index]);
    }

    destroy_screen(screen);

    return 0;
}
