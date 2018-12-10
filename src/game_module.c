#include <SDL.h>
#include <unistd.h>
#include "dungeon_lib.h"
#include "dungeon_audio.h"
#include "character_lib.h"

typedef struct {
    int coord_x;
    int coord_y;
    int prev_coord_x;
    int prev_coord_y;
    int facing;  // Facing. NESW
    double * attrs;
    double initial_strength;
    double initial_vitality;
    double initial_experience;
    char * name;
    int * inventory;
    int * spells_remaining;
    int treasure;
    int gold;
    int torches;
} character_t;

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

void sound(audio_state_t * audio_state,
           Uint8 *(sound_func)(
               int frequency, int length, SDL_AudioSpec * audio_spec
           ), int stream_num, int sound_frequency) {
    int length = 0.3 * audio_state->audio_spec->freq;
    Uint8 * stream = sound_func(
        sound_frequency * 16.940,
        length,
        audio_state->audio_spec
    );
    volume_filter(stream, length, 0, length, 0, 0);
    stream_queue_enqueue(*(audio_state->streams + stream_num), stream, length);
    SDL_PauseAudioDevice(audio_state->device, 0);
}

void sound_sawtooth(audio_state_t * audio_state, int sound_frequency) {
    sound(audio_state, &sawtooth, 0, sound_frequency);
}

void sound_noise(audio_state_t * audio_state, int sound_frequency) {
    sound(audio_state, &noise, 1, sound_frequency);
}

char get_keyboard_input(screen_t * screen, char * message) {
    char pressed_key;
    SDL_Rect * text_rect;
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    tab(screen->cursor, 0, 5);
    text_rect = print_text(screen, message);
    SDL_RenderPresent(screen->ren);
    pressed_key = inkey$();
    clear_rect(screen, text_rect, YELLOW);
    free(text_rect);
    return pressed_key;
}

void draw_message(screen_t * screen, char * message) {
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    tab(screen->cursor, 0, 5);
    SDL_Rect * text_rect = print_text(screen, message);
    SDL_RenderPresent(screen->ren);
    SDL_Delay(340);
    clear_rect(screen, text_rect, YELLOW);
    free(text_rect);
}

void draw_character_and_stats(screen_t * screen, char * char_code_hero,
                              character_t * character) {
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    render_bitmap(
        screen,
        character->coord_x,
        character->coord_y + 5,
        char_code_hero[character->facing] - DUNGEON_BASE,
        WHITE,
        RED
    );
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    tab(screen->cursor, 16, 8);
    sprintf(outstring, "%i ", (int) character->attrs[STRENGTH]);
    free(print_text(screen, outstring));

    tab(screen->cursor, 16, 11);
    sprintf(outstring, "%i ", (int) character->attrs[VITALITY]);
    free(print_text(screen, outstring));
    tab(screen->cursor, 16, 14);
    sprintf(outstring, "%i ", (int) character->attrs[AURA]);
    free(print_text(screen, outstring));
    tab(screen->cursor, 16, 17);
    outstring[0] = "NESW."[character->facing - 1];
    outstring[1] = 0;
    free(print_text(screen, outstring));
    tab(screen->cursor, 16, 20);
    sprintf(outstring, "%i ", (int) character->attrs[EXPERIENCE]);
    free(print_text(screen, outstring));
    free(outstring);
    SDL_RenderPresent(screen->ren);
}

enum CharCode render_coord(screen_t * screen, int ** dungeon_contents,
                           int coord_x, int coord_y) {
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
    return item_at_coord;
}

void check_for_monster(enum CharCode item_at_coord, monster_t * cur_monster,
                       int coord_x, int coord_y) {
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


void render_coord_and_check_for_monster(screen_t * screen,
                                        monster_t * cur_monster,
                                        int ** dungeon_contents, int coord_x,
                                        int coord_y) {
    int item_at_coord = render_coord(
            screen,
            dungeon_contents,
            coord_x,
            coord_y
        );
    check_for_monster(item_at_coord, cur_monster, coord_x, coord_y);
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

void monster_breaks_items(screen_t * screen, audio_state_t * audio_state,
                          int item_num, int sound_frequency,
                          character_t * character, const char ** strings,
                          const char ** item_names) {
    char * message;
    character->inventory[item_num] = 0;
    message = (char *) malloc(
        sizeof(char) * (strlen(strings[8]) + strlen(item_names[item_num]) + 2)
    );
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    sprintf(message, "%s %s", strings[8], item_names[item_num]);
    draw_message(screen, message);
    free(message);
    sound_noise(audio_state, sound_frequency);
    sound_frequency = item_num;
    sound_sawtooth(audio_state, sound_frequency);
}

void monster_moves(screen_t * screen, monster_t * cur_monster,
                   character_t * character, int ** dungeon_contents) {
    int item_at_monster_next_coord, direction_to_monster_x,
        direction_to_monster_y;
    cur_monster->distance_x = (int) cur_monster->coord_x - character->coord_x;
    direction_to_monster_x = sign(cur_monster->distance_x);
    cur_monster->distance_y = (int) cur_monster->coord_y - character->coord_y;
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
    render_coord(
        screen, dungeon_contents, cur_monster->coord_x, cur_monster->coord_y
    );
    dungeon_contents[
        (int) cur_monster->next_coord_x
    ][(int) cur_monster->next_coord_y] = cur_monster->char_code;
    render_coord(
        screen, dungeon_contents, (int) cur_monster->next_coord_x,
        (int) cur_monster->next_coord_y
    );
    cur_monster->coord_x = cur_monster->next_coord_x;
    cur_monster->coord_y = cur_monster->next_coord_y;
}

void monster_attacks(screen_t * screen, audio_state_t * audio_state,
                     monster_t * cur_monster, character_t * character,
                     int item_at_character_coord, const char ** strings,
                     const char ** item_names) {
    int damage, sound_frequency, item_num, monster_broke_item;
    char * message;
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
    if (damage * 12 < character->attrs[LUCK] + character->attrs[AGILITY]) {
        return;
    }
    message = (char *) malloc(sizeof(char) * (strlen(strings[5]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, strings[5]);
    draw_message(screen, message);
    free(message);
    sound_noise(audio_state, sound_frequency);
    damage /= (
        3 + character->inventory[HEAVY_ARMOUR] +
        character->inventory[CHAIN_ARMOUR] +
        character->inventory[LEATHER_ARMOUR] +
        character->inventory[HEAVY_ROBE] + character->inventory[GOLD_HELMET] +
        character->inventory[HEADPIECE]
    );
    character->attrs[STRENGTH] -= damage;
    character->attrs[VITALITY] -= damage / 101;
    item_num = 0;
    monster_broke_item = rand() % cur_monster->type;
    sound_frequency = cur_monster->char_code;
    sound_sawtooth(audio_state, sound_frequency);
    sound_noise(audio_state, sound_frequency);
    int done = 0;
    do {
        if (monster_broke_item == 1 && character->inventory[item_num] > 0) {
            monster_breaks_items(
                screen, audio_state, item_num, sound_frequency, character,
                strings, item_names
            );
            monster_broke_item = 0;
        }
        if (item_num < 10) {
            item_num += 1;
        } else {
            done = 1;
        }
    } while (!done);
}

void monsters_turn(screen_t * screen, audio_state_t * audio_state,
                   monster_t * cur_monster, character_t * character,
                   int ** dungeon_contents, int item_at_character_coord,
                   const char ** strings, const char ** item_names) {
    monster_moves(screen, cur_monster, character, dungeon_contents);
    monster_attacks(
        screen, audio_state, cur_monster, character, item_at_character_coord,
        strings, item_names
    );
}

void character_dies(screen_t * screen, audio_state_t * audio_state,
                    char * char_code_hero, character_t * character) {
    char * message;
    int sound_frequency;
    character->facing = 5;
    character->attrs[STRENGTH] = 0;
    message = (char *) malloc(sizeof(char));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, "");
    draw_message(screen, message);
    free(message);
    tab(screen->cursor, 1, 5);
    free(print_text(screen, "THOU HAST EXPIRED!"));
    for (sound_frequency = 150; sound_frequency >= 1; sound_frequency -= 4) {
        sound_sawtooth(audio_state, sound_frequency);
        sound_noise(audio_state, sound_frequency);
        draw_character_and_stats(screen, char_code_hero, character);
    }
}

void monster_dies(screen_t * screen, audio_state_t * audio_state,
                  character_t * character, monster_t * cur_monster,
                  int ** dungeon_contents, const char ** strings) {
    char * message;
    int sound_frequency;
    cur_monster->distance_x = 255;
    cur_monster->strength = 0;
    dungeon_contents[
        (int) cur_monster->next_coord_x
    ][(int) cur_monster->next_coord_y] = BLANK;
    character->attrs[EXPERIENCE] += 0.1;
    message = (char *) malloc(sizeof(char) * (strlen(strings[6]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, strings[6]);
    draw_message(screen, message);
    free(message);
    for (sound_frequency = 200; sound_frequency >= 150; sound_frequency -= 8) {
        sound_sawtooth(audio_state, sound_frequency);
        sound_noise(audio_state, sound_frequency);
    }
    render_coord(
        screen, dungeon_contents, cur_monster->coord_x, cur_monster->coord_y
    );
}


void attack_monster(screen_t * screen, audio_state_t * audio_state,
                    character_t * character, monster_t * cur_monster,
                    int ** dungeon_contents, const char ** strings) {
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
        character->attrs[STRENGTH] + character->inventory[TWO_HAND_SWORD] +
        character->inventory[BROADSWORD] + character->inventory[SHORTSWORD] +
        character->inventory[AXE] + character->inventory[MACE] +
        character->inventory[FLAIL] + character->inventory[DAGGER] +
        character->inventory[GAUNTLET] + (
            rand() * character->attrs[LUCK] / RAND_MAX
        );
    if (
            character->attrs[AGILITY] + character->attrs[LUCK] <
                rand() % cur_monster->type + 2
    ) {
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
    draw_message(screen, message);
    free(message);
    character->attrs[STRENGTH] -= damage / 100;
    character->attrs[EXPERIENCE] += 0.05;
    if (cur_monster->strength < 1) {
        monster_dies(
            screen, audio_state, character, cur_monster, dungeon_contents,
            strings
        );
    }
}

void cast_superzap(screen_t * screen, audio_state_t * audio_state,
                   character_t * character, monster_t * cur_monster,
                   int ** dungeon_contents, const char ** strings) {
    int sound_frequency;
    for (sound_frequency = 1; sound_frequency <= 12; sound_frequency += 1) {
        sound_sawtooth(audio_state, sound_frequency);
        sound_noise(audio_state, sound_frequency);
    }
    if (cur_monster->distance_x < 255) {
        monster_dies(
            screen, audio_state, character, cur_monster, dungeon_contents,
            strings
        );
    }
}

void cast_sanctuary(audio_state_t * audio_state, character_t * character,
                    int ** dungeon_contents, int item_at_character_coord) {
    int sound_frequency;
    if (item_at_character_coord == BLANK) {
        dungeon_contents[character->coord_x][character->coord_y] = SAFE_PLACE;
    }
    sound_frequency = 100;
    sound_sawtooth(audio_state, sound_frequency);
    sound_frequency = 200;
    sound_sawtooth(audio_state, sound_frequency);
}

void cast_teleport(screen_t * screen, audio_state_t * audio_state,
                   char * char_code_hero, character_t * character) {
    int sound_frequency;
    character->coord_x = rand() % 13;
    character->coord_y = rand() % 13;
    for (sound_frequency = 0; sound_frequency <= 255; sound_frequency += 8) {
        sound_noise(audio_state, sound_frequency);
        sound_sawtooth(audio_state, sound_frequency);
    }
    draw_character_and_stats(screen, char_code_hero, character);
}

void cast_powersurge(character_t * character, int spell_number) {
    character->attrs[VITALITY] +=
        rand() % character->spells_remaining[spell_number];
    character->attrs[STRENGTH] +=
        rand() % character->spells_remaining[spell_number];
    character->attrs[AURA] -= 1;
}

void cast_metamorphosis(screen_t * screen, audio_state_t * audio_state,
                        monster_t * cur_monster, character_t * character,
                        int ** dungeon_contents, int item_at_character_coord) {
    int sound_frequency;
    for (sound_frequency = 1; sound_frequency <= 30; sound_frequency += 1) {
        dungeon_contents[character->coord_x][character->coord_y] =
            rand() % 8 + 1 + BLANK;
        sound_sawtooth(audio_state, sound_frequency);
        render_coord(
            screen, dungeon_contents, character->coord_x, character->coord_y
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

void cast_healing(character_t * character) {
    character->attrs[VITALITY] = character->initial_vitality;
    character->attrs[STRENGTH] = character->initial_strength;
    character->attrs[AURA] -= 1;
}

void cast_spell(screen_t * screen, audio_state_t * audio_state,
                char * char_code_hero, monster_t * cur_monster,
                character_t * character, int ** dungeon_contents,
                int item_at_character_coord, const char ** strings,
                int screen_cols) {
    int spell_number;
    char pressed_key, * message;
    draw_character_and_stats(screen, char_code_hero, character);
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    tab(screen->cursor, 0, 1);
    free(print_text(screen, "YOU MAY USE MAGICKS"));
    if (character->inventory[NECRONOMICON] > 0) {
        tab(screen->cursor, 0, 2);
        free(print_text(screen, "FROM NECRONOMICON"));
    }
    if (character->inventory[SCROLLS] > 0) {
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
        pressed_key = get_keyboard_input(screen, message);
        free(message);
        char * outstring = (char *) malloc(sizeof(char) * 2);
        if (outstring == NULL) {
            fprintf(stderr, "outstring is NULL!\n");
            exit(1);
        }
        sprintf(outstring, "%c", pressed_key);
        spell_number = atoi(outstring);
        free(outstring);
    } while (
            spell_number == 0 ||
            (character->inventory[NECRONOMICON] == 0 && spell_number < 5) ||
            (character->inventory[SCROLLS] == 0 && spell_number > 3) ||
            spell_number > 6
    );

    character->spells_remaining[spell_number] -= 1;
    if (character->spells_remaining[spell_number] < 0) {
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
    SDL_Rect rect = {
        .x = 0,
        .y = 8 * screen->zoom,
        .w = screen_cols * 8 * screen->zoom,
        .h = 5 * 8 * screen->zoom
    };
    clear_rect(screen, &rect, YELLOW);
    switch (spell_number) {
        case 1:
            cast_superzap(
                screen, audio_state, character, cur_monster, dungeon_contents,
                strings
            );
            break;
        case 2:
            cast_sanctuary(
                audio_state, character, dungeon_contents,
                item_at_character_coord
            );
            break;
        case 3:
            cast_teleport(screen, audio_state, char_code_hero, character);
            break;
        case 4:
            cast_powersurge(character, spell_number);
            break;
        case 5:
            cast_metamorphosis(
                screen, audio_state, cur_monster, character, dungeon_contents,
                item_at_character_coord
            );
            break;
        case 6:
            cast_healing(character);
            break;
        case 7:
            break;
    }
    character->attrs[EXPERIENCE] += 0.2;
    draw_message(screen, message);
    free(message);
}

int * init_song_notes() {
    int * song_notes = malloc(sizeof(int) * 18);
    if (song_notes == NULL) {
        fprintf(stderr, "song_notes is NULL!\n");
        exit(1);
    }
    song_notes[0] = 69;
    song_notes[1] = 117;
    song_notes[2] = 73;
    song_notes[3] = 121;
    song_notes[4] = 81;
    song_notes[5] = 129;
    song_notes[6] = 69;
    song_notes[7] = 117;
    song_notes[8] = 73;
    song_notes[9] = 121;
    song_notes[10] = 81;
    song_notes[11] = 129;
    song_notes[12] = 89;
    song_notes[13] = 137;
    song_notes[14] = 97;
    song_notes[15] = 145;
    song_notes[16] = 101;
    song_notes[17] = 149;

    return song_notes;
}

void game_won(screen_t * screen, audio_state_t * audio_state,
              char * char_code_hero, int * finished, monster_t * cur_monster,
              character_t * character) {
    int index, sound_frequency, direction,
        * song_notes = init_song_notes();
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, RED);
    tab(screen->cursor, 0, 1);
    free(print_text(screen, " THY QUEST IS OVER! "));
    for (index = 0; index < 18; index += 1) {
        sound_frequency = song_notes[index];
        sound_sawtooth(audio_state, sound_frequency);

        for (direction = 1; direction <= 4; direction += 1) {
            character->facing = direction;
            draw_character_and_stats(screen, char_code_hero, character);
            SDL_Delay(300 / 4);
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
            (character->treasure * 10) + (
                character->gold * character->attrs[EXPERIENCE]
            ) + character->attrs[STRENGTH] + character->attrs[VITALITY] +
            character->attrs[AGILITY]
        )
    );
    free(print_text(screen, outstring));
    free(outstring);
    free(song_notes);
    *finished = 1;
}

void get_item(screen_t * screen, audio_state_t * audio_state,
              int ** vertices, char * char_code_hero, int * finished,
              monster_t * cur_monster, character_t * character,
              int ** dungeon_contents) {
    int sound_frequency, item_to_get, item_to_get_coord_x, item_to_get_coord_y;
    item_to_get_coord_x = character->coord_x + vertices[character->facing][1];
    item_to_get_coord_y = character->coord_y + vertices[character->facing][2];
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
        character->inventory[HEALING_SALVE] += 1;
        character->inventory[POTIONS] += 1;
    }
    if (item_to_get == CHEST) {
        character->treasure += 1;
    }
    if (item_to_get == IDOL) {
        game_won(
            screen, audio_state, char_code_hero, finished, cur_monster,
            character
        );
    }
    render_coord(
        screen, dungeon_contents, item_to_get_coord_x, item_to_get_coord_y
    );
    if (item_to_get > WALL && item_to_get < IDOL) {
        sound_frequency = item_to_get;
        sound_sawtooth(audio_state, sound_frequency);
        sound_frequency = item_to_get + 5;
        sound_sawtooth(audio_state, sound_frequency);
    }
}

void drink_potion(character_t * character) {
    if (
            character->inventory[POTIONS] > 0 &&
            character->attrs[STRENGTH] < character->initial_strength
    ) {
        character->attrs[STRENGTH] = character->initial_strength;
        character->inventory[POTIONS] -= 1;
    }
    if (
            character->inventory[HEALING_SALVE] > 0 &&
            character->attrs[VITALITY] < character->initial_vitality
    ) {
        character->attrs[VITALITY] = character->initial_vitality;
        character->inventory[HEALING_SALVE] -= 1;
    }
}

void light_torch(screen_t * screen, monster_t * cur_monster,
                 character_t * character, int ** dungeon_contents,
                 const char ** strings) {
    int coord_x, coord_y;
    char * message;
    if (character->torches == 0) {
        message = (char *) malloc(sizeof(char) * (strlen(strings[7]) + 1));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, strings[7]);
        draw_message(screen, message);
        free(message);
        return;
    }
    for (
            coord_y = character->coord_y - 3;
            coord_y <= character->coord_y + 3;
            coord_y += 1
    ) {
        for (
                coord_x = character->coord_x - 3;
                coord_x <= character->coord_x + 3;
                coord_x += 1
        ) {
            if (coord_x > 0 && coord_x < 16 && coord_y > 0 && coord_y < 16) {
                render_coord_and_check_for_monster(
                    screen, cur_monster, dungeon_contents, coord_x, coord_y
                );
            }
        }
    }
    character->torches -= 1;
}

void show_level_too_deep_messages(screen_t * screen, character_t * character) {
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
    sprintf(outstring, "FOR LEVEL %d", (int) character->attrs[EXPERIENCE]);
    free(print_text(screen, outstring));
    free(outstring);
}


void draw_interface(screen_t * screen, character_t * character,
                    int screen_cols) {
    paper(screen->cursor, RED);
    clear_screen(screen);
    draw_box(screen, 0, 0, 1, screen_cols, WHITE);
    tab(screen->cursor, 0, 0);
    paper(screen->cursor, WHITE);
    ink(screen->cursor, BLACK);
    free(print_text(screen, character->name));
    draw_box(screen, 1, 0, 5, screen_cols, YELLOW);
    draw_box(screen, 6, 1, 15, 15, BLACK);
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

void load_level(screen_t * screen, int skip_first_exp_check,
                monster_t * cur_monster, int * dungeon_level,
                character_t * character, int ** dungeon_contents,
                const char ** strings, int screen_cols) {
    int correct_level_loaded, index, entrance_coord_x, entrance_coord_y,
        coord_x, coord_y;
    char * message;
    do {

        if (
                !skip_first_exp_check &&
                character->attrs[EXPERIENCE] <
                    character->initial_experience + 1
        ) {
            message =
                (char *) malloc(sizeof(char) * (strlen(strings[11]) + 1));
            if (message == NULL) {
                fprintf(stderr, "message is NULL!\n");
                exit(1);
            }
            strcpy(message, strings[11]);
            character->coord_x = character->prev_coord_x;
            character->coord_y = character->prev_coord_y;
            draw_message(screen, message);
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
        get_keyboard_input(screen, message);
        free(message);
        size_t filesize;
        FILE * file_handle = fopen("LEVEL", "r");
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
        if (*dungeon_level > character->attrs[EXPERIENCE]) {
            show_level_too_deep_messages(screen, character);
            correct_level_loaded = 1;
        } else {
            correct_level_loaded = 0;
        }
        free(file_contents);
    } while (correct_level_loaded);
    draw_interface(screen, character, screen_cols);
    character->coord_x = entrance_coord_x;
    character->coord_y = entrance_coord_y;
    character->prev_coord_x = character->coord_x;
    character->prev_coord_y = character->coord_y;
    cur_monster->distance_x = 255;
}

void load_level_with_first_exp_check(screen_t * screen,
                                     monster_t * cur_monster,
                                     int * dungeon_level,
                                     character_t * character,
                                     int ** dungeon_contents,
                                     const char ** strings, int screen_cols) {
    load_level(
        screen, 0, cur_monster, dungeon_level, character, dungeon_contents,
        strings, screen_cols
    );
}

void load_level_wo_first_exp_check(screen_t * screen, monster_t * cur_monster,
                                   int * dungeon_level,
                                   character_t * character,
                                   int ** dungeon_contents,
                                   const char ** strings, int screen_cols) {
    load_level(
        screen, 1, cur_monster, dungeon_level, character, dungeon_contents,
        strings, screen_cols
    );
}

void load_character(screen_t * screen, character_t * character,
                    int * num_item_types, const char ** strings) {
    char * message;
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
    get_keyboard_input(screen, message);
    free(message);
    FILE * file_handle = fopen("HERO", "r");
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
    for (index = 0; index < 8; index += 1) {
        character->attrs[index] =
            (int) file_contents[file_index - 1] - CHARACTER_BASE;
        file_index += 1;
    }
    character->inventory = malloc(sizeof(int) * *num_item_types);
    for (index = 0; index < *num_item_types; index += 1) {
        character->inventory[index] =
            (int) file_contents[file_index - 1] - CHARACTER_BASE;
        file_index += 1;
    }
    character->gold = (int) file_contents[file_index - 1] - CHARACTER_BASE;
    character->treasure = (int) file_contents[file_index] - CHARACTER_BASE;
    character->name = (char *) malloc(sizeof(char) * (file_index + 2));
    if (character->name == NULL) {
        fprintf(stderr, "character->name is NULL!\n");
        exit(1);
    }
    strcpy(character->name, file_contents + file_index + 1);
    character->initial_strength = character->attrs[STRENGTH];
    character->initial_vitality = character->attrs[VITALITY];
    character->initial_experience = character->attrs[EXPERIENCE];
    for (index = 1; index <= 2; index += 1) {
        for (subindex = 1; subindex <= 3; subindex += 1) {
            character->spells_remaining[(index - 1) * 3 + subindex] =
                character->inventory[TORCH + index] * character->attrs[AURA];
        }
    }
    if (character->inventory[TORCH] == 1) {
        character->torches = 20;
    }
    free(file_contents);
}

void save_game(screen_t * screen, int * finished, int dungeon_level,
               character_t * character, int num_item_types,
               int ** dungeon_contents) {
    int index, coord_x, coord_y;
    char * message;
    message = (char *) malloc(sizeof(char) * 18);
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, "ONE MOMENT PLEASE");
    draw_message(screen, message);
    free(message);
    char * character_file_contents = (char *) malloc(
        sizeof(char) * (12 + num_item_types + strlen(character->name))
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
        (char) (DUNGEON_BASE + character->coord_x);
    t_index += 1;
    dungeon_file_contents[t_index] =
        (char) (DUNGEON_BASE + character->coord_y);
    t_index += 1;
    dungeon_file_contents[t_index] =
        (char) (DUNGEON_BASE + dungeon_level);
    t_index += 1;
    dungeon_file_contents[t_index] = 0;
    character_file_contents[s_index] =
        (char) (CHARACTER_BASE + num_item_types);
    s_index += 1;
    for (index = 0; index < 8; index += 1) {
        character_file_contents[s_index] =
            (char) (character->attrs[index] + CHARACTER_BASE);
        s_index += 1;
    }
    for (index = 0; index < num_item_types; index += 1) {
        character_file_contents[s_index] = (char)
            (character->inventory[index] + CHARACTER_BASE);
        s_index += 1;
    }
    character_file_contents[s_index] = (char) (
        character->gold + CHARACTER_BASE
    );
    s_index += 1;
    character_file_contents[s_index] = (char) (
        character->treasure + CHARACTER_BASE
    );
    s_index += 1;
    strcpy(character_file_contents + s_index, character->name);
    s_index += strlen(character->name);
    character_file_contents[s_index] = 0;
    message = (char *) malloc(sizeof(char) * 16);
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, "ANY KEY TO SAVE");
    get_keyboard_input(screen, message);
    free(message);
    FILE * file_handle = fopen("HERO", "w");
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

void init_vars(int *** vertices, char ** char_code_hero, int * finished,
               character_t * character, int *** dungeon_contents,
               const char *** strings, int * trapped, int * trap_coord_x,
               int * trap_coord_y, int * screen_cols,
               const char *** item_names, audio_state_t ** audio_state) {
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
    character->attrs = (double *) malloc(sizeof(double) * 9);
    if (character->attrs == NULL) {
        fprintf(stderr, "character->attrs is NULL!\n");
        exit(1);
    }
    *item_names = (const char **) malloc(sizeof(const char *) * (11));
    if (*item_names == NULL) {
        fprintf(stderr, "*item_names is NULL!\n");
        exit(1);
    }
    character->spells_remaining = (int *) malloc(sizeof(int) * 7);
    if (character->spells_remaining == NULL) {
        fprintf(stderr, "character->spells_remaining is NULL!\n");
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
    (*item_names)[0] = "GR SWORD";
    (*item_names)[1] = "SWORD";
    (*item_names)[2] = "AXE";
    (*item_names)[3] = "MACE";
    (*item_names)[4] = "FLAIL";
    (*item_names)[5] = "DAGGER";
    (*item_names)[6] = "ARMOUR";
    (*item_names)[7] = "ARMOUR";
    (*item_names)[8] = "ARMOUR";
    (*item_names)[9] = "HELMET";
    (*item_names)[10] = "HEADPC.";


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
    character->facing = 0;
    *trap_coord_x = 0;
    *trap_coord_y = 0;
    *trapped = 0;
    *char_code_hero = (char *) malloc(sizeof(char) * 7);
    if (*char_code_hero == NULL) {
        fprintf(stderr, "char_code_hero is NULL!\n");
        exit(1);
    }
    character->coord_x = 1;
    character->coord_y = 1;
    character->torches = 0;
    for (index = 1; index <= 5; index += 1) {
        (*char_code_hero)[index] = DUNGEON_BASE + index;
    }
    (*char_code_hero)[6] = 0;
    init_platform_vars(audio_state);
}

int main(__attribute__((__unused__)) int argc,
         __attribute__((__unused__)) char * argv[]) {
    int ** vertices,
        finished,
        dungeon_level,
        num_item_types,
        ** dungeon_contents,
        item_at_character_coord,  // Object at character->coord_x / NY
        trapped,  // Flag to see if we can exit.
        trap_coord_x,
        trap_coord_y,
        screen_cols;
    char pressed_key,
         * char_code_hero = NULL,
         * message;
    const char ** strings, ** item_names;
    monster_t * cur_monster = malloc(sizeof(monster_t));
    if (cur_monster == NULL) {
        fprintf(stderr, "cur_monster is NULL!\n");
        exit(1);
    }
    *cur_monster = (monster_t) {
        .coord_x = 0,
        .coord_y = 0,
        .next_coord_x = 0,
        .next_coord_y = 0,
        .speed = 0,
        .distance_x = 255
    };
    character_t * character = malloc(sizeof(character_t));
    if (character == NULL) {
        fprintf(stderr, "character is NULL!\n");
        exit(1);
    }
    screen_t * screen = init_screen();
    audio_state_t * audio_state;
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    clear_screen(screen);
    init_vars(
        &vertices, &char_code_hero, &finished, character, &dungeon_contents,
        &strings, &trapped, &trap_coord_x, &trap_coord_y, &screen_cols,
        &item_names, &audio_state
    );
    load_character(screen, character, &num_item_types, strings);
    load_level_wo_first_exp_check(
        screen, cur_monster, &dungeon_level, character, dungeon_contents,
        strings, screen_cols
    );
    int game_over = 0;
    do {
        SDL_RenderPresent(screen->ren);
        pressed_key = inkey$();
        if (pressed_key == 'a' && cur_monster->distance_x < 255 ) {
            attack_monster(
                screen, audio_state, character, cur_monster, dungeon_contents,
                strings
            );
        }
        if (
                pressed_key == 'c' &&
                character->attrs[AURA] > 0 &&
                character->inventory[NECRONOMICON] +
                    character->inventory[SCROLLS] > 0
        ) {
            cast_spell(
                screen, audio_state, char_code_hero, cur_monster, character,
                dungeon_contents, item_at_character_coord, strings, screen_cols
            );
        }
        if (pressed_key == 'g') {
            get_item(
                screen, audio_state, vertices, char_code_hero, &finished,
                cur_monster, character, dungeon_contents
            );
        }
        if (pressed_key == 'p') {
            drink_potion(character);
        }
        if (pressed_key == 'r') {
            light_torch(
                screen, cur_monster, character, dungeon_contents, strings
            );
        }
        if (pressed_key == 's') {
            save_game(
                screen, &finished, dungeon_level, character, num_item_types,
                dungeon_contents
            );
        }
        if (pressed_key == 'b') {
            character->facing -= 1;
        }
        if (pressed_key == 'n') {
            character->facing += 1;
        }
        if (character->facing > 4) {
            character->facing = 1;
        }
        if (character->facing < 1) {
            character->facing = 4;
        }
        if (pressed_key == 'm') {
            character->coord_x += vertices[character->facing][1];
            character->coord_y += vertices[character->facing][2];
        }
        if (character->coord_y > 15) {
            character->coord_y = 15;
        }
        if (character->coord_y < 1) {
            character->coord_y = 1;
        }
        if (character->coord_x <  1) {
            character->coord_x = 1;
        }
        if (character->coord_x > 15) {
            character->coord_x = 15;
        }
        item_at_character_coord =
            dungeon_contents[character->coord_x][character->coord_y];
        if (item_at_character_coord == WALL) {
            render_coord(
                screen, dungeon_contents, character->coord_x,
                character->coord_y
            );
            character->coord_x = character->prev_coord_x;
            character->coord_y = character->prev_coord_y;
            character->attrs[STRENGTH] -= 0.03;
        }
        if (item_at_character_coord == TRAP) {
            trap_coord_x = character->coord_x;
            trap_coord_y = character->coord_y;
            trapped = 1;
        }
        if (trapped == 1) {
            character->coord_x = trap_coord_x;
            character->coord_y = trap_coord_y;
        }
        if (
                character->attrs[STRENGTH] >
                    character->initial_strength * 0.8 &&
                rand() % 8 < character->attrs[LUCK]
        ) {
            trapped = 0;
        }
        if (pressed_key != 0) {
            character->attrs[STRENGTH] = character->attrs[STRENGTH] * 0.99;
        }
        if (character->attrs[STRENGTH] < character->initial_strength) {
            character->attrs[STRENGTH] += character->attrs[VITALITY] / 1100;
        }
        draw_character_and_stats(screen, char_code_hero, character);
        if (
                character->prev_coord_x != character->coord_x ||
                character->prev_coord_y != character->coord_y
        ) {
            render_coord_and_check_for_monster(
                screen, cur_monster, dungeon_contents, character->prev_coord_x,
                character->prev_coord_y
            );
        }
        character->prev_coord_x = character->coord_x;
        character->prev_coord_y = character->coord_y;
        if (cur_monster->distance_x < 255) {
            monsters_turn(
                screen, audio_state, cur_monster, character, dungeon_contents,
                item_at_character_coord, strings, item_names
            );
        }
        if (
                character->attrs[STRENGTH] > 0 && finished < 1 &&
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
            draw_message(screen, message);
            free(message);
            load_level_with_first_exp_check(
                screen, cur_monster, &dungeon_level, character,
                dungeon_contents, strings, screen_cols
            );
            game_over = 0;
        } else {
            game_over = 1;
        }
    } while (!game_over);
    if (character->attrs[STRENGTH] < 1) {
        character_dies(screen, audio_state, char_code_hero, character);
    }
    tab(screen->cursor, 0, 10);

    destroy_audio_state(audio_state);
    destroy_screen(screen);

    int i;
    free(character->name);
    free(character->inventory);
    for (i = 0; i < 5; i += 1) {
        free(vertices[i]);
    }
    free(vertices);
    free(character->attrs);
    free(char_code_hero);
    free(character->spells_remaining);
    for (i = 0; i < 16; i += 1) {
        free(dungeon_contents[i]);
    }
    free(dungeon_contents);
    free(strings);
    free(item_names);
    free(character);
    free(cur_monster);
    return 0;
}
