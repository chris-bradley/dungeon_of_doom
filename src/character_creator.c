#include <errno.h>
#include <SDL.h>
#include "dungeon_lib.h"
#include "character_lib.h"

typedef struct {
    const char * title;
    SDL_Rect * title_rect;
} title_row_t;

typedef struct {
    const char * label;
    int points;
    char * message;
    int left_col;
    int top_row;
    int cols;
    int rows;
    enum ColourNum text_colour;
    enum ColourNum background_colour;
    enum ColourNum border_colour;
    SDL_Rect * message_rect;
    SDL_Rect * label_rect;
    SDL_Rect * points_rect;
} header_t;

typedef struct {
    const char * label;
    int value;
    SDL_Rect * label_rect;
    SDL_Rect * value_rect;
} menu_item_t;

typedef struct {
    int selected_row;
    int num_rows;
    int cols;
    menu_item_t * items[8];
    int top_row;
    enum ColourNum text_colour;
    enum ColourNum selector_colour;
    enum ColourNum background_colour;
    enum ColourNum border_colour;
    SDL_Rect * selector_rect;
} main_menu_t;

typedef struct {
    enum InventoryCode id;
    const char * name;
    int price;
    int batch_size;
} item_t;

typedef struct {
    const char * name;
    item_t items[8];
} store_t;

header_t * init_header(int screen_cols) {
    header_t * header = malloc(sizeof(header_t));
    if (header == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "header is NULL!");
        exit(1);
    }
    *header = (header_t) {
        .left_col = 0,
        .top_row = 1,
        .cols = screen_cols - 2,
        .rows = 2,
        .text_colour = BLACK,
        .background_colour = YELLOW,
        .border_colour = WHITE
    };
    header->message = (char *) malloc(sizeof(char) * 40);
    if (header->message == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "header->message is NULL!");
        exit(1);
    }
    strcpy(header->message, "");
    return header;
}

char * get_player_string(screen_t * screen, int col, int row,
                         enum ColourNum background_colour) {
    int ind = 0;
    char pressed_key, * typed_string;
    SDL_Rect * typed_rect = NULL;
    typed_string = (char *) malloc(sizeof(char) * 40);
    if (typed_string == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "typed_string is NULL!");
        exit(1);
    }
    typed_string[0] = 0;
    while (1) {
        pressed_key = inkey$();
        if (pressed_key == '\n') {
            clear_rect(screen, typed_rect, background_colour);
            free(typed_rect);
            return typed_string;
        } else if (pressed_key > '/' && pressed_key < ']' && ind < 39) {
            typed_string[ind] = pressed_key;
            ind += 1;
            typed_string[ind] = 0;
            tab(screen->cursor, col, row);
            free(typed_rect);
            typed_rect = print_text(screen, typed_string);
            SDL_RenderPresent(screen->ren);
        }
    }
}

void update_header(screen_t * screen, header_t * header) {
    paper(screen->cursor, header->background_colour);
    ink(screen->cursor, header->text_colour);

    if (header->message_rect != NULL) {
        clear_rect(screen, header->message_rect, header->background_colour);
        free(header->message_rect);
    }
    tab(screen->cursor, header->left_col + 2, header->top_row + 1);
    header->message_rect = print_text(screen, header->message);

    if (header->label_rect != NULL) {
        clear_rect(screen, header->label_rect, header->background_colour);
        free(header->label_rect);
        header->label_rect = NULL;
    }
    if (header->points_rect != NULL) {
        clear_rect(screen, header->points_rect, header->background_colour);
        free(header->points_rect);
    }

    if (header->label != NULL) {
        tab(screen->cursor, header->left_col + 2, header->top_row + 2);
        header->label_rect = print_text(screen, header->label);
    }
    tab(screen->cursor, header->left_col + 15, header->top_row + 2);
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "outstring is NULL!");
        exit(1);
    }
    sprintf(outstring, "%i ", header->points);
    header->points_rect = print_text(screen, outstring);
    free(outstring);
}

int can_class_buy_item(character_class_t * character_class, int item_num,
                       int ** item_to_char_class) {
    if (item_to_char_class[item_num][character_class->id] == 1) {
        return 1;
    }
    return 0;
}

void buy_item(item_t * item, character_t * character,
              int max_accepted_discount, int offer, header_t * header) {
    int price;
    if (character->inventory[item->id] > 0 && item->id < 22) {
        strcpy(header->message, "YOU HAVE IT SIRE");
    } else {
        price = item->price - max_accepted_discount;
        if (character->gold < price) {
            strcpy(header->message, "YOU CANNOT AFFORD");
        } else {
            if (offer >= price) {
                character->inventory[item->id] += item->batch_size;
                character->gold -= offer;
                header->points -= offer;
                strcpy(header->message, "TIS YOURS!");
            } else {
                strcpy(header->message, "OFFER REJECTED");
            }
        }
    }
}

void make_offer_for_item(screen_t * screen, item_t * item,
                         int max_accepted_discount,
                         character_t * character, header_t * header) {
    int offer;
    char * typed_string = NULL;
    strcpy(header->message, "YOUR OFFER");
    update_header(screen, header);
    SDL_RenderPresent(screen->ren);
    typed_string = get_player_string(screen, 14, 2, header->background_colour);
    offer = atoi(typed_string);
    free(typed_string);
    buy_item(item, character, max_accepted_discount, offer, header);
}

int calc_row_pos(main_menu_t * main_menu, int row_num) {
    return row_num * 2 + main_menu->top_row + 1;
}

void select_row(screen_t * screen, main_menu_t * main_menu, char pressed_key) {
    paper(screen->cursor, WHITE);
    ink(screen->cursor, RED);
    if (main_menu->selector_rect != NULL) {
        clear_rect(
            screen, main_menu->selector_rect, main_menu->background_colour
        );
        free(main_menu->selector_rect);
        main_menu->selector_rect = NULL;
    }
    if (pressed_key == 'a' && main_menu->selected_row > 0) {
        main_menu->selected_row -= 1;
    } else if (
            pressed_key == 'z' &&
            main_menu->selected_row < main_menu->num_rows - 1
    ) {
        main_menu->selected_row += 1;
    }
    tab(screen->cursor, 1, calc_row_pos(main_menu, main_menu->selected_row));
    main_menu->selector_rect = print_text(screen, ">");
}

void draw_title_row(screen_t * screen, title_row_t * title_row) {
    paper(screen->cursor, BLACK);
    ink(screen->cursor, YELLOW);
    if (title_row->title_rect != NULL) {
        clear_rect(screen, title_row->title_rect, BLACK);
        free(title_row->title_rect);
    }
    tab(screen->cursor, 0, 0);
    title_row->title_rect = print_text(screen, title_row->title);
};

title_row_t * init_title_row() {
    title_row_t * title_row = (title_row_t *) malloc(sizeof(title_row_t));
    if (title_row == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "title_row is NULL!");
        exit(1);
    }
    *title_row = (title_row_t) {
        .title = "CHARACTER CREATION"
    };
    return title_row;
}

void draw_header(screen_t * screen, header_t * header) {
    draw_bordered_box(
        screen, header->top_row, header->left_col, header->rows, header->cols,
        header->background_colour, header->border_colour
    );
    update_header(screen, header);
}

void update_main(screen_t * screen, main_menu_t * main_menu) {
    int index, row;
    menu_item_t * menu_item;
    paper(screen->cursor, main_menu->background_colour);
    ink(screen->cursor, main_menu->text_colour);
    for (index = 0; index < main_menu->num_rows; index += 1) {
        row = calc_row_pos(main_menu, index);
        menu_item = main_menu->items[index];

        if (menu_item->label_rect != NULL) {
            clear_rect(
                screen, menu_item->label_rect, main_menu->background_colour
            );
            free(menu_item->label_rect);
            menu_item->label_rect = NULL;
        }
        if (menu_item->value_rect != NULL) {
            clear_rect(
                screen, menu_item->value_rect, main_menu->background_colour
            );
            free(menu_item->value_rect);
            menu_item->value_rect = NULL;
        }

        tab(screen->cursor, 2, row);
        menu_item->label_rect = print_text(screen, menu_item->label);
        tab(screen->cursor, 16, row);
        char * outstring = (char *) malloc(sizeof(char) * 40);
        if (outstring == NULL) {
            SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "outstring is NULL!");
            exit(1);
        }
        sprintf(outstring, "%i ", menu_item->value);
        menu_item->value_rect = print_text(screen, outstring);
        free(outstring);
    }
}

void draw_main(screen_t * screen, main_menu_t * main_menu) {
    main_menu->top_row = 5;
    draw_bordered_box(
        screen, main_menu->top_row, 0, main_menu->num_rows * 2 - 1,
        main_menu->cols, main_menu->background_colour, main_menu->border_colour
    );
    update_main(screen, main_menu);
}

int init_screen_cols() {
    return 40;
}

main_menu_t * init_main_menu(int screen_cols) {
    main_menu_t * main_menu = (main_menu_t *) malloc(sizeof(main_menu_t));
    if (main_menu == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "main_menu is NULL!");
        exit(1);
    }
    *main_menu = (main_menu_t) {
        .num_rows = 8,
        .cols = screen_cols - 2,
        .text_colour = BLACK,
        .selector_colour = RED,
        .background_colour = WHITE,
        .border_colour = YELLOW,
        .selector_rect = NULL
    };
    return main_menu;
}

character_t * init_character(int inventory_size) {
    int index;
    character_t * character = (character_t *) malloc(sizeof(character_t));
    if (character == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "character is NULL!");
        exit(1);
    }
    character->inventory = (int *) malloc(sizeof(int) * inventory_size);
    if (character->inventory == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "*inventory is NULL!");
        exit(1);
    }

    for (index = 0; index < inventory_size; index += 1) {
        character->inventory[index] = 0;
    }
    character->attrs = malloc(sizeof(double) * 8);
    for (index = 0; index < 8; index += 1) {
        character->attrs[index] = (rand() % 5) + 2;
    }
    character->attrs[EXPERIENCE] = 1;
    character->gold = 120 + (rand() % 60);

    return character;
}

int can_be_wanderer(__attribute__((__unused__)) character_t * character) {
    return 1;
}

int can_be_cleric(character_t * character) {
    double * attrs = character->attrs;
    if (attrs[INTELLIGENCE] > 6 && attrs[MORALITY] > 7) {
        return 1;
    }
    return 0;
}

int can_be_mage(character_t * character) {
    double * attrs = character->attrs;
    if (attrs[INTELLIGENCE] > 8 && attrs[AURA] > 7) {
        return 1;
    }
    return 0;
}

int can_be_warrior(character_t * character) {
    double * attrs = character->attrs;
    if (
            attrs[STRENGTH] > 7 && attrs[MORALITY] > 5 &&
            attrs[STRENGTH] + attrs[VITALITY] > 10
    ) {
        return 1;
    }
    return 0;
}

int can_be_barbarian(character_t * character) {
    double * attrs = character->attrs;
    if (
            attrs[STRENGTH] > 8 && attrs[VITALITY] + attrs[AGILITY] > 12 &&
            attrs[MORALITY] < 6
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
    if (character_classes == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "character_classes is NULL!");
        exit(1);
    }
    for (index = 0; index < 5; index += 1) {
        character_classes[index] = malloc(sizeof(character_class_t));
        if (character_classes[index] == NULL) {
            SDL_LogCritical(
                SDL_LOG_CATEGORY_SYSTEM,
                "character_classes[%i] is NULL!",
                index
            );
            exit(1);
        }
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
    if (item_to_char_class == NULL) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM,
            "item_to_char_class is NULL!"
        );
        exit(1);
    }
    int index_a, index_b, values[24][5] = {
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
        if (item_to_char_class[index_a] == NULL) {
            SDL_LogCritical(
                SDL_LOG_CATEGORY_SYSTEM,
                "item_to_char_class[%i] is NULL!",
                index_a
            );
            exit(1);
        }
        for (index_b = 0; index_b < 5; index_b += 1) {
            item_to_char_class[index_a][index_b] = values[index_a][index_b];
        }
    }

    return item_to_char_class;
}

store_t * init_stores() {
    store_t * stores = malloc(sizeof(store_t) * 3);
    if (stores == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "stores is NULL!");
        exit(1);
    }
    stores[0] = (store_t) {
        .name = "ARMOURY",
        .items = {
            (item_t) {
                .id = TWO_HAND_SWORD,
                .name = "2 HAND SWORD",
                .price = 20,
                .batch_size = 5
            },
            (item_t) {
                .id = BROADSWORD,
                .name = "BROADSWORD",
                .price = 16,
                .batch_size = 4
            },
            (item_t) {
                .id = SHORTSWORD,
                .name = "SHORTSWORD",
                .price = 12,
                .batch_size = 3
            },
            (item_t) {
                .id = AXE,
                .name = "AXE",
                .price = 15,
                .batch_size = 3
            },
            (item_t) {
                .id = MACE,
                .name = "MACE",
                .price = 8,
                .batch_size = 2
            },
            (item_t) {
                .id = FLAIL,
                .name = "FLAIL",
                .price = 10,
                .batch_size = 2
            },
            (item_t) {
                .id = DAGGER,
                .name = "DAGGER",
                .price = 8,
                .batch_size = 1
            },
            (item_t) {
                .id = GAUNTLET,
                .name = "GAUNTLET",
                .price = 6,
                .batch_size = 1
            }
        }
    };
    stores[1] = (store_t) {
        .name = "ACCOUTREMENTS",
        .items = {
            (item_t) {
                .id = HEAVY_ARMOUR,
                .name = "HEAVY ARMOUR",
                .price = 18,
                .batch_size = 5
            },
            (item_t) {
                .id = CHAIN_ARMOUR,
                .name = "CHAIN ARMOUR",
                .price = 15,
                .batch_size = 4
            },
            (item_t) {
                .id = LEATHER_ARMOUR,
                .name = "LEATHER ARMOUR",
                .price = 9,
                .batch_size = 3
            },
            (item_t) {
                .id = HEAVY_ROBE,
                .name = "HEAVY ROBE",
                .price = 9,
                .batch_size = 1
            },
            (item_t) {
                .id = GOLD_HELMET,
                .name = "GOLD HELMET",
                .price = 14,
                .batch_size = 2
            },
            (item_t) {
                .id = HEADPIECE,
                .name = "HEADPIECE",
                .price = 8,
                .batch_size = 1
            },
            (item_t) {
                .id = SHIELD,
                .name = "SHIELD",
                .price = 6,
                .batch_size = 3
            },
            (item_t) {
                .id = TORCH,
                .name = "TORCH",
                .price = 6,
                .batch_size = 1
            }
        }
    };
    stores[2] = (store_t) {
        .name = "EMPORIUM",
        .items = {
            (item_t) {
                .id = NECRONOMICON,
                .name = "NECRONOMICON",
                .price = 20,
                .batch_size = 4
            },
            (item_t) {
                .id = SCROLLS,
                .name = "SCROLLS",
                .price = 15,
                .batch_size = 3
            },
            (item_t) {
                .id = RING,
                .name = "RING",
                .price = 14,
                .batch_size = 2
            },
            (item_t) {
                .id = MYSTIC_AMULET,
                .name = "MYSTIC AMULET",
                .price = 12,
                .batch_size = 2
            },
            (item_t) {
                .id = SASH,
                .name = "SASH",
                .price = 10,
                .batch_size = 3
            },
            (item_t) {
                .id = CLOAK,
                .name = "CLOAK",
                .price = 8,
                .batch_size = 1
            },
            (item_t) {
                .id = HEALING_SALVE,
                .name = "HEALING SALVE",
                .price = 6,
                .batch_size = 1
            },
            (item_t) {
                .id = POTIONS,
                .name = "POTIONS",
                .price = 6,
                .batch_size = 1
            }
        }
    };

    return stores;
}

const char ** init_attr_names() {
    const char ** attr_names = (const char **) malloc(
            sizeof(const char *) * 8
        );
    if (attr_names == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "attr_names is NULL!");
        exit(1);
    }
    attr_names[STRENGTH] = "STRENGTH";
    attr_names[VITALITY] = "VITALITY";
    attr_names[AGILITY] = "AGILITY";
    attr_names[INTELLIGENCE] = "INTELLIGENCE";
    attr_names[EXPERIENCE] = "EXPERIENCE";
    attr_names[LUCK] = "LUCK";
    attr_names[AURA] = "AURA";
    attr_names[MORALITY] = "MORALITY";
    return attr_names;
}

void save_character(character_t * character, int num_item_types) {
    int char_base = 65, error, index;
    char * character_dir = get_character_dir();
    char * character_path = malloc(sizeof(char) * (strlen(character_dir) + 6));
    sprintf(character_path, "%s%cHERO", character_dir, PATHSEP);
    free(character_dir);
    errno = 0;
    FILE * save_file_handle = fopen(character_path, "w");
    if (save_file_handle == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "save_file_handle is NULL!");
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "Error: %d (%s).", errno, strerror(errno)
        );
        exit(1);
    }
    free(character_path);

    if (!fputc((char) (num_item_types + char_base), save_file_handle)) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM, "Error %i writing number of items!",
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
            SDL_LogError(
                SDL_LOG_CATEGORY_SYSTEM, "Error %i writing attr %i!",
                ferror(save_file_handle), index
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
            SDL_LogError(
                SDL_LOG_CATEGORY_SYSTEM, "Error %i writing inventory %i!",
                ferror(save_file_handle), index
            );
        }
    }
    if (!fputc((char) (character->gold + char_base), save_file_handle)) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM, "Error %i writing gold!",
            ferror(save_file_handle)
        );
    }
    if (!fputc((char) char_base, save_file_handle)) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM, "Error %i writing char_base!",
            ferror(save_file_handle)
        );
    }
    error = fputs(character->name, save_file_handle);
    if (error < 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM, "Error %i writing character name!",
            ferror(save_file_handle)
        );
    }
    error = fputs(" -", save_file_handle);
    if (error < 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM, "Error %i writing name delimiter!",
            ferror(save_file_handle)
        );
    }
    error = fputs(character->class->name, save_file_handle);
    if (error < 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM, "Error %i writing character class name!",
            ferror(save_file_handle)
        );
    }
    error = fclose(save_file_handle);
    if (error) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM, "Error %i saving the character!", error
        );
    }
}

void set_attr_phase(screen_t * screen, character_t * character,
                    main_menu_t * main_menu, header_t * header,
                    character_class_t ** character_classes) {
    int index,
        attr_points = 3 + (rand() % 5);
    char pressed_key;
    const char ** attr_names = init_attr_names();

    main_menu->selected_row = 0;
    for (index = 0; index < 8; index += 1) {
        main_menu->items[index] = malloc(sizeof(menu_item_t));
        if (main_menu->items[index] == NULL) {
            SDL_LogCritical(
                SDL_LOG_CATEGORY_SYSTEM, "main_menu->items[%i] is NULL!", index
            );
            exit(1);
        }
        *main_menu->items[index] = (menu_item_t) {
            .label = attr_names[index],
            .value = character->attrs[index]
        };
    }
    draw_main(screen, main_menu);
    select_row(screen, main_menu, ' ');
    header->points = attr_points;
    header->label = "POINTS";
    update_header(screen, header);
    SDL_RenderPresent(screen->ren);
    do {
        pressed_key = inkey$();
        select_row(screen, main_menu, pressed_key);
        if (main_menu->selected_row != EXPERIENCE) {
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
                character->attrs[main_menu->selected_row] -= 1;
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
            strcpy(header->message, character->class->name);
            update_header(screen, header);
            SDL_RenderPresent(screen->ren);
        }
    } while (pressed_key != ' ');

    free(attr_names);
}

void purchasing_phase(screen_t * screen, character_t * character,
                      store_t * stores, main_menu_t * main_menu,
                      header_t * header, title_row_t * title_row) {
    int max_accepted_discount, index, store_ind, offer, item_for_class;
    int ** item_to_char_class = init_item_to_char_class();
    item_t * item;
    char pressed_key;
    header->label = "GOLD COINS";
    header->points = character->gold;
    for (store_ind = 0; store_ind < 3; store_ind += 1) {
        main_menu->selected_row = 0;
        strcpy(header->message, "CHOOSE WELL SIRE!");
        title_row->title = stores[store_ind].name;
        draw_title_row(screen, title_row);
        draw_header(screen, header);
        for (index = 0; index < 8; index += 1) {
            item = &stores[store_ind].items[index];
            main_menu->items[index]->label = item->name;
            main_menu->items[index]->value = item->price;
        }
        draw_main(screen, main_menu);
        select_row(screen, main_menu, ' ');
        do {
            SDL_RenderPresent(screen->ren);
            pressed_key = inkey$();
            select_row(screen, main_menu, pressed_key);
            item = &stores[store_ind].items[main_menu->selected_row];
            strcpy(header->message, "MAKE YOUR CHOICE");
            item_for_class = can_class_buy_item(
                character->class, item->id, item_to_char_class
            );
            if (item_for_class) {
                max_accepted_discount = 0;
                if (pressed_key == ';') {
                    offer = item->price;
                    buy_item(
                        item, character, max_accepted_discount, offer, header
                    );
                }
                if (pressed_key == '-') {
                    max_accepted_discount = rand() % 3;
                    make_offer_for_item(
                        screen, item, max_accepted_discount, character, header
                    );
                }
            } else {
                sprintf(header->message, "NOT FOR %s", character->class->name);
            }
            update_header(screen, header);
        } while (pressed_key != ' ');
    }
    for (index = 0; index < 24; index += 1) {
        free(item_to_char_class[index]);
    }
    free(item_to_char_class);
}

void character_naming_phase(screen_t * screen, character_t * character,
                            header_t * header) {
    int col, row;
    char * typed_string = NULL;
    character->name = (char *) malloc(sizeof(char) * 40);
    if (character->name == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "character->name is NULL!");
        exit(1);
    }
    do {
        tab(screen->cursor, 1, 2);
        free(print_text(screen, "NAME THY CHARACTER"));
        if (header->label_rect != NULL) {
            clear_rect(screen, header->label_rect, header->background_colour);
            free(header->label_rect);
            header->label_rect = NULL;
        }
        if (header->points_rect != NULL) {
            clear_rect(screen, header->points_rect, header->background_colour);
            free(header->points_rect);
            header->points_rect = NULL;
        }
        SDL_RenderPresent(screen->ren);
        tab(screen->cursor, 1, 3);

        col = 1;
        row = 3;
        typed_string = get_player_string(
            screen, col, row, header->background_colour
        );
        strcpy(character->name, typed_string);
        free(typed_string);
    } while (strlen(character->name) > 10);
}

int main(int argc, char * argv[]) {
    handle_args(argc, argv);
    int index, num_item_types,
        screen_cols = init_screen_cols();
    character_class_t ** character_classes = init_character_classes();
    store_t * stores = init_stores();
    header_t * header = init_header(screen_cols);
    main_menu_t * main_menu = init_main_menu(screen_cols);
    character_t * character = init_character(main_menu->num_rows * 3);
    screen_t * screen = init_screen();
    title_row_t * title_row = init_title_row();

    paper(screen->cursor, BLACK);
    draw_title_row(screen, title_row);
    draw_header(screen, header);
    set_attr_phase(screen, character, main_menu, header, character_classes);
    purchasing_phase(screen, character, stores, main_menu, header, title_row);
    character_naming_phase(screen, character, header);
    tab(screen->cursor, 1, 3);
    free(print_text(screen, "ONE MOMENT PLEASE"));
    SDL_RenderPresent(screen->ren);
    tab(screen->cursor, 1, 3);
    num_item_types = main_menu->num_rows * 3;
    save_character(character, num_item_types);
    free(title_row->title_rect);
    free(title_row);
    free(stores);
    free(header->message);
    free(header->message_rect);
    free(header->label_rect);
    free(header->points_rect);
    free(header);
    free(character->name);
    free(character->attrs);
    for (index = 0; index < 8; index += 1) {
        free(main_menu->items[index]->label_rect);
        free(main_menu->items[index]->value_rect);
        free(main_menu->items[index]);
    }
    free(main_menu->selector_rect);
    free(main_menu);
    for (index = 0; index < 5; index += 1) {
        free(character_classes[index]);
    }
    free(character_classes);
    free(character->inventory);
    free(character);
    destroy_screen(screen);

    return 0;
}
