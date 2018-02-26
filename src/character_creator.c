#include <SDL.h>
#include "dungeon_lib.h"

typedef struct {
    const char *label;
    int points;
    int left_col;
    int top_row;
    int cols;
    int rows;
} header_t;

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

struct character_s_t;

typedef struct {
    int id;
    const char * name;
    int (*elig_func)(struct character_s_t *);
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

typedef struct character_s_t {
    int attrs[8];
    character_class_t * class;
    char * name;
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

void update_header(screen_t *screen, header_t *header, char * message) {
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    tab(screen->cursor, header->left_col + 2, header->top_row + 1);
    print_left$_b$(screen, 17);
    tab(screen->cursor, header->left_col + 2, header->top_row + 1);
    print_text(screen, message);
    tab(screen->cursor, header->left_col + 15, header->top_row + 2);
    print_left$_b$(screen, 4);
    tab(screen->cursor, header->left_col + 2, header->top_row + 2);
    print_text(screen, header->label);
    tab(screen->cursor, header->left_col + 15, header->top_row + 2);
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    sprintf(outstring, "%i ", header->points);
    print_text(screen, outstring);
    free(outstring);
}

int can_class_buy_item(character_class_t *character_class, int item_num,
                       char * message, int ** item_to_char_class) {
    if (item_to_char_class[item_num][character_class->id] == 1) {
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
              int offer, int item_for_class, char * message,
              header_t *header) {
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
                header->points -= offer;
            } else {
                strcpy(message, "OFFER REJECTED");
            }
        }
    }
}

void make_offer_for_item(screen_t *screen, item_t *item,
                         int max_accepted_discount,
                         character_t *character, char * message,
                         int ** item_to_char_class, header_t *header) {
    int item_for_class, offer;
    char * typed_string = NULL;
    strcpy(message, "");
    update_header(screen, header, message);
    tab(screen->cursor, 2, 2);
    print_text(screen, "YOUR OFFER");
    SDL_RenderPresent(screen->ren);
    typed_string = get_player_string(screen, 14, 2);
    offer = atoi(typed_string);
    free(typed_string);
    item_for_class = can_class_buy_item(
        character->class, item->id, message, item_to_char_class
    );
    buy_item(
        item, character, max_accepted_discount, offer, item_for_class, message,
        header
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

void draw_title_row(screen_t *screen, const char * stage_name, int screen_cols
                    )  {
    paper(screen->cursor, BLACK);
    ink(screen->cursor, YELLOW);
    tab(screen->cursor, 0, 0);
    print_left$_b$(screen, screen_cols);
    tab(screen->cursor, 0, 0);
    print_text(screen, stage_name);
};

void draw_header(screen_t *screen, header_t *header, char * message) {
    draw_bordered_box(
        screen,
        header->top_row,
        header->left_col,
        header->rows,
        header->cols,
        YELLOW,
        WHITE
    );
    update_header(screen, header, message);
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

int init_screen_cols() {
    return 40;
}

main_menu_t * init_main_menu() {
    main_menu_t *main_menu = (main_menu_t *) malloc(sizeof(main_menu_t));
    main_menu->num_rows = 8;
    return main_menu;
}

character_t * init_character(int inventory_size) {
    int index;
    character_t *character = (character_t *) malloc(sizeof(character_t));
    character->inventory = (int *) malloc(sizeof(int) * inventory_size);
    if (character->inventory == NULL) {
        fprintf(stderr, "*inventory is NULL!\n");
        exit(1);
    }

    for (index = 0; index < inventory_size; index += 1) {
        character->inventory[index] = 0;
    }
    for (index = 0; index < 8; index += 1) {
        character->attrs[index] = (rand() % 5) + 2;
    }
    character->attrs[4] = 1;
    character->gold = 120 + (rand() % 60);

    return character;
}

int can_be_wanderer(character_t * character) {
    return 1;
}

int can_be_cleric(character_t * character) {
    if (character->attrs[3] > 6 && character->attrs[7] > 7) {
        return 1;
    }
    return 0;
}

int can_be_mage(character_t * character) {
    if (character->attrs[3] > 8 && character->attrs[6] > 7) {
        return 1;
    }
    return 0;
}

int can_be_warrior(character_t * character) {
    if (
            character->attrs[0] > 7 && character->attrs[7] > 5 &&
            character->attrs[0] + character->attrs[1] > 10
    ) {
        return 1;
    }
    return 0;
}

int can_be_barbarian(character_t * character) {
    if (
            character->attrs[0] > 8 &&
            character->attrs[1] + character->attrs[2] > 12 &&
            character->attrs[7] < 6
    ) {
        return 1;
    }
    return 0;
}

character_class_t ** init_character_classes() {
    int index;
    character_class_t ** character_classes = malloc(
        sizeof(character_class_t *) * 5
    );
    for (index = 0; index < 5; index += 1) {
        character_classes[index] = malloc(sizeof(character_class_t));
    }
    *character_classes[0] = (character_class_t) {
        .id = 0,
        .name = "WANDERER",
        .elig_func = &can_be_wanderer
    };
    *character_classes[1] = (character_class_t) {
        .id = 1,
        .name = "CLERIC",
        .elig_func = &can_be_cleric
    };
    *character_classes[2] = (character_class_t) {
        .id = 2,
        .name = "MAGE",
        .elig_func = &can_be_mage
    };
    *character_classes[3] = (character_class_t) {
        .id = 3,
        .name = "WARRIOR",
        .elig_func = &can_be_warrior
    };
    *character_classes[4] = (character_class_t) {
        .id = 4,
        .name = "BARBARIAN",
        .elig_func = &can_be_barbarian
    };

    return character_classes;
}

int ** init_item_to_char_class() {
    int ** item_to_char_class = malloc(sizeof(int *) * 24);
    int index_a, index_b,
        values[24][5] = {
            {0, 0, 0, 0, 1},
            {0, 0, 0, 1, 1},
            {1, 0, 0, 1, 1},
            {1, 0, 0, 1, 1},
            {1, 0, 0, 1, 1},
            {0, 0, 0, 1, 1},
            {1, 1, 1, 1, 1},
            {1, 0, 0, 1, 1},
            {0, 0, 0, 1, 1},
            {0, 0, 0, 1, 1},
            {1, 0, 0, 1, 1},
            {1, 1, 1, 1, 1},
            {0, 0, 0, 1, 1},
            {1, 1, 0, 1, 1},
            {1, 1, 0, 1, 1},
            {1, 1, 1, 1, 1},
            {1, 1, 1, 0, 0},
            {0, 0, 1, 0, 0},
            {1, 1, 1, 0, 0},
            {1, 0, 1, 0, 0},
            {1, 1, 1, 0, 0},
            {1, 1, 1, 0, 0},
            {1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1}
        };
    for (index_a = 0; index_a < 24; index_a += 1) {
        item_to_char_class[index_a] = malloc(sizeof(int) * 5);
        for (index_b = 0; index_b < 5; index_b += 1) {
            item_to_char_class[index_a][index_b] = values[index_a][index_b];
        }
    }

    return item_to_char_class;
}

store_t * init_stores() {
    store_t * stores = malloc(sizeof(store_t) * 3);
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

    return stores;
}

void init_vars(int *attr_points, char ** message, const char * attr_names[8]) {
    attr_names[0] = "STRENGTH";
    attr_names[1] = "VITALITY";
    attr_names[2] = "AGILITY";
    attr_names[3] = "INTELLIGENCE";
    attr_names[4] = "EXPERIENCE";
    attr_names[5] = "LUCK";
    attr_names[6] = "AURA";
    attr_names[7] = "MORALITY";
    *attr_points = 3 + (rand() % 5);
    *message = (char *) malloc(sizeof(char) * 40);
    if (*message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(*message, "");
}

void save_character(character_t * character, int num_item_types) {
    int char_base = 65, error, index;
    FILE *save_file_handle = fopen("HERO", "w");

    if (!fputc((char) (num_item_types + char_base), save_file_handle)) {
        fprintf(
            stderr,
            "Error %i writing number of items!\n",
            ferror(save_file_handle)
        );
    }

    for (index = 0; index < 8; index += 1) {
        if (
                !fputc(
                    (char) (character->attrs[index] + char_base),
                    save_file_handle
                )
        ) {
            fprintf(
                stderr,
                "Error %i writing attr %i!\n",
                ferror(save_file_handle),
                index
            );
        }
    }
    for (index = 0; index < num_item_types; index += 1) {
        if (
                !fputc(
                    (char) (character->inventory[index] + char_base),
                    save_file_handle
                )
        ) {
            fprintf(
                stderr,
                "Error %i writing inventory %i!\n",
                ferror(save_file_handle),
                index
            );
        }
    }
    if (!fputc((char) (character->gold + char_base), save_file_handle)) {
        fprintf(stderr, "Error %i writing gold!\n", ferror(save_file_handle));
    }
    if (!fputc((char) char_base, save_file_handle)) {
        fprintf(
            stderr,
            "Error %i writing char_base!\n",
            ferror(save_file_handle)
        );
    }
    error = fputs(character->name, save_file_handle);
    if (error) {
        fprintf(stderr, "Error %i writing character name!\n", error);
    }
    error = fputs(" -", save_file_handle);
    if (error) {
        fprintf(stderr, "Error %i writing name delimiter!\n", error);
    }
    error = fputs(character->class->name, save_file_handle);
    if (error) {
        fprintf(stderr, "Error %i writing character class name!\n", error);
    }
    error = fclose(save_file_handle);
    if (error) {
        fprintf(stderr, "Error %i saving the character!\n", error);
    }
}

int main(int argc, char *argv[]) {
    int max_accepted_discount, index, store_ind, attr_points, num_item_types,
        offer, screen_cols, col, row, item_for_class;
    character_class_t ** character_classes;
    character_t *character;
    store_t * stores;
    header_t * header;
    main_menu_t *main_menu;
    int ** item_to_char_class;
    char pressed_key, * typed_string = NULL, * message = NULL;
    const char * attr_names[8];
    item_t * item;
    character_classes = init_character_classes();
    header = malloc(sizeof(header_t));
    main_menu = init_main_menu();
    character = init_character(main_menu->num_rows * 3);
    item_to_char_class = init_item_to_char_class();
    stores = init_stores();
    init_vars(&attr_points, &message, attr_names);
    screen_t *screen = NULL;
    if (init_screen(&screen) < 0) {
        return 1;
    }
    screen_cols = init_screen_cols();
    *header = (header_t) {
        .label = "POINTS",
        .points = attr_points,
        .left_col = 0,
        .top_row = 1,
        .cols = screen_cols - 2,
        .rows = 2
    };
    paper(screen->cursor, BLACK);
    draw_title_row(screen, "CHARACTER_CREATION", screen_cols);
    draw_header(screen, header, message);
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
            header->points -= 1;
            update_main(screen, main_menu);
        }
        if (
                pressed_key == '-' &&
                character->attrs[main_menu->selected_row] > 1
        ) {
            character->attrs[main_menu->selected_row] -=1;
            attr_points += 1;
            main_menu->items[main_menu->selected_row]->value -= 1;
            header->points += 1;
            update_main(screen, main_menu);
        }
        for (index = 0; index < 5; index += 1) {
            if (character_classes[index]->elig_func(character)) {
                character->class = character_classes[index];
            }
        }
        strcpy(message, character->class->name);
        update_header(screen, header, message);
        SDL_RenderPresent(screen->ren);
    } while (pressed_key != ' ');
    header->label = "GOLD COINS";
    header->points = character->gold;
    for (store_ind = 0; store_ind < 3; store_ind += 1) {
        main_menu->selected_row = 0;
        strcpy(message, "CHOOSE WELL SIRE!");
        draw_title_row(screen, stores[store_ind].name, screen_cols);
        draw_header(screen, header, message);
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
                character->class, item->id, message, item_to_char_class
            );
            max_accepted_discount = 0;
            offer = 0;
            if (pressed_key == ';') {
                offer = item->price;
                buy_item(
                    item, character, max_accepted_discount, offer,
                    item_for_class, message, header
                );
            }
            if (pressed_key == '-') {
                max_accepted_discount = rand() % 3;
                make_offer_for_item(
                    screen, item, max_accepted_discount, character,
                    message, item_to_char_class, header
                );
            }
            update_header(screen, header, message);
        } while (pressed_key != ' ');
    }
    character->name = (char *) malloc(sizeof(char) * 40);
    if (character->name == NULL) {
        fprintf(stderr, "character->name is NULL!\n");
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
        strcpy(character->name, typed_string);
        free(typed_string);
    } while (strlen(character->name) > 10);
    tab(screen->cursor, 1, 3);
    print_text(screen, "ONE MOMENT PLEASE");
    SDL_RenderPresent(screen->ren);
    tab(screen->cursor, 1, 3);
    num_item_types = main_menu->num_rows * 3;
    save_character(character, num_item_types);
    free(stores);
    free(message);
    free(character->name);
    for (index = 0; index < 8; index += 1) {
        free(main_menu->items[index]);
    }
    free(main_menu);

    for (index = 1; index < 6; index += 1) {
        free(character_classes[index]);
    }
    free(character_classes);
    for (index = 0; index < 24; index += 1) {
        free(item_to_char_class[index]);
    }
    free(item_to_char_class);

    free(character);
    destroy_screen(screen);

    return 0;
}
