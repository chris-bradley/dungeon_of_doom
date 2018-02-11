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
                       const char * item_char_class_avail[24]) {
    if (item_char_class_avail[item_num][character_class->id - 1] == '1') {
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
              int prices[3][8], int * inventory, char * message,
              int item_batch_size[23]) {
    int price;
    if (inventory[item_num] > 0 && item_num < 22) {
        strcpy(message, "YOU HAVE IT SIRE");
    } else {
        price = prices[stage - 1][selected_row] - max_accepted_discount;
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
                         int prices[3][8], int * inventory,
                         const char * point_label, char * message,
                         int item_batch_size[23],
                         const char * item_char_class_avail[24]) {
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
        offer, item_for_class, prices, inventory, message, item_batch_size
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
    if (*pressed_key == 'a' && *selected_row > 0) {
        *selected_row -= 1;
    }
    else if (*pressed_key == 'z' && *selected_row < interface_num_rows - 1) {
        *selected_row += 1;
    }
    *selected_row_pos = (*selected_row + 1) * 2 + top_row - 1;
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
                 const char * stage_names[4]) {
    int background_colour, border_colour, rows;
    paper(screen->cursor, 0);
    ink(screen->cursor, 2);
    tab(screen->cursor, 0, 0);
    print_left$_b$(screen, screen_cols);
    tab(screen->cursor, 0, 0);
    print_text(screen, stage_names[stage]);
    background_colour = 2;
    border_colour = 3;
    *top_row = 1;
    rows = 2;
    draw_box(
        screen, screen_cols, background_colour, border_colour, *top_row, rows
    );
    update_header(screen, num_points, point_label, message);
}

void update_main(screen_t *screen, int top_row, int values[8],
                 const char * labels[8]) {
    int index, row;
    paper(screen->cursor, 3);
    ink(screen->cursor, 0);
    for (index = 0; index < 8; index += 1) {
        row = top_row + index * 2 + 1;
        tab(screen->cursor, 15, row);
        print_left$_b$(screen, 5);

        tab(screen->cursor, 2, row);
        print_text(screen, labels[index]);
        tab(screen->cursor, 16, row);
        char * outstring = (char *) malloc(sizeof(char) * 40);
        if (outstring == NULL) {
            fprintf(stderr, "outstring is NULL!\n");
            exit(1);
        }
        sprintf(outstring, "%i ", values[index]);
        print_text(screen, outstring);
        free(outstring);
    }
}

void draw_main(screen_t *screen, int *top_row, int screen_cols, int values[8],
               const char * labels[8]) {
    int background_colour, border_colour, rows;
    background_colour = 3;
    border_colour = 2;
    *top_row = 5;
    rows = 15;
    draw_box(
        screen, screen_cols, background_colour, border_colour, *top_row, rows
    );
    update_main(screen, *top_row, values, labels);
}

void init_platform_vars(int *screen_cols) {
    *screen_cols = 40;
}

void init_vars(int *char_base, int *interface_num_rows, int *gold_coins,
               int *attr_points, int *screen_cols, int attrs[8],
               int prices[3][8], int ** inventory,
               character_class_t * character_classes[5], char ** message,
               int item_batch_size[23], const char * item_char_class_avail[24],
               const char * attr_names[8], const char * item_names[3][8],
               const char * stage_names[4]) {
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

    item_char_class_avail[0] = "00001";
    item_char_class_avail[1] = "00011";
    item_char_class_avail[2] = "10011";
    item_char_class_avail[3] = "10011";
    item_char_class_avail[4] = "10011";
    item_char_class_avail[5] = "00011";
    item_char_class_avail[6] = "11111";
    item_char_class_avail[7] = "10011";
    item_char_class_avail[8] = "00011";
    item_char_class_avail[9] = "00011";
    item_char_class_avail[10] = "10011";
    item_char_class_avail[11] = "11111";
    item_char_class_avail[12] = "00011";
    item_char_class_avail[13] = "11011";
    item_char_class_avail[14] = "11011";
    item_char_class_avail[15] = "11111";
    item_char_class_avail[16] = "11100";
    item_char_class_avail[17] = "00100";
    item_char_class_avail[18] = "11100";
    item_char_class_avail[19] = "10100";
    item_char_class_avail[20] = "11100";
    item_char_class_avail[21] = "11100";
    item_char_class_avail[22] = "11111";
    item_char_class_avail[23] = "11111";
    for (index = 0; index < 8; index += 1) {
        attrs[index] = (rand() % 5) + 2;
    }
    attrs[4] = 1;
    prices[0][0] = 20;
    prices[0][1] = 16;
    prices[0][2] = 12;
    prices[0][3] = 15;
    prices[0][4] = 8;
    prices[0][5] = 10;
    prices[0][6] = 8;
    prices[0][7] = 6;
    prices[1][0] = 18;
    prices[1][1] = 15;
    prices[1][2] = 9;
    prices[1][3] = 9;
    prices[1][4] = 14;
    prices[1][5] = 8;
    prices[1][6] = 6;
    prices[1][7] = 6;
    prices[2][0] = 20;
    prices[2][1] = 15;
    prices[2][2] = 14;
    prices[2][3] = 12;
    prices[2][4] = 10;
    prices[2][5] = 8;
    prices[2][6] = 6;
    prices[2][7] = 6;
    item_batch_size[0] = 5;
    item_batch_size[1] = 4;
    item_batch_size[2] = 3;
    item_batch_size[3] = 3;
    item_batch_size[4] = 2;
    item_batch_size[5] = 2;
    item_batch_size[6] = 1;
    item_batch_size[7] = 1;
    item_batch_size[8] = 5;
    item_batch_size[9] = 4;
    item_batch_size[10] = 3;
    item_batch_size[11] = 1;
    item_batch_size[12] = 2;
    item_batch_size[13] = 1;
    item_batch_size[14] = 3;
    item_batch_size[15] = 1;
    item_batch_size[16] = 4;
    item_batch_size[17] = 3;
    item_batch_size[18] = 2;
    item_batch_size[19] = 2;
    item_batch_size[20] = 3;
    item_batch_size[21] = 1;
    item_batch_size[22] = 1;
    item_batch_size[23] = 1;

    attr_names[0] = "STRENGTH";
    attr_names[1] = "VITALITY";
    attr_names[2] = "AGILITY";
    attr_names[3] = "INTELLIGENCE";
    attr_names[4] = "EXPERIENCE";
    attr_names[5] = "LUCK";
    attr_names[6] = "AURA";
    attr_names[7] = "MORALITY";
    stage_names[0] = "CHARACTER CREATION";
    item_names[0][0] = "2 HAND SWORD";
    item_names[0][1] = "BROADSWORD";
    item_names[0][2] = "SHORTSWORD";
    item_names[0][3] = "AXE";
    item_names[0][4] = "MACE";
    item_names[0][5] = "FLAIL";
    item_names[0][6] = "DAGGER";
    item_names[0][7] = "GAUNTLET";
    stage_names[1] = "ARMOURY";
    item_names[1][0] = "HEAVY ARMOUR";
    item_names[1][1] = "CHAIN ARMOUR";
    item_names[1][2] = "LEATHER ARMOUR";
    item_names[1][3] = "HEAVY ROBE";
    item_names[1][4] = "GOLD HELMET";
    item_names[1][5] = "HEADPIECE";
    item_names[1][6] = "SHIELD";
    item_names[1][7] = "TORCH";
    stage_names[2] = "ACCOUTREMENTS";
    item_names[2][0] = "NECRONOMICON";
    item_names[2][1] = "SCROLLS";
    item_names[2][2] = "RING";
    item_names[2][3] = "MYSTIC AMULET";
    item_names[2][4] = "SASH";
    item_names[2][5] = "CLOAK";
    item_names[2][6] = "HEALING SALVE";
    item_names[2][7] = "POTIONS";
    stage_names[3] = "EMPORIUM";

    for (index = 0; index < 5; index += 1) {
        character_classes[index] = malloc(sizeof(character_class_t));
    }
    character_classes[0]->id = 1;
    character_classes[0]->name = "WANDERER";
    character_classes[1]->id = 2;
    character_classes[1]->name = "CLERIC";
    character_classes[2]->id = 3;
    character_classes[2]->name = "MAGE";
    character_classes[3]->id = 4;
    character_classes[3]->name = "WARRIOR";
    character_classes[4]->id = 5;
    character_classes[4]->name = "BARBARIAN";
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
    int attrs[8], prices[3][8];
    int * inventory;
    character_class_t *character_class;
    character_class_t *character_classes[5];

    const char * point_label,
               * item_char_class_avail[24];
    char * pressed_key, * typed_string = NULL, * message = NULL,
         * character_name;
    int item_batch_size[23];
    const char * attr_names[8], * item_names[3][8], * stage_names[4];
    init_vars(
        &char_base, &interface_num_rows, &gold_coins, &attr_points,
        &screen_cols, attrs, prices, &inventory, character_classes, &message,
        item_batch_size, item_char_class_avail, attr_names, item_names,
        stage_names
    );
    screen_t *screen = NULL;
    if (init_screen(&screen) < 0) {
        return 1;
    }
    paper(screen->cursor, 0);
    stage = 0;
    point_label = "POINTS";
    draw_header(
        screen, stage, attr_points, &top_row, screen_cols, point_label,
        message, stage_names
    );
    draw_main(screen, &top_row, screen_cols, attrs, attr_names);
    selected_row = 0;
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
        while (selected_row == 4) {
            get_input_and_select_row(
                screen, interface_num_rows, &selected_row, &selected_row_pos,
                top_row, pressed_key
            );
        }
        if (*pressed_key == ';' && attr_points > 0) {
            attrs[selected_row] += 1;
            attr_points -= 1;
            update_main(screen, top_row, attrs, attr_names);
        }
        if (*pressed_key == '-' && attrs[selected_row] > 1) {
            attrs[selected_row] -=1;
            attr_points += 1;
            update_main(screen, top_row, attrs, attr_names);
        }
        character_class = character_classes[0];
        if (attrs[3] > 6 && attrs[7] > 7) {
            character_class = character_classes[1];
        }
        if (attrs[3] > 8 && attrs[6] > 7) {
            character_class = character_classes[2];
        }
        if (attrs[0] > 7 && attrs[7] > 5 && attrs[0] + attrs[1] > 10) {
            character_class = character_classes[3];
        }
        if (attrs[0] > 8 && attrs[1] + attrs[2] > 12 && attrs[7] < 6) {
            character_class = character_classes[4];
        }
        strcpy(message, character_class->name);
        update_header(screen, attr_points, point_label, message);
        SDL_RenderPresent(screen->ren);
    } while (*pressed_key != ' ');
    point_label = "GOLD COINS";
    for (stage = 1; stage < 4; stage += 1) {
        selected_row = 0;
        selected_row_pos = top_row + 1;
        strcpy(message, "CHOOSE WELL SIRE!");
        draw_header(
            screen, stage, gold_coins, &top_row, screen_cols, point_label,
            message, stage_names
        );
        draw_main(
            screen, &top_row, screen_cols, prices[stage - 1],
            item_names[stage - 1]
        );
        tab(screen->cursor, 1, selected_row_pos);
        print_text(screen, ">");
        do {
            SDL_RenderPresent(screen->ren);
            get_input_and_select_row(
                screen, interface_num_rows, &selected_row, &selected_row_pos,
                top_row, pressed_key
            );
            item_num = 8 * (stage - 1) + selected_row;
            strcpy(message, "MAKE YOUR CHOICE");
            item_for_class = can_class_buy_item(
                character_class, item_num, message, item_char_class_avail
            );
            max_accepted_discount = 0;
            offer = 0;
            if (*pressed_key == ';') {
                offer = prices[stage - 1][selected_row];
                buy_item(
                    max_accepted_discount, stage, &gold_coins, selected_row,
                    item_num, offer, item_for_class, prices, inventory,
                    message, item_batch_size
                );
            }
            if (*pressed_key == '-') {
                max_accepted_discount = rand() % 3;
                make_offer_for_item(
                    screen, max_accepted_discount, character_class, stage,
                    &gold_coins, selected_row, item_num, prices, inventory,
                    point_label, message, item_batch_size,
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
    for (index = 0; index < 8; index +=1 ) {
        save_file_contents[index + 1] = (char) (
            attrs[index] + char_base
        );
    }
    for (index = 0; index < num_item_types; index += 1) {
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
