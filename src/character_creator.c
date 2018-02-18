#include <SDL.h>
#include "dungeon_lib.h"

typedef struct {
    const char * label;
    int value;
} menu_item_t;

typedef struct {
    int selected_row;
    int num_rows;
    menu_item_t *items[8];
    int top_row;
} main_menu_t;

typedef struct {
    int id;
    const char * name;
} character_class_t;

typedef struct {
    int id;
    const char * name;
    int price;
    int batch_size;
} item_t;

typedef struct {
    const char * name;
    item_t items[8];
} store_t;

typedef struct {
    int attrs[8];
    int gold;
    int *inventory;
} character_t;

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
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
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

void buy_item(item_t *item, character_t *character, int max_accepted_discount,
              int offer, int item_for_class, char * message) {
    int price;
    if (character->inventory[item->id] > 0 && item->id < 22) {
        strcpy(message, "YOU HAVE IT SIRE");
    } else {
        price = item->price - max_accepted_discount;
        if (character->gold < price) {
            strcpy(message, "YOU CANNOT AFFORD");
        } else if (item_for_class == 1) {
            if (offer >= price)  {
                character->inventory[item->id] += item->batch_size;
                character->gold -= offer;
                strcpy(message, "TIS YOURS!");
            } else {
                strcpy(message, "OFFER REJECTED");
            }
        }
    }
}

void make_offer_for_item(screen_t *screen, item_t *item,
                         int max_accepted_discount,
                         character_class_t *character_class,
                         character_t *character, const char * point_label,
                         char * message,
                         const char * item_char_class_avail[24]) {
    int item_for_class, offer;
    char * typed_string = NULL;
    strcpy(message, "");
    update_header(screen, character->gold, point_label, message);
    tab(screen->cursor, 2, 2);
    print_text(screen, "YOUR OFFER");
    SDL_RenderPresent(screen->ren);
    typed_string = get_player_string(screen, 14, 2);
    offer = atoi(typed_string);
    free(typed_string);
    item_for_class = can_class_buy_item(
        character_class, item->id, message, item_char_class_avail
    );
    buy_item(
        item, character, max_accepted_discount, offer, item_for_class, message
    );
}

void select_row(screen_t *screen, main_menu_t *main_menu, char pressed_key) {
    int selected_row_pos =
        main_menu->selected_row * 2 + main_menu->top_row + 1;
    paper(screen->cursor, WHITE);
    ink(screen->cursor, RED);
    tab(screen->cursor, 1, selected_row_pos);
    print_text(screen, " ");
    if (pressed_key == 'a' && main_menu->selected_row > 0) {
        main_menu->selected_row -= 1;
    }
    else if (
            pressed_key == 'z' &&
            main_menu->selected_row < main_menu->num_rows - 1
    ) {
        main_menu->selected_row += 1;
    }
    selected_row_pos =
        main_menu->selected_row * 2 + main_menu->top_row + 1;
    tab(screen->cursor, 1, selected_row_pos);
    print_text(screen, ">");
}

void draw_header(screen_t *screen, int num_points, int screen_cols,
                 const char * point_label, char * message,
                 const char * stage_name) {
    paper(screen->cursor, BLACK);
    ink(screen->cursor, YELLOW);
    tab(screen->cursor, 0, 0);
    print_left$_b$(screen, screen_cols);
    tab(screen->cursor, 0, 0);
    print_text(screen, stage_name);
    draw_bordered_box(screen, 1, 0, 2, screen_cols - 2, YELLOW, WHITE);
    update_header(screen, num_points, point_label, message);
}

void update_main(screen_t *screen, main_menu_t *main_menu) {
    int index, row;
    paper(screen->cursor, WHITE);
    ink(screen->cursor, BLACK);
    for (index = 0; index < 8; index += 1) {
        row = main_menu->top_row + index * 2 + 1;
        tab(screen->cursor, 15, row);
        print_left$_b$(screen, 5);

        tab(screen->cursor, 2, row);
        print_text(screen, main_menu->items[index]->label);
        tab(screen->cursor, 16, row);
        char * outstring = (char *) malloc(sizeof(char) * 40);
        if (outstring == NULL) {
            fprintf(stderr, "outstring is NULL!\n");
            exit(1);
        }
        sprintf(outstring, "%i ", main_menu->items[index]->value);
        print_text(screen, outstring);
        free(outstring);
    }
}

void draw_main(screen_t *screen, main_menu_t *main_menu, int screen_cols) {
    main_menu->top_row = 5;
    draw_bordered_box(
        screen, main_menu->top_row, 0, 15, screen_cols - 2, WHITE, YELLOW
    );
    update_main(screen, main_menu);
}

void init_platform_vars(int *screen_cols) {
    *screen_cols = 40;
}

void init_vars(int *char_base, main_menu_t **main_menu, int *attr_points,
               int *screen_cols, character_t **character,
               character_class_t * character_classes[5], char ** message,
               const char * item_char_class_avail[24],
               const char * attr_names[8], store_t stores[3]) {
    int index;
    init_platform_vars(screen_cols);
    *main_menu = (main_menu_t *) malloc(sizeof(main_menu));
    (*main_menu)->num_rows = 8;
    (*character)->inventory = (int *) malloc(
        sizeof(int) * ((*main_menu)->num_rows) * 3
    );
    if ((*character)->inventory == NULL) {
        fprintf(stderr, "*inventory is NULL!\n");
        exit(1);
    }

    int i;
    for (i = 0; i < (*main_menu)->num_rows * 3; i += 1) {
        (*character)->inventory[i] = 0;
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
    *character = (character_t *) malloc(sizeof(character_t));
    for (index = 0; index < 8; index += 1) {
        (*character)->attrs[index] = (rand() % 5) + 2;
    }
    (*character)->attrs[4] = 1;

    attr_names[0] = "STRENGTH";
    attr_names[1] = "VITALITY";
    attr_names[2] = "AGILITY";
    attr_names[3] = "INTELLIGENCE";
    attr_names[4] = "EXPERIENCE";
    attr_names[5] = "LUCK";
    attr_names[6] = "AURA";
    attr_names[7] = "MORALITY";
    stores[0] = (store_t) {
        .name="ARMOURY",
        .items={
            (item_t) {
                .id=0,
                .name="2 HAND SWORD",
                .price=20,
                .batch_size=5
            },
            (item_t) {
                .id=1,
                .name="BROADSWORD",
                .price=16,
                .batch_size=4
            },
            (item_t) {
                .id=2,
                .name="SHORTSWORD",
                .price=12,
                .batch_size=3
            },
            (item_t) {
                .id=3,
                .name="AXE",
                .price=15,
                .batch_size=3
            },
            (item_t) {
                .id=4,
                .name="MACE",
                .price=8,
                .batch_size=2
            },
            (item_t) {
                .id=5,
                .name="FLAIL",
                .price=10,
                .batch_size=2
            },
            (item_t) {
                .id=6,
                .name="DAGGER",
                .price=8,
                .batch_size=1
            },
            (item_t) {
                .id=7,
                .name="GAUNTLET",
                .price=6,
                .batch_size=1
            }
        }
    };
    stores[1] = (store_t) {
        .name="ACCOUTREMENTS",
        .items={
            (item_t) {
                .id=8,
                .name="HEAVY ARMOUR",
                .price=18,
                .batch_size=5
            },
            (item_t) {
                .id=9,
                .name="CHAIN ARMOUR",
                .price=15,
                .batch_size=4
            },
            (item_t) {
                .id=10,
                .name="LEATHER ARMOUR",
                .price=9,
                .batch_size=3
            },
            (item_t) {
                .id=11,
                .name="HEAVY ROBE",
                .price=9,
                .batch_size=1
            },
            (item_t) {
                .id=12,
                .name="GOLD HELMET",
                .price=14,
                .batch_size=2
            },
            (item_t) {
                .id=13,
                .name="HEADPIECE",
                .price=8,
                .batch_size=1
            },
            (item_t) {
                .id=14,
                .name="SHIELD",
                .price=6,
                .batch_size=3
            },
            (item_t) {
                .id=15,
                .name="TORCH",
                .price=6,
                .batch_size=1
            }
        }
    };
    stores[2] = (store_t) {
        .name="EMPORIUM",
        .items={
            (item_t) {
                .id=16,
                .name="NECRONOMICON",
                .price=20,
                .batch_size=4
            },
            (item_t) {
                .id=17,
                .name="SCROLLS",
                .price=15,
                .batch_size=3
            },
            (item_t) {
                .id=18,
                .name="RING",
                .price=14,
                .batch_size=2
            },
            (item_t) {
                .id=19,
                .name="MYSTIC AMULET",
                .price=12,
                .batch_size=2
            },
            (item_t) {
                .id=20,
                .name="SASH",
                .price=10,
                .batch_size=3
            },
            (item_t) {
                .id=21,
                .name="CLOAK",
                .price=8,
                .batch_size=1
            },
            (item_t) {
                .id=22,
                .name="HEALING SALVE",
                .price=6,
                .batch_size=1
            },
            (item_t) {
                .id=23,
                .name="POTIONS",
                .price=6,
                .batch_size=1
            }
        }
    };

    for (index = 0; index < 5; index += 1) {
        character_classes[index] = malloc(sizeof(character_class_t));
    }
    *(character_classes[0]) = (character_class_t) {
        .id = 0,
        .name = "WANDERER"
    };
    *(character_classes[1]) = (character_class_t) {
        .id = 1,
        .name = "CLERIC"
    };
    *(character_classes[2]) = (character_class_t) {
        .id = 2,
        .name = "MAGE"
    };
    *(character_classes[3]) = (character_class_t) {
        .id = 3,
        .name = "WARRIOR"
    };
    *(character_classes[4]) = (character_class_t) {
        .id = 4,
        .name = "BARBARIAN"
    };
    *attr_points = 3 + (rand() % 5);
    (*character)->gold = 120 + (rand() % 60);
    *message = (char *) malloc(sizeof(char) * 40);
    if (*message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(*message, "");
    *char_base = 65;
}

int main(int argc, char *argv[]) {
    int char_base, max_accepted_discount, index, store_ind, attr_points,
        num_item_types, offer, screen_cols, col, row, item_for_class;
    character_class_t *character_class;
    character_class_t *character_classes[5];
    character_t *character;
    store_t stores[3];
    main_menu_t *main_menu;
    const char * point_label,
               * item_char_class_avail[24];
    char pressed_key, * typed_string = NULL, * message = NULL,
         * character_name;
    const char * attr_names[8];
    item_t * item;
    init_vars(
        &char_base, &main_menu, &attr_points, &screen_cols, &character,
        character_classes, &message, item_char_class_avail, attr_names, stores
    );
    screen_t *screen = NULL;
    if (init_screen(&screen) < 0) {
        return 1;
    }
    paper(screen->cursor, BLACK);
    point_label = "POINTS";
    draw_header(
        screen, attr_points, screen_cols, point_label, message,
        "CHARACTER CREATION"
    );
    main_menu->selected_row = 0;
    for (index = 0; index < 8; index += 1) {
        main_menu->items[index] = malloc(sizeof(main_menu_t));
        *main_menu->items[index] = (menu_item_t) {
            .label = attr_names[index],
            .value = character->attrs[index]
        };
    }
    draw_main(screen, main_menu, screen_cols);
    tab(screen->cursor, 1, main_menu->top_row + 1);
    print_text(screen, ">");
    SDL_RenderPresent(screen->ren);
    do {
        pressed_key = inkey$();
        select_row(screen, main_menu, pressed_key);
        while (main_menu->selected_row == 4) {
            pressed_key = inkey$();
            select_row(screen, main_menu, pressed_key);
        }
        if (pressed_key == ';' && attr_points > 0) {
            character->attrs[main_menu->selected_row] += 1;
            attr_points -= 1;
            main_menu->items[main_menu->selected_row]->value += 1;
            update_main(screen, main_menu);
        }
        if (
                pressed_key == '-' &&
                character->attrs[main_menu->selected_row] > 1
        ) {
            character->attrs[main_menu->selected_row] -=1;
            attr_points += 1;
            main_menu->items[main_menu->selected_row]->value -= 1;
            update_main(screen, main_menu);
        }
        character_class = character_classes[0];
        if (character->attrs[3] > 6 && character->attrs[7] > 7) {
            character_class = character_classes[1];
        }
        if (character->attrs[3] > 8 && character->attrs[6] > 7) {
            character_class = character_classes[2];
        }
        if (
                character->attrs[0] > 7 && character->attrs[7] > 5 &&
                character->attrs[0] + character->attrs[1] > 10
        ) {
            character_class = character_classes[3];
        }
        if (
                character->attrs[0] > 8 &&
                character->attrs[1] + character->attrs[2] > 12 &&
                character->attrs[7] < 6
        ) {
            character_class = character_classes[4];
        }
        strcpy(message, character_class->name);
        update_header(screen, attr_points, point_label, message);
        SDL_RenderPresent(screen->ren);
    } while (pressed_key != ' ');
    point_label = "GOLD COINS";
    for (store_ind = 0; store_ind < 3; store_ind += 1) {
        main_menu->selected_row = 0;
        strcpy(message, "CHOOSE WELL SIRE!");
        draw_header(
            screen, character->gold, screen_cols, point_label, message,
            stores[store_ind].name
        );
        for (index = 0; index < 8; index += 1) {
            item = &stores[store_ind].items[index];
            main_menu->items[index]->label = item->name;
            main_menu->items[index]->value = item->price;
        }
        draw_main(screen, main_menu, screen_cols);
        tab(screen->cursor, 1, main_menu->top_row + 1);
        print_text(screen, ">");
        do {
            SDL_RenderPresent(screen->ren);
            pressed_key = inkey$();
            select_row(screen, main_menu, pressed_key);
            item = &stores[store_ind].items[main_menu->selected_row];
            strcpy(message, "MAKE YOUR CHOICE");
            item_for_class = can_class_buy_item(
                character_class, item->id, message, item_char_class_avail
            );
            max_accepted_discount = 0;
            offer = 0;
            if (pressed_key == ';') {
                offer = item->price;
                buy_item(
                    item, character, max_accepted_discount, offer,
                    item_for_class, message
                );
            }
            if (pressed_key == '-') {
                max_accepted_discount = rand() % 3;
                make_offer_for_item(
                    screen, item, max_accepted_discount, character_class,
                    character, point_label, message, item_char_class_avail
                );
            }
            update_header(screen, character->gold, point_label, message);
        } while (pressed_key != ' ');
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
    num_item_types = main_menu->num_rows * 3;

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
            character->attrs[index] + char_base
        );
    }
    for (index = 0; index < num_item_types; index += 1) {
        save_file_contents[8 + index] = (char) (
            character->inventory[index] + char_base
        );
    }
    save_file_contents[9 + num_item_types] = (char) (
        character->gold + char_base
    );
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

    free(message);
    free(character_name);
    for (index = 0; index < 8; index += 1) {
        free(main_menu->items[index]);
    }
    free(main_menu);

    for (index = 1; index < 6; index += 1) {
        free(character_classes[index]);
    }

    free(character);
    destroy_screen(screen);

    return 0;
}
