#include <SDL.h>
#include <unistd.h>
#include "dungeon_lib.h"
#include "dungeon_audio.h"

enum CharCode {
    CHARACTER_BASE = 65,
    DUNGEON_BASE = 96,
    BLANK = 102,
    WALL = 103,
    VASE = 104,
    CHEST = 105,
    IDOL = 106,
    EXIT = 108,
    TRAP = 109,
    SAFE_PLACE = 110
};

enum AttrNum {
    STRENGTH = 1,
    VITALITY = 2,
    AGILITY = 3,
    INTELLIGENCE = 4,
    EXPERIENCE = 5,
    LUCK = 6,
    AURA = 7,
    MORALITY = 8
};

typedef struct {
    float coord_x;
    float coord_y;
    float next_coord_x;
    float next_coord_y;
    float speed;
    int distance_x;  // A value of 255 indicates no monster active.
    int distance_y;
    int type;
    int strength;
    int char_code;
} monster_t;

void sound_sawtooth(audio_state_t * audio_state, int sound_frequency) {
    int length = 0.3 * audio_state->audio_spec->freq;
    Uint8 * stream = sawtooth(
        sound_frequency * 16.940,
        length,
        audio_state->audio_spec
    );
    volume_filter(stream, length, 0, length, 0, 0);
    stream_queue_enqueue(*audio_state->streams, stream, length);
    SDL_PauseAudioDevice(audio_state->device, 0);
}

void sound_noise(audio_state_t * audio_state, int sound_frequency) {
    int length = 0.3 * audio_state->audio_spec->freq;
    Uint8 * stream = noise(
        sound_frequency * 16.940,
        length,
        audio_state->audio_spec
    );
    volume_filter(stream, length, 0, length, 0, 0);
    stream_queue_enqueue(*(audio_state->streams + 1), stream, length);
    SDL_PauseAudioDevice(audio_state->device, 0);
}

void get_keyboard_input(screen_t *screen, char *pressed_key, char *message,
                        int screen_cols) {
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    tab(screen->cursor, 0, 5);
    free(print_text(screen, message));
    SDL_RenderPresent(screen->ren);
    *pressed_key = inkey$();
    tab(screen->cursor, 0, 5);
    print_left$_b$(screen, screen_cols);
}

void draw_message_wo_colour_change(screen_t *screen, char *message,
                                   int screen_cols) {
    tab(screen->cursor, 0, 5);
    free(print_text(screen, message));
    SDL_RenderPresent(screen->ren);
    sleep(0.34);
    tab(screen->cursor, 0, 5);
    print_left$_b$(screen, screen_cols);
}

void draw_message(screen_t *screen, char *message, int screen_cols) {
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    draw_message_wo_colour_change(screen, message, screen_cols);
}

void draw_character_and_stats(screen_t *screen, double *attrs,
                              char *char_code_hero, int character_facing,
                              int character_coord_x, int character_coord_y) {
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    render_bitmap(
        screen,
        character_coord_x,
        character_coord_y + 5,
        char_code_hero[character_facing] - DUNGEON_BASE,
        WHITE,
        RED
    );
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    tab(screen->cursor, 16, 8);
    sprintf(outstring, "%i ", (int) attrs[STRENGTH]);
    free(print_text(screen, outstring));

    tab(screen->cursor, 16, 11);
    sprintf(outstring, "%i ", (int) attrs[VITALITY]);
    free(print_text(screen, outstring));
    tab(screen->cursor, 16, 14);
    sprintf(outstring, "%i ", (int) attrs[AURA]);
    free(print_text(screen, outstring));
    tab(screen->cursor, 16, 17);
    outstring[0] = "NESW."[character_facing - 1];
    outstring[1] = 0;
    free(print_text(screen, outstring));
    tab(screen->cursor, 16, 20);
    sprintf(outstring, "%i ", (int) attrs[EXPERIENCE]);
    free(print_text(screen, outstring));
    free(outstring);
    SDL_RenderPresent(screen->ren);
}

void render_coord_and_check_for_monster(screen_t *screen,
                                        monster_t *cur_monster,
                                        int **dungeon_contents, int coord_x,
                                        int coord_y) {
    enum CharCode item_at_coord;
    paper(screen->cursor, RED);
    ink(screen->cursor, YELLOW);
    item_at_coord = dungeon_contents[coord_x][coord_y];
    tab(screen->cursor, coord_x, coord_y + 5);
    render_bitmap(
        screen,
        screen->cursor->curs_x,
        screen->cursor->curs_y,
        item_at_coord - DUNGEON_BASE,
        YELLOW,
        RED
    );
    if (
            abs(cur_monster->distance_x) < 4 ||
            item_at_coord <= SAFE_PLACE
    ) {
        return;
    }
    cur_monster->char_code = item_at_coord;
    cur_monster->type = cur_monster->char_code - VASE;
    cur_monster->speed = cur_monster->type / 16.0;
    cur_monster->strength = cur_monster->type * 6;
    cur_monster->distance_x = 3;
    cur_monster->coord_x = (float) coord_x;
    cur_monster->coord_y = (float) coord_y;
}

int sign(int x) {
    if (x > 0) {
        return 1;
    }
    if (x < 0) {
        return -1;
    }
    return 0;
}

void monster_breaks_items(screen_t *screen, audio_state_t * audio_state,
                          int item_num, int sound_frequency,
                          int *monster_broke_item, int inventory[25],
                          const char **strings, int screen_cols,
                          const char **item_names) {
    char * message;
    inventory[item_num] = 0;
    message = (char *) malloc(
        sizeof(char) * (strlen(strings[8]) + strlen(item_names[item_num]) + 2)
    );
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    sprintf(message, "%s %s", strings[8], item_names[item_num]);
    draw_message(screen, message, screen_cols);
    free(message);
    *monster_broke_item = 0;
    sound_noise(audio_state, sound_frequency);
    sound_frequency = item_num;
    sound_sawtooth(audio_state, sound_frequency);
}

void monsters_turn(screen_t *screen, audio_state_t * audio_state,
                   double *attrs, monster_t *cur_monster,
                   int character_coord_x, int character_coord_y,
                   int inventory[25], int **dungeon_contents,
                   int item_at_character_coord, const char **strings,
                   int screen_cols, const char **item_names) {
    int damage, item_num, sound_frequency, monster_broke_item,
        item_at_monster_next_coord, direction_to_monster_x,
        direction_to_monster_y, coord_x, coord_y;
    char * message;
    cur_monster->distance_x = (int) cur_monster->coord_x - character_coord_x;
    direction_to_monster_x = sign(cur_monster->distance_x);
    cur_monster->distance_y = (int) cur_monster->coord_y - character_coord_y;
    direction_to_monster_y = sign(cur_monster->distance_y);
    cur_monster->next_coord_x =
        cur_monster->coord_x - (
            cur_monster->speed * (float) direction_to_monster_x
        );
    cur_monster->next_coord_y =
        cur_monster->coord_y - (
            cur_monster->speed * (float) direction_to_monster_y
        );
    item_at_monster_next_coord =
        dungeon_contents[
            (int) cur_monster->next_coord_x
        ][(int) cur_monster->next_coord_y];
    if (
            item_at_monster_next_coord > BLANK &&
            item_at_monster_next_coord != cur_monster->char_code
    ) {
        cur_monster->next_coord_y = cur_monster->coord_y;
        cur_monster->next_coord_x = cur_monster->coord_x;
    }
    dungeon_contents[
        (int) cur_monster->coord_x
    ][(int) cur_monster->coord_y] = BLANK;
    coord_x = cur_monster->coord_x;
    coord_y = cur_monster->coord_y;
    render_coord_and_check_for_monster(
        screen, cur_monster, dungeon_contents, coord_x, coord_y
    );
    dungeon_contents[
        (int) cur_monster->next_coord_x
    ][(int) cur_monster->next_coord_y] = cur_monster->char_code;
    coord_x = (int) cur_monster->next_coord_x;
    coord_y = (int) cur_monster->next_coord_y;
    render_coord_and_check_for_monster(
        screen, cur_monster, dungeon_contents, coord_x, coord_y
    );
    cur_monster->coord_x = cur_monster->next_coord_x;
    cur_monster->coord_y = cur_monster->next_coord_y;
    damage = 0;
    if (
            abs(cur_monster->distance_x) <= 1 &&
            abs(cur_monster->distance_y) <= 1 &&
            item_at_character_coord != SAFE_PLACE
    ) {
        damage = cur_monster->type * 0.5;
        sound_frequency = damage;
        sound_sawtooth(audio_state, sound_frequency);
    } else {
        sound_frequency = 0;
    }
    if (damage * 12 < attrs[LUCK] + attrs[AGILITY]) {
        return;
    }
    message = (char *) malloc(sizeof(char) * (strlen(strings[5]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, strings[5]);
    draw_message(screen, message, screen_cols);
    free(message);
    sound_noise(audio_state, sound_frequency);
    damage /= (
        3 + inventory[9] + inventory[10] + inventory[11] + inventory[12] +
        inventory[13] + inventory[14]
    );
    attrs[STRENGTH] -= damage;
    attrs[VITALITY] -= damage / 101;
    item_num = 1;
    monster_broke_item = rand() % cur_monster->type;
    sound_frequency = cur_monster->char_code;
    sound_sawtooth(audio_state, sound_frequency);
    sound_noise(audio_state, sound_frequency);
    int done = 0;
    do {
        if (monster_broke_item == 1 && inventory[item_num] > 0) {
            monster_breaks_items(
                screen, audio_state, item_num, sound_frequency,
                &monster_broke_item, inventory, strings, screen_cols,
                item_names
            );
        }
        if (item_num < 11) {
            item_num += 1;
            done = 1;
        }
    } while (!done);
}

void character_dies(screen_t *screen, audio_state_t * audio_state,
                    double *attrs, char *char_code_hero,
                    monster_t *cur_monster,
                    int *character_facing, int character_coord_x,
                    int character_coord_y, int **dungeon_contents,
                    int screen_cols, int coord_x, int coord_y) {
    char * message;
    int sound_frequency;
    *character_facing = 5;
    attrs[STRENGTH] = 0;
    message = (char *) malloc(sizeof(char));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, "");
    draw_message_wo_colour_change(screen, message, screen_cols);
    free(message);
    tab(screen->cursor, 1, 5);
    free(print_text(screen, "THOU HAST EXPIRED!"));
    for (sound_frequency = 150; sound_frequency >= 1; sound_frequency -= 4) {
        sound_sawtooth(audio_state, sound_frequency);
        sound_noise(audio_state, sound_frequency);
        render_coord_and_check_for_monster(
            screen, cur_monster, dungeon_contents, coord_x, coord_y
        );
        draw_character_and_stats(
            screen, attrs, char_code_hero, *character_facing,
            character_coord_x, character_coord_y
        );
    }
}

void monster_dies(screen_t *screen, audio_state_t * audio_state,
                  double *attrs, monster_t *cur_monster,
                  int **dungeon_contents, const char **strings,
                  int screen_cols, int coord_x, int coord_y) {
    char * message;
    int sound_frequency;
    cur_monster->distance_x = 255;
    cur_monster->strength = 0;
    dungeon_contents[
        (int) cur_monster->next_coord_x
    ][(int) cur_monster->next_coord_y] = BLANK;
    attrs[EXPERIENCE] += 0.1;
    message = (char *) malloc(sizeof(char) * (strlen(strings[6]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, strings[6]);
    draw_message(screen, message, screen_cols);
    free(message);
    for (sound_frequency = 200; sound_frequency >= 150; sound_frequency -= 8) {
        sound_sawtooth(audio_state, sound_frequency);
        sound_noise(audio_state, sound_frequency);
    }
    render_coord_and_check_for_monster(
        screen, cur_monster, dungeon_contents, coord_x, coord_y
    );
}


void attack_monster(screen_t *screen, audio_state_t * audio_state,
                    double *attrs, monster_t *cur_monster, int inventory[25],
                    int **dungeon_contents, const char **strings,
                    int screen_cols, int coord_x, int coord_y) {
    int damage, t$_ind = rand() % 3 + 1;
    char * message;
    message = (char *) malloc(sizeof(char) * (strlen(strings[t$_ind]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, strings[t$_ind]);
    /*
    The original code did not define sound_frequency before calling the
    subroutine at line 360 here. Since the value of sound_frequency is not
    easily predicted, we just use 100.
    */
    sound_noise(audio_state, 100);
    damage =
        attrs[STRENGTH] + inventory[1] + inventory[2] + inventory[3] +
        inventory[4] + inventory[5] + inventory[6] + inventory[7] +
        inventory[8] + (rand() * attrs[LUCK] / RAND_MAX);
    if (attrs[AGILITY] + attrs[LUCK] < rand() % cur_monster->type + 2) {
        free(message);
        message = (char *) malloc(sizeof(char) * (strlen(strings[4]) + 1));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, strings[4]);
        damage = 0;
    }
    cur_monster->strength -= damage;
    draw_message(screen, message, screen_cols);
    free(message);
    attrs[STRENGTH] -= damage / 100;
    attrs[EXPERIENCE] += 0.05;
    if (cur_monster->strength < 1) {
        monster_dies(
            screen, audio_state, attrs, cur_monster, dungeon_contents, strings,
            screen_cols, coord_x, coord_y
        );
    }
}

void cast_superzap(screen_t *screen, audio_state_t * audio_state,
                   double *attrs, monster_t *cur_monster,
                   int **dungeon_contents, const char **strings,
                   int screen_cols) {
    int sound_frequency, coord_x, coord_y;
    for (sound_frequency = 1; sound_frequency <= 12; sound_frequency += 1) {
        sound_sawtooth(audio_state, sound_frequency);
        sound_noise(audio_state, sound_frequency);
    }
    if (cur_monster->distance_x < 255) {
        coord_x = cur_monster->next_coord_x;
        coord_y = cur_monster->next_coord_y;
        monster_dies(
            screen, audio_state, attrs, cur_monster, dungeon_contents, strings,
            screen_cols, coord_x, coord_y
        );
    }
}

void cast_sanctuary(audio_state_t * audio_state, int character_coord_x,
                    int character_coord_y, int **dungeon_contents,
                    int item_at_character_coord) {
    int sound_frequency;
    if (item_at_character_coord == BLANK) {
        dungeon_contents[character_coord_x][character_coord_y] = SAFE_PLACE;
    }
    sound_frequency = 100;
    sound_sawtooth(audio_state, sound_frequency);
    sound_frequency = 200;
    sound_sawtooth(audio_state, sound_frequency);
}

void cast_teleport(screen_t *screen, audio_state_t * audio_state,
                   double *attrs, char *char_code_hero, int character_facing,
                   int *character_coord_x, int *character_coord_y) {
    int sound_frequency;
    *character_coord_x = rand() % 13;
    *character_coord_y = rand() % 13;
    for (sound_frequency = 0; sound_frequency <= 255; sound_frequency += 8) {
        sound_noise(audio_state, sound_frequency);
        sound_sawtooth(audio_state, sound_frequency);
    }
    draw_character_and_stats(
        screen, attrs, char_code_hero, character_facing, *character_coord_x,
        *character_coord_y
    );
}

void cast_powersurge(double *attrs, int *spells_remaining, int spell_number) {
    attrs[VITALITY] += rand() % spells_remaining[spell_number];
    attrs[STRENGTH] += rand() % spells_remaining[spell_number];
    attrs[AURA] -= 1;
}

void cast_metamorphosis(screen_t *screen, audio_state_t * audio_state,
                        monster_t *cur_monster, int character_coord_x,
                        int character_coord_y, int **dungeon_contents,
                        int item_at_character_coord, int coord_x,
                        int coord_y) {
    int sound_frequency;
    for (sound_frequency = 1; sound_frequency <= 30; sound_frequency += 1) {
        dungeon_contents[character_coord_x][character_coord_y] =
            rand() % 8 + 1 + BLANK;
        sound_sawtooth(audio_state, sound_frequency);
        render_coord_and_check_for_monster(
            screen, cur_monster, dungeon_contents, coord_x, coord_y
        );
    }
    if (item_at_character_coord <= SAFE_PLACE) {
        cur_monster->distance_x = 255;
        cur_monster->strength = 0;
    }
    for (sound_frequency = 1; sound_frequency <= 20; sound_frequency += 4) {
        sound_sawtooth(audio_state, sound_frequency);
    }
}

void cast_healing(double *attrs, double initial_strength,
                  double initial_vitality) {
    attrs[VITALITY] = initial_vitality;
    attrs[STRENGTH] = initial_strength;
    attrs[AURA] -= 1;
}

void cast_spell(screen_t *screen, audio_state_t * audio_state,
                double *attrs, char *char_code_hero, monster_t *cur_monster,
                int *spells_remaining, int character_facing,
                int *character_coord_x, int *character_coord_y,
                int inventory[25], int **dungeon_contents,
                int item_at_character_coord, double initial_strength,
                double initial_vitality, const char **strings,
                int screen_cols) {
    int row_num, spell_number, coord_x, coord_y;
    char pressed_key, * message;
    draw_character_and_stats(
        screen, attrs, char_code_hero, character_facing, *character_coord_x,
        *character_coord_y
    );
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    tab(screen->cursor, 0, 1);
    free(print_text(screen, "YOU MAY USE MAGICKS"));
    if (inventory[17] > 0) {
        tab(screen->cursor, 0, 2);
        free(print_text(screen, "FROM NECRONOMICON"));
    }
    if (inventory[18] > 0) {
        tab(screen->cursor, 0, 3);
        free(print_text(screen, "FROM THE SCROLLS"));
    }
    tab(screen->cursor, 0, 4);
    free(print_text(screen, "CONSULT THE LORE"));
    do {
        message = (char *) malloc(sizeof(char) * 18);
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, "USE SPELL NUMBER?");
        get_keyboard_input(screen, &pressed_key, message, screen_cols);
        free(message);
        char * outstring = (char *) malloc(sizeof(char) * 2);
        sprintf(outstring, "%c", pressed_key);
        spell_number = atoi(outstring);
        free(outstring);
    } while (
            spell_number == 0 ||
            (inventory[17] == 0 && spell_number < 5) ||
            (inventory[19] == 0 && spell_number > 3) ||
            spell_number > 6
    );

    spells_remaining[spell_number] -= 1;
    coord_x = *character_coord_x;
    coord_y = *character_coord_y;
    if (spells_remaining[spell_number] < 0) {
        message = (char *) malloc(sizeof(char) * (strlen(strings[9]) + 1));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, strings[9]);
        spell_number = 7;
    } else {
        message = (char *) malloc(sizeof(char));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, "");
    }
    for (row_num = 1; row_num <= 5; row_num += 1) {
        tab(screen->cursor, 0, row_num);
        print_left$_b$(screen, screen_cols);
        newline(screen->cursor);
    }
    render_coord_and_check_for_monster(
        screen, cur_monster, dungeon_contents, coord_x, coord_y
    );
    switch (spell_number) {
        case 1:
            cast_superzap(
                screen, audio_state, attrs, cur_monster, dungeon_contents,
                strings, screen_cols
            );
            break;
        case 2:
            cast_sanctuary(
                audio_state, *character_coord_x, *character_coord_y,
                dungeon_contents, item_at_character_coord
            );
            break;
        case 3:
            cast_teleport(
                screen, audio_state, attrs, char_code_hero, character_facing,
                character_coord_x, character_coord_y
            );
            break;
        case 4:
            cast_powersurge(attrs, spells_remaining, spell_number);
            break;
        case 5:
            cast_metamorphosis(
                screen, audio_state, cur_monster, *character_coord_x,
                *character_coord_y, dungeon_contents, item_at_character_coord,
                coord_x, coord_y
            );
            break;
        case 6:
            cast_healing(attrs, initial_strength, initial_vitality);
            break;
        case 7:
            break;
    }
    attrs[EXPERIENCE] += 0.2;
    draw_message(screen, message, screen_cols);
    free(message);
}

void game_won(screen_t *screen, audio_state_t * audio_state, double *attrs,
              char *char_code_hero, int *finished, int gold,
              monster_t *cur_monster, int *character_facing,
              int character_coord_x, int character_coord_y, int *song_notes,
              int treasure) {
    int index, sound_frequency, direction;
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, RED);
    tab(screen->cursor, 0, 1);
    free(print_text(screen, " THY QUEST IS OVER! "));
    for (index = 1; index <= 18; index += 1) {
        sound_frequency = song_notes[index];
        sound_sawtooth(audio_state, sound_frequency);

        for (direction = 1; direction <=4; direction += 1) {
            *character_facing = direction;
            draw_character_and_stats(
                screen, attrs, char_code_hero, *character_facing,
                character_coord_x, character_coord_y
            );
        }
    }
    cur_monster->strength = 0;
    tab(screen->cursor, 1, 2);
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    sprintf(
        outstring,
        "THY SCORE=%i",
        (int) (
            (treasure * 10) + (gold * attrs[EXPERIENCE]) + attrs[STRENGTH] +
            attrs[VITALITY] + attrs[AGILITY]
        )
    );
    free(print_text(screen, outstring));
    free(outstring);
    *finished = 1;
}

void get_item(screen_t *screen, audio_state_t * audio_state,
              int **vertices, double *attrs, char *char_code_hero,
              int *finished, int gold, monster_t *cur_monster,
              int *character_facing, int character_coord_x,
              int character_coord_y, int inventory[25], int **dungeon_contents,
              int *song_notes, int *treasure) {
    int sound_frequency, item_to_get, item_to_get_coord_x, item_to_get_coord_y,
        coord_x, coord_y;
    item_to_get_coord_x = character_coord_x + vertices[*character_facing][1];
    item_to_get_coord_y = character_coord_y + vertices[*character_facing][2];
    if (item_to_get_coord_x < 0) {
        item_to_get_coord_x = 0;
    }
    if (item_to_get_coord_y < 0) {
        item_to_get_coord_y = 0;
    }
    if (item_to_get_coord_x > 15) {
        item_to_get_coord_x = 15;
    }
    if (item_to_get_coord_y > 15) {
        item_to_get_coord_y = 15;
    }
    item_to_get = dungeon_contents[item_to_get_coord_x][item_to_get_coord_y];
    if (item_to_get > WALL && item_to_get < IDOL) {
        dungeon_contents[item_to_get_coord_x][item_to_get_coord_y] = BLANK;
    }
    if (item_to_get == VASE) {
        inventory[23] += 1;
        inventory[24] += 1;
    }
    if (item_to_get == CHEST) {
        treasure += 1;
    }
    if (item_to_get == IDOL) {
        game_won(
            screen, audio_state, attrs, char_code_hero, finished, gold,
            cur_monster, character_facing, character_coord_x,
            character_coord_y, song_notes, *treasure
        );
    }
    coord_x = item_to_get_coord_x;
    coord_y = item_to_get_coord_y;
    render_coord_and_check_for_monster(
        screen, cur_monster, dungeon_contents, coord_x, coord_y
    );
    if (item_to_get > WALL && item_to_get < IDOL) {
        sound_frequency = item_to_get;
        sound_sawtooth(audio_state, sound_frequency);
        sound_frequency = item_to_get + 5;
        sound_sawtooth(audio_state, sound_frequency);
    }
}

void drink_potion(double *attrs, int inventory[25], double initial_strength,
                  double initial_vitality) {
    if (inventory[24] > 0 && attrs[STRENGTH] < initial_strength) {
        attrs[STRENGTH] = initial_strength;
        inventory[24] -= 1;
    }
    if (inventory[23] > 0 && attrs[VITALITY] < initial_vitality) {
        attrs[VITALITY] = initial_vitality;
        inventory[23] -= 1;
    }
}

void light_torch(screen_t *screen, int *torches, monster_t *cur_monster,
                 int character_coord_x, int character_coord_y,
                 int **dungeon_contents, const char **strings,
                 int screen_cols) {
    int coord_x, coord_y;
    char * message;
    if (*torches == 0) {
        message = (char *) malloc(sizeof(char) * (strlen(strings[7]) + 1));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, strings[7]);
        draw_message(screen, message, screen_cols);
        free(message);
        return;
    }
    for (
            coord_y = character_coord_y - 3;
            coord_y <= character_coord_y + 3;
            coord_y += 1
    ) {
        for (
                coord_x = character_coord_x - 3;
                coord_x <= character_coord_x + 3;
                coord_x += 1
        ) {
            if (coord_x > 0 && coord_x < 16 && coord_y > 0 && coord_y < 16) {
                render_coord_and_check_for_monster(
                    screen, cur_monster, dungeon_contents, coord_x, coord_y
                );
            }
        }
    }
    *torches -= 1;
}

void show_level_too_deep_messages(screen_t *screen, double *attrs) {
    newline(screen->cursor);
    free(print_text(screen, "LEVEL TOO DEEP"));
    newline(screen->cursor);
    free(print_text(screen, "REWIND TAPE"));
    newline(screen->cursor);
    free(print_text(screen, "TO POSITION"));
    newline(screen->cursor);
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    sprintf(outstring, "FOR LEVEL %d", (int) attrs[EXPERIENCE]);
    free(print_text(screen, outstring));
    free(outstring);
}


void draw_interface(screen_t *screen, char *character_name, int screen_cols) {
    int index;
    paper(screen->cursor, RED);
    clear_screen(screen);
    tab(screen->cursor, 1, 1);
    paper(screen->cursor, WHITE);
    ink(screen->cursor, BLACK);
    free(print_text(screen, character_name));
    print_left$_b$(screen, screen_cols - strlen(character_name));
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, WHITE);
    for (index = 1; index <= 5; index += 1) {
        print_left$_b$(screen, screen_cols);
        newline(screen->cursor);
    }
    paper(screen->cursor, BLACK);
    ink(screen->cursor, RED);
    for (index = 1; index <= 15; index += 1) {
        tab(screen->cursor, 1, 5 + index);
        print_left$_b$(screen, 15);
    }
    paper(screen->cursor, RED);
    ink(screen->cursor, WHITE);
    tab(screen->cursor, 16, 7);
    free(print_text(screen, "STR"));
    tab(screen->cursor, 16, 10);
    free(print_text(screen, "VIT"));
    tab(screen->cursor, 16, 13);
    free(print_text(screen, "AUR"));
    tab(screen->cursor, 16, 16);
    free(print_text(screen, "FACE"));
    tab(screen->cursor, 16, 19);
    free(print_text(screen, "EXP"));
}

void load_level(screen_t *screen, int skip_first_exp_check,
                char *character_name, monster_t *cur_monster, double *attrs,
                int *dungeon_level, int *character_coord_x,
                int *character_coord_y, int *character_prev_coord_x,
                int *character_prev_coord_y, int **dungeon_contents,
                double initial_experience, const char **strings,
                int screen_cols) {
    int correct_level_loaded, index, entrance_coord_x, entrance_coord_y,
        coord_x, coord_y;
    char pressed_key, * message;
    do {

        if (
                !skip_first_exp_check &&
                attrs[EXPERIENCE] < initial_experience + 1
        ) {
            message =
                (char *) malloc(sizeof(char) * (strlen(strings[11]) + 1));
            if (message == NULL) {
                fprintf(stderr, "message is NULL!\n");
                exit(1);
            }
            strcpy(message, strings[11]);
            *character_coord_x = *character_prev_coord_x;
            *character_coord_y = *character_prev_coord_y;
            draw_message(screen, message, screen_cols);
            free(message);
            return;
        }
        skip_first_exp_check = 0;
        clear_screen(screen);
        tab(screen->cursor, 0, 3);
        free(print_text(screen, "PREPARE DUNGEON TAPE"));
        message = (char *) malloc(sizeof(char) * (strlen(strings[10]) + 1));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, strings[10]);
        get_keyboard_input(screen, &pressed_key, message, screen_cols);
        free(message);
        size_t filesize;
        FILE *file_handle = fopen("LEVEL", "r");
        fseek(file_handle, 0, SEEK_END);
        // TODO: Check for fseek errors.
        filesize = ftell(file_handle);
        fseek(file_handle, 0, SEEK_SET);
        char * file_contents = (char *) malloc(sizeof(char) * (filesize + 1));
        if (file_contents == NULL) {
            fprintf(stderr, "file_contents is NULL!\n");
            exit(1);
        }
        fread(file_contents, 1, filesize, file_handle);
        fclose(file_handle);
        index = 1;
        for (coord_y = 1; coord_y <= 15; coord_y += 1) {
            for (coord_x = 1; coord_x <= 15; coord_x += 1) {
                dungeon_contents[coord_x][coord_y] =
                    (int) file_contents[index - 1];
                index += 1;
            }
        }
        entrance_coord_x = (int) file_contents[index - 1] - DUNGEON_BASE;
        entrance_coord_y = (int) file_contents[index] - DUNGEON_BASE;
        *dungeon_level = (int) file_contents[index + 1] - DUNGEON_BASE;
        if (*dungeon_level > attrs[EXPERIENCE]) {
            show_level_too_deep_messages(screen, attrs);
            correct_level_loaded = 1;
        } else {
            correct_level_loaded = 0;
        }
        free(file_contents);
    } while (correct_level_loaded);
    draw_interface(screen, character_name, screen_cols);
    *character_coord_x = entrance_coord_x;
    *character_coord_y = entrance_coord_y;
    *character_prev_coord_x = *character_coord_x;
    *character_prev_coord_y = *character_coord_y;
    cur_monster->distance_x = 255;
}

void load_level_with_first_exp_check(screen_t *screen, char *character_name,
                                     monster_t *cur_monster, double *attrs,
                                     int *dungeon_level,
                                     int *character_coord_x,
                                     int *character_coord_y,
                                     int *character_prev_coord_x,
                                     int *character_prev_coord_y,
                                     int **dungeon_contents,
                                     double initial_experience,
                                     const char **strings, int screen_cols) {
    load_level(
        screen, 0, character_name, cur_monster, attrs, dungeon_level,
        character_coord_x, character_coord_y, character_prev_coord_x,
        character_prev_coord_y, dungeon_contents, initial_experience, strings,
        screen_cols
    );
}

void load_level_wo_first_exp_check(screen_t *screen, char *character_name,
                                   monster_t *cur_monster, double *attrs,
                                   int *dungeon_level,
                                   int *character_coord_x,
                                   int *character_coord_y,
                                   int *character_prev_coord_x,
                                   int *character_prev_coord_y,
                                   int **dungeon_contents,
                                   double initial_experience,
                                   const char **strings, int screen_cols) {
    load_level(
        screen, 1, character_name, cur_monster, attrs, dungeon_level,
        character_coord_x, character_coord_y, character_prev_coord_x,
        character_prev_coord_y, dungeon_contents, initial_experience, strings,
        screen_cols
    );
}

void load_character(screen_t *screen, char **character_name, double *attrs,
                    int *gold, int *torches, int *spells_remaining,
                    int inventory[25], int *num_item_types,
                    double *initial_strength, double *initial_vitality,
                    double *initial_experience, const char **strings,
                    int *treasure, int screen_cols) {
    char pressed_key, * message;
    int index, subindex, file_index;
    clear_screen(screen);
    tab(screen->cursor, 0, 3);
    free(print_text(screen, "PREPARE HERO TAPE"));
    message = (char *) malloc(sizeof(char) * (strlen(strings[10]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, strings[10]);
    get_keyboard_input(screen, &pressed_key, message, screen_cols);
    free(message);
    FILE *file_handle = fopen("HERO", "r");
    size_t filesize;
    fseek(file_handle, 0, SEEK_END);
    // TODO: Check for fseek errors.
    filesize = ftell(file_handle);
    fseek(file_handle, 0, SEEK_SET);
    char * file_contents = (char *) malloc(sizeof(char) * (filesize + 1));
    if (file_contents == NULL) {
        fprintf(stderr, "file_contents is NULL!\n");
        exit(1);
    }
    fread(file_contents, 1, filesize, file_handle);
    fclose(file_handle);
    file_contents[filesize] = 0;
    file_index = 2;
    *num_item_types = (int) file_contents[0] - CHARACTER_BASE;
    for (index = 1; index <= 8; index += 1) {
        attrs[index] =
            (int) file_contents[file_index - 1] - CHARACTER_BASE;
        file_index += 1;
    }
    for (index = 1; index <= *num_item_types; index += 1) {
        inventory[index] =
            (int) file_contents[file_index - 1] - CHARACTER_BASE;
        file_index += 1;
    }
    *gold = (int) file_contents[file_index - 1] - CHARACTER_BASE;
    *treasure = (int) file_contents[file_index] - CHARACTER_BASE;
    *character_name = (char *) malloc(sizeof(char) * (file_index + 2));
    if (*character_name == NULL) {
        fprintf(stderr, "character_name is NULL!\n");
        exit(1);
    }
    strcpy(*character_name, file_contents + file_index + 1);
    *initial_strength = attrs[STRENGTH];
    *initial_vitality = attrs[VITALITY];
    *initial_experience = attrs[EXPERIENCE];
    for (index = 1; index <= 2; index += 1) {
        for (subindex = 1; subindex <= 3; subindex += 1) {
            spells_remaining[(index - 1) * 3 + subindex] =
                inventory[16 + index] * attrs[AURA];
        }
    }
    if (inventory[16] == 1) {
        *torches = 20;
    }
    free(file_contents);
}

void save_game(screen_t *screen, char *character_name, double *attrs,
               int *finished, int gold, int dungeon_level,
               int character_coord_x, int character_coord_y, int inventory[25],
               int num_item_types, int **dungeon_contents, int treasure,
               int screen_cols) {
    int index, coord_x, coord_y;
    char pressed_key, * message;
    message = (char *) malloc(sizeof(char) * 18);
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, "ONE MOMENT PLEASE");
    draw_message(screen, message, screen_cols);
    free(message);
    char * character_file_contents = (char *) malloc(
        sizeof(char) * (12 + num_item_types + strlen(character_name))
    );
    if (character_file_contents == NULL) {
        fprintf(stderr, "character_file_contents is NULL!\n");
        exit(1);
    }
    char * dungeon_file_contents = (char *) malloc(sizeof(char) * 229);
    if (dungeon_file_contents == NULL) {
        fprintf(stderr, "strings is NULL!\n");
        exit(1);
    }
    int s_index = 0;
    int t_index = 0;
    for (coord_y = 1; coord_y <= 15; coord_y += 1) {
        for (coord_x = 1; coord_x <= 15; coord_x += 1) {
            dungeon_file_contents[t_index] =
                (char) dungeon_contents[coord_x][coord_y];
            t_index += 1;
        }
    }
    dungeon_file_contents[t_index] =
        (char) (DUNGEON_BASE + character_coord_x);
    t_index += 1;
    dungeon_file_contents[t_index] =
        (char) (DUNGEON_BASE + character_coord_y);
    t_index += 1;
    dungeon_file_contents[t_index] =
        (char) (DUNGEON_BASE + dungeon_level);
    t_index += 1;
    dungeon_file_contents[t_index] = 0;
    character_file_contents[s_index] =
        (char) (CHARACTER_BASE + num_item_types);
    s_index += 1;
    for (index = 1; index <= 8; index += 1) {
        character_file_contents[s_index] =
            (char) (attrs[index] + CHARACTER_BASE);
        s_index += 1;
    }
    for (index = 1; index <= num_item_types; index += 1) {
        character_file_contents[s_index] = (char)
            (inventory[index] + CHARACTER_BASE);
        s_index += 1;
    }
    character_file_contents[s_index] = (char) (gold + CHARACTER_BASE);
    s_index += 1;
    character_file_contents[s_index] = (char) (treasure + CHARACTER_BASE);
    s_index += 1;
    strcpy(character_file_contents + s_index, character_name);
    s_index += strlen(character_name);
    character_file_contents[s_index] = 0;
    message = (char *) malloc(sizeof(char) * 16);
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, "ANY KEY TO SAVE");
    get_keyboard_input(screen, &pressed_key, message, screen_cols);
    free(message);
    FILE *file_handle = fopen("HERO", "w");
    int error = fputs(character_file_contents, file_handle);
    if (error) {
        fprintf(stderr, "Error %i writing the character!", error);
    }
    error = fclose(file_handle);
    if (error) {
        fprintf(stderr, "Error %i closing the character!", error);
    }
    file_handle = fopen("LEVEL", "w");
    error = fputs(dungeon_file_contents, file_handle);
    if (error) {
        fprintf(stderr, "Error %i writing the level!", error);
    }
    error = fclose(file_handle);
    if (error) {
        fprintf(stderr, "Error %i closing the level!", error);
    }
    *finished = 1;
}

void init_platform_vars(audio_state_t ** audio_state) {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL Init Failure!: %s\n", SDL_GetError());
        exit(1);
    }
    *audio_state = init_audio_state(2);
}

void init_vars(int ***vertices, double **attrs, char **char_code_hero,
               int *finished, int *torches, int **spells_remaining,
               int *character_facing, int *character_coord_x,
               int *character_coord_y, int ***dungeon_contents,
               int **song_notes, const char ***strings, int *trapped,
               int *trap_coord_x, int *trap_coord_y, int *screen_cols,
               const char ***item_names, audio_state_t ** audio_state) {
    int index;
    *screen_cols = 40;
    *dungeon_contents = (int **) malloc(sizeof(int *) * 16);
    if (*dungeon_contents == NULL) {
        fprintf(stderr, "*dungeon_contents is NULL!\n");
        exit(1);
    }
    int i;
    for (i = 0; i < 16; i += 1) {
        (*dungeon_contents)[i] = (int *) malloc(sizeof(int) * 16);
        if ((*dungeon_contents)[i] == NULL) {
            fprintf(stderr, "(*dungeon_contents)[%i] is NULL!\n", i);
            exit(1);
        }
    }
    *attrs = (double *) malloc(sizeof(double) * 9);
    if (attrs == NULL) {
        fprintf(stderr, "*attrs is NULL!\n");
        exit(1);
    }
    *item_names = (const char **) malloc(sizeof(const char *) * (12));
    if (*item_names == NULL) {
        fprintf(stderr, "*item_names is NULL!\n");
        exit(1);
    }
    *spells_remaining = (int *) malloc(sizeof(int) * 7);
    if (*spells_remaining == NULL) {
        fprintf(stderr, "*spells_remaining is NULL!\n");
        exit(1);
    }
    *vertices = (int **) malloc(sizeof(int *) * 5);
    if (*vertices == NULL) {
        fprintf(stderr, "*vertices is NULL!\n");
        exit(1);
    }
    for (i = 0; i < 5; i += 1) {
        (*vertices)[i] = (int *) malloc(sizeof(int) * 3);
        if ((*vertices)[i] == NULL) {
            fprintf(stderr, "(*vertices)[%i] is NULL!\n", i);
            exit(1);
        }
    }
    *song_notes = (int *) malloc(sizeof(int) * 19);
    if (*song_notes == NULL) {
        fprintf(stderr, "*song_notes is NULL!\n");
        exit(1);
    }
    (*item_names)[1] = "GR SWORD";
    (*item_names)[2] = "SWORD";
    (*item_names)[3] = "AXE";
    (*item_names)[4] = "MACE";
    (*item_names)[5] = "FLAIL";
    (*item_names)[6] = "DAGGER";
    (*item_names)[7] = "ARMOUR";
    (*item_names)[8] = "ARMOUR";
    (*item_names)[9] = "ARMOUR";
    (*item_names)[10] = "HELMET";
    (*item_names)[11] = "HEADPC.";


    *strings = (const char **) malloc(sizeof(const char *) * (13));
    if (*strings == NULL) {
        fprintf(stderr, "*strings is NULL!\n");
        exit(1);
    }

    (*strings)[1] = "A GOOD BLOW";
    (*strings)[2] = "WELL HIT SIRE";
    (*strings)[3] = "THY AIM IS TRUE";
    (*strings)[4] = "MISSED!";
    (*strings)[5] = "HIT THEE!!";
    (*strings)[6] = "THE MONSTER IS SLAIN";
    (*strings)[7] = "NO LIGHT";
    (*strings)[8] = "BROKEN THY ";
    (*strings)[9] = "SPELL EXHAUSTED";
    (*strings)[10] = "PRESS ANY KEY";
    (*strings)[11] = "YOU NEED EXPERIENCE";
    (*strings)[12] = "EXIT FROM THIS LEVEL";


    (*vertices)[1][1] = 0;
    (*vertices)[1][2] = -1;
    (*vertices)[2][1] = 1;
    (*vertices)[2][2] = 0;
    (*vertices)[3][1] = 0;
    (*vertices)[3][2] = 1;
    (*vertices)[4][1] = -1;
    (*vertices)[4][2] = 0;

    *finished = 0;
    *character_facing = 0;
    *trap_coord_x = 0;
    *trap_coord_y = 0;
    *trapped = 0;
    *char_code_hero = (char *) malloc(sizeof(char) * 7);
    if (*char_code_hero == NULL) {
        fprintf(stderr, "char_code_hero is NULL!\n");
        exit(1);
    }
    *character_coord_x = 1;
    *character_coord_y = 1;
    *torches = 0;
    for (index = 1; index <= 5; index += 1) {
        (*char_code_hero)[index] = DUNGEON_BASE + index;
    }
    (*char_code_hero)[6] = 0;
    (*song_notes)[1] = 69;
    (*song_notes)[2] = 117;
    (*song_notes)[3] = 73;
    (*song_notes)[4] = 121;
    (*song_notes)[5] = 81;
    (*song_notes)[6] = 129;
    (*song_notes)[7] = 69;
    (*song_notes)[8] = 117;
    (*song_notes)[9] = 73;
    (*song_notes)[10] = 121;
    (*song_notes)[11] = 81;
    (*song_notes)[12] = 129;
    (*song_notes)[13] = 89;
    (*song_notes)[14] = 137;
    (*song_notes)[15] = 97;
    (*song_notes)[16] = 145;
    (*song_notes)[17] = 101;
    (*song_notes)[18] = 149;
    init_platform_vars(audio_state);
}

int main(int argc, char *argv[]) {
    int ** vertices,
        finished,
        gold,
        dungeon_level,
        torches,
        * spells_remaining,
        character_facing,  // Facing. NESW
        character_coord_x,
        character_coord_y,
        inventory[25],
        num_item_types,
        character_prev_coord_x,
        character_prev_coord_y,
        ** dungeon_contents,
        item_at_character_coord,  // Object at character_coord_x / NY
        * song_notes,
        trapped,  // Flag to see if we can exit.
        treasure,
        trap_coord_x,
        trap_coord_y,
        screen_cols,
        coord_x,
        coord_y;
    double initial_strength, initial_vitality, initial_experience, * attrs;
    char * character_name = NULL,
         pressed_key,
         * char_code_hero = NULL,
         * message;
    const char ** strings, **item_names;
    monster_t * cur_monster = malloc(sizeof(monster_t));
    *cur_monster = (monster_t) {
        .coord_x = 0,
        .coord_y = 0,
        .next_coord_x = 0,
        .next_coord_y = 0,
        .speed = 0,
        .distance_x = 255
    };
    screen_t *screen = init_screen();
    audio_state_t * audio_state;
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    clear_screen(screen);
    init_vars(
        &vertices, &attrs, &char_code_hero, &finished, &torches,
        &spells_remaining, &character_facing, &character_coord_x,
        &character_coord_y, &dungeon_contents, &song_notes, &strings, &trapped,
        &trap_coord_x, &trap_coord_y, &screen_cols, &item_names, &audio_state
    );
    load_character(
        screen, &character_name, attrs, &gold, &torches, spells_remaining,
        inventory, &num_item_types, &initial_strength, &initial_vitality,
        &initial_experience, strings, &treasure, screen_cols
    );
    load_level_wo_first_exp_check(
        screen, character_name, cur_monster, attrs, &dungeon_level,
        &character_coord_x, &character_coord_y, &character_prev_coord_x,
        &character_prev_coord_y, dungeon_contents, initial_experience, strings,
        screen_cols
    );
    int game_over = 0;
    do {
        SDL_RenderPresent(screen->ren);
        pressed_key = inkey$();
        if (pressed_key == 'a' && cur_monster->distance_x < 255 ) {
            attack_monster(
                screen, audio_state, attrs, cur_monster, inventory,
                dungeon_contents, strings, screen_cols, coord_x, coord_y
            );
        }
        if (
                pressed_key == 'c' &&
                attrs[AURA] > 0 &&
                inventory[17] + inventory[18] > 0
        ) {
            cast_spell(
                screen, audio_state, attrs, char_code_hero, cur_monster,
                spells_remaining, character_facing, &character_coord_x,
                &character_coord_y, inventory, dungeon_contents,
                item_at_character_coord, initial_strength, initial_vitality,
                strings, screen_cols
            );
        }
        if (pressed_key == 'g') {
            get_item(
                screen, audio_state, vertices, attrs, char_code_hero,
                &finished, gold, cur_monster, &character_facing,
                character_coord_x, character_coord_y, inventory,
                dungeon_contents, song_notes, &treasure
            );
        }
        if (pressed_key == 'p') {
            drink_potion(
                attrs, inventory, initial_strength, initial_vitality
            );
        }
        if (pressed_key == 'r') {
            light_torch(
                screen, &torches, cur_monster, character_coord_x,
                character_coord_y, dungeon_contents, strings, screen_cols
            );
        }
        if (pressed_key == 's') {
            save_game(
                screen, character_name, attrs, &finished, gold, dungeon_level,
                character_coord_x, character_coord_y, inventory,
                num_item_types, dungeon_contents, treasure, screen_cols
            );
        }
        if (pressed_key == 'b') {
            character_facing -= 1;
        }
        if (pressed_key == 'n') {
            character_facing += 1;
        }
        if (character_facing > 4) {
            character_facing = 1;
        }
        if (character_facing < 1) {
            character_facing = 4;
        }
        if (pressed_key == 'm') {
            character_coord_x += vertices[character_facing][1];
            character_coord_y += vertices[character_facing][2];
        }
        if (character_coord_y > 15) {
            character_coord_y = 15;
        }
        if (character_coord_y < 1) {
            character_coord_y = 1;
        }
        if (character_coord_x <  1) {
            character_coord_x = 1;
        }
        if (character_coord_x > 15) {
            character_coord_x = 15;
        }
        item_at_character_coord =
            dungeon_contents[character_coord_x][character_coord_y];
        if (item_at_character_coord == WALL) {
            coord_x = character_coord_x;
            coord_y = character_coord_y;
            render_coord_and_check_for_monster(
                screen, cur_monster, dungeon_contents, coord_x, coord_y
            );
            character_coord_x = character_prev_coord_x;
            character_coord_y = character_prev_coord_y;
            attrs[STRENGTH] -= 0.03;
        }
        if (item_at_character_coord == TRAP) {
            trap_coord_x = character_coord_x;
            trap_coord_y = character_coord_y;
            trapped = 1;
        }
        if (trapped == 1) {
            character_coord_x = trap_coord_x;
            character_coord_y = trap_coord_y;
        }
        if (
                attrs[STRENGTH] > initial_strength * 0.8 &&
                rand() % 8 < attrs[LUCK]
        ) {
            trapped = 0;
        }
        if (pressed_key != 0) {
            attrs[STRENGTH] = attrs[STRENGTH] * 0.99;
        }
        if (attrs[STRENGTH] < initial_strength) {
            attrs[STRENGTH] += attrs[VITALITY] / 1100;
        }
        draw_character_and_stats(
            screen, attrs, char_code_hero, character_facing, character_coord_x,
            character_coord_y
        );
        if (
                character_prev_coord_x != character_coord_x ||
                character_prev_coord_y != character_coord_y
        ) {
            coord_x = character_prev_coord_x;
            coord_y = character_prev_coord_y;
            render_coord_and_check_for_monster(
                screen, cur_monster, dungeon_contents, coord_x, coord_y
            );
        }
        character_prev_coord_x = character_coord_x;
        character_prev_coord_y = character_coord_y;
        if (cur_monster->distance_x < 255) {
            monsters_turn(
                screen, audio_state, attrs, cur_monster, character_coord_x,
                character_coord_y, inventory, dungeon_contents,
                item_at_character_coord, strings, screen_cols, item_names
            );
        }
        if (
                attrs[STRENGTH] > 0 && finished < 1 &&
                item_at_character_coord != EXIT
        ) {
            game_over = 0;
        }
        else if (item_at_character_coord == EXIT) {
            message =
                (char *) malloc(sizeof(char) * (strlen(strings[12]) + 1));
            if (message == NULL) {
                fprintf(stderr, "message is NULL!\n");
                exit(1);
            }
            strcpy(message, strings[12]);
            message[strlen(strings[12])] = 0;
            draw_message(screen, message, screen_cols);
            free(message);
            load_level_with_first_exp_check(
                screen, character_name, cur_monster, attrs, &dungeon_level,
                &character_coord_x, &character_coord_y,
                &character_prev_coord_x, &character_prev_coord_y,
                dungeon_contents, initial_experience, strings, screen_cols
            );
            game_over = 0;
        } else {
            game_over = 1;
        }
    } while (!game_over);
    if (attrs[STRENGTH] < 1) {
        character_dies(
            screen, audio_state, attrs, char_code_hero, cur_monster,
            &character_facing, character_coord_x, character_coord_y,
            dungeon_contents, screen_cols, coord_x, coord_y
        );
    }
    tab(screen->cursor, 0, 10);

    destroy_audio_state(audio_state);
    destroy_screen(screen);

    int i;
    free(character_name);
    for (i = 0; i < 5; i += 1) {
        free(vertices[i]);
    }
    free(vertices);
    free(attrs);
    free(char_code_hero);
    free(spells_remaining);
    for (i = 0; i < 16; i += 1) {
        free(dungeon_contents[i]);
    }
    free(dungeon_contents);
    free(song_notes);
    free(strings);
    free(item_names);
    free(cur_monster);
    return 0;
}
