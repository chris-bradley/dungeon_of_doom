#include <SDL.h>
#include <errno.h>
#include <unistd.h>
#include "dungeon_lib.h"
#include "monster_lib.h"
#include "dungeon_audio.h"
#include "character_lib.h"
#include "libcoord.h"
#include "libmap.h"


typedef struct {
    screen_t * screen;
    audio_state_t * audio_state;
    const char ** strings;
    int screen_cols;
} ui_t;

typedef struct {
    character_t * character;
    dungeon_t * dungeon;
    monster_list_t * monster_list;
    int finished;
} game_state_t;

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

char get_keyboard_input(screen_t * screen, const char * message) {
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

void draw_message(screen_t * screen, const char * message) {
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
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "outstring is NULL!");
        exit(1);
    }
    render_bitmap(
        screen,
        character->coord.x + 1,
        character->coord.y + 6,
        char_code_hero[character->facing],
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
    outstring[0] = "NESW."[character->facing];
    outstring[1] = 0;
    free(print_text(screen, outstring));
    tab(screen->cursor, 16, 20);
    sprintf(outstring, "%i ", (int) character->attrs[EXPERIENCE]);
    free(print_text(screen, outstring));
    free(outstring);
    SDL_RenderPresent(screen->ren);
}

enum CharCode render_coord(screen_t * screen, dungeon_t * dungeon,
                           coord_t coord) {
    enum CharCode item_at_coord;
    paper(screen->cursor, RED);
    ink(screen->cursor, YELLOW);
    item_at_coord = dungeon_t_get_item(dungeon, coord);
    tab(screen->cursor, coord.x + 1, coord.y + 6);
    render_bitmap(
        screen,
        screen->cursor->curs_x,
        screen->cursor->curs_y,
        item_at_coord,
        YELLOW,
        RED
    );
    return item_at_coord;
}

void check_for_monster(enum CharCode item_at_coord,
                       monster_list_t * monster_list, coord_t coord) {
    if (item_at_coord <= SAFE_PLACE) {
        return;
    }
    monster_list_node_t * monster_list_node = monster_list->first_node;
    while (monster_list_node != NULL) {
        if (
                coord_float_t_is_equal(
                    monster_list_node->monster->coord, coord
                )
        ) {
            return;
        }
        monster_list_node = monster_list_node->next_node;
    }
    monster_list_add(monster_list, monster_init(item_at_coord - VASE, coord));
}


void render_coord_and_check_for_monster(screen_t * screen,
                                        game_state_t * game_state,
                                        coord_t coord) {
    int item_at_coord = render_coord(screen, game_state->dungeon, coord);
    check_for_monster(item_at_coord, game_state->monster_list, coord);
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

void monster_breaks_items(ui_t * ui, int item_num, int sound_frequency,
                          character_t * character, const char ** item_names) {
    char * message;
    character->inventory[item_num] = 0;
    message = (char *) malloc(
        sizeof(char) * (
            strlen(ui->strings[7]) + strlen(item_names[item_num]) + 2
        )
    );
    if (message == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "message is NULL!");
        exit(1);
    }
    sprintf(message, "%s %s", ui->strings[7], item_names[item_num]);
    draw_message(ui->screen, message);
    free(message);
    sound_noise(ui->audio_state, sound_frequency);
    sound_sawtooth(ui->audio_state, item_num);
}

void monster_moves(screen_t * screen, monster_t * monster,
                   character_t * character, dungeon_t * dungeon) {
    int direction_to_monster_x, direction_to_monster_y;
    direction_to_monster_x = sign((int) monster->coord.x - character->coord.x);
    direction_to_monster_y = sign((int) monster->coord.y - character->coord.y);
    coord_float_t next_coord_float = {
        .x = monster->coord.x - (
            monster->speed * (float) direction_to_monster_x
        ),
        .y = monster->coord.y - (
            monster->speed * (float) direction_to_monster_y
        )
    };
    coord_t next_coord;
    coord_t_set_from_float(&next_coord, next_coord_float);
    if (dungeon_t_get_item(dungeon, next_coord) > BLANK) {
        return;
    }
    coord_t monster_coord;
    coord_t_set_from_float(&monster_coord, monster->coord);
    dungeon_t_set_item(dungeon, monster_coord, BLANK);
    render_coord(screen, dungeon, monster_coord);
    dungeon_t_set_item(dungeon, next_coord, monster->char_code);
    render_coord(screen, dungeon, next_coord);
    coord_float_t_set_from_float(&monster->coord, next_coord_float);
}

void monster_attacks(ui_t * ui, monster_t * monster, character_t * character,
                     int item_at_character_coord, const char ** item_names) {
    int damage, item_num, monster_broke_item;
    if (
            abs((int) monster->coord.x - character->coord.x) > 1 ||
            abs((int) monster->coord.y - character->coord.y) > 1 ||
            item_at_character_coord == SAFE_PLACE
    ) {
        return;
    }
    damage = monster->type * 0.5;
    sound_sawtooth(ui->audio_state, damage);
    if (damage * 12 < character->attrs[LUCK] + character->attrs[AGILITY]) {
        return;
    }
    draw_message(ui->screen, ui->strings[4]);
    sound_noise(ui->audio_state, damage);
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
    monster_broke_item = rand() % monster->type;
    sound_sawtooth(ui->audio_state, monster->char_code);
    sound_noise(ui->audio_state, monster->char_code);
    if (monster_broke_item == 1) {
        do {
            if (character->inventory[item_num] > 0) {
                monster_breaks_items(
                    ui, item_num, monster->char_code, character, item_names
                );
                item_num = 10;
            }
            item_num += 1;
        } while (item_num < 10);
    }
}

void monsters_turn(ui_t * ui, game_state_t * game_state,
                   int item_at_character_coord, const char ** item_names) {
    monster_list_node_t * monster_list_node =
        game_state->monster_list->first_node;
    monster_t * monster = NULL;
    while (monster_list_node != NULL) {
        monster = monster_list_node->monster;
        monster_moves(
            ui->screen,
            monster,
            game_state->character,
            game_state->dungeon
        );
        monster_attacks(
            ui, monster, game_state->character, item_at_character_coord,
            item_names
        );
        monster_list_node = monster_list_node->next_node;
    }
}

void character_dies(ui_t * ui, char * char_code_hero,
                    character_t * character) {
    int sound_frequency;
    character->facing = 4;
    character->attrs[STRENGTH] = 0;
    draw_message(ui->screen, "");
    tab(ui->screen->cursor, 1, 5);
    free(print_text(ui->screen, "THOU HAST EXPIRED!"));
    for (sound_frequency = 150; sound_frequency >= 1; sound_frequency -= 4) {
        sound_sawtooth(ui->audio_state, sound_frequency);
        sound_noise(ui->audio_state, sound_frequency);
        draw_character_and_stats(ui->screen, char_code_hero, character);
    }
}

void monster_dies(ui_t * ui, game_state_t * game_state, monster_t * monster) {
    coord_t coord;
    coord_t_set_from_float(&coord, monster->coord);
    int sound_frequency;
    monster_list_remove(game_state->monster_list, monster);
    free(monster);
    dungeon_t_set_item(game_state->dungeon, coord, BLANK);
    game_state->character->attrs[EXPERIENCE] += 0.1;
    draw_message(ui->screen, ui->strings[5]);
    for (sound_frequency = 200; sound_frequency >= 150; sound_frequency -= 8) {
        sound_sawtooth(ui->audio_state, sound_frequency);
        sound_noise(ui->audio_state, sound_frequency);
    }
    render_coord(ui->screen, game_state->dungeon, coord);
}

void attack_monster(ui_t * ui, game_state_t * game_state,
                    monster_t * monster) {
    int damage;
    /*
    The original code did not define sound_frequency before calling the
    subroutine at line 360 here. Since the value of sound_frequency is not
    easily predicted, we just use 100.
    */
    sound_noise(ui->audio_state, 100);
    game_state->character->attrs[EXPERIENCE] += 0.05;
    if (
            game_state->character->attrs[AGILITY] +
            game_state->character->attrs[LUCK] <
                rand() % monster->type + 2
    ) {
        draw_message(ui->screen, ui->strings[3]);
        return;
    }
    damage =
        game_state->character->attrs[STRENGTH] +
        game_state->character->inventory[TWO_HAND_SWORD] +
        game_state->character->inventory[BROADSWORD] +
        game_state->character->inventory[SHORTSWORD] +
        game_state->character->inventory[AXE] +
        game_state->character->inventory[MACE] +
        game_state->character->inventory[FLAIL] +
        game_state->character->inventory[DAGGER] +
        game_state->character->inventory[GAUNTLET] + (
            rand() * game_state->character->attrs[LUCK] / RAND_MAX
        );
    monster->strength -= damage;
    draw_message(ui->screen, ui->strings[rand() % 3]);
    game_state->character->attrs[STRENGTH] -= damage / 100;
    game_state->character->attrs[EXPERIENCE] += 0.05;
    if (monster->strength < 1) {
        monster_dies(ui, game_state, monster);
    }
}

void cast_superzap(ui_t * ui, game_state_t * game_state) {
    int sound_frequency;
    monster_t * monster;
    for (sound_frequency = 1; sound_frequency <= 12; sound_frequency += 1) {
        sound_sawtooth(ui->audio_state, sound_frequency);
        sound_noise(ui->audio_state, sound_frequency);
    }
    if (game_state->monster_list->first_node != NULL) {
        monster = monster_list_get_nearest(
            game_state->monster_list, game_state->character->coord
        );
        monster_dies(ui, game_state, monster);
    }
}

void cast_sanctuary(audio_state_t * audio_state, game_state_t * game_state,
                    int item_at_character_coord) {
    if (item_at_character_coord == BLANK) {
        dungeon_t_set_item(
            game_state->dungeon, game_state->character->coord, SAFE_PLACE
        );
    }
    sound_sawtooth(audio_state, 100);
    sound_sawtooth(audio_state, 200);
}

void cast_teleport(ui_t * ui, char * char_code_hero, character_t * character) {
    int sound_frequency;
    character->coord.x = rand() % 13;
    character->coord.y = rand() % 13;
    for (sound_frequency = 0; sound_frequency <= 255; sound_frequency += 8) {
        sound_noise(ui->audio_state, sound_frequency);
        sound_sawtooth(ui->audio_state, sound_frequency);
    }
    draw_character_and_stats(ui->screen, char_code_hero, character);
}

void cast_powersurge(character_t * character, int spell_number) {
    character->attrs[VITALITY] +=
        rand() % character->spells_remaining[spell_number];
    character->attrs[STRENGTH] +=
        rand() % character->spells_remaining[spell_number];
    character->attrs[AURA] -= 1;
}

void cast_metamorphosis(ui_t * ui, game_state_t * game_state,
                        int item_at_character_coord) {
    int sound_frequency;
    monster_t * monster;
    for (sound_frequency = 1; sound_frequency <= 30; sound_frequency += 1) {
        dungeon_t_set_item(
            game_state->dungeon, game_state->character->coord,
            rand() % 8 + 1 + BLANK
        );
        sound_sawtooth(ui->audio_state, sound_frequency);
        render_coord(
            ui->screen, game_state->dungeon, game_state->character->coord
        );
        SDL_RenderPresent(ui->screen->ren);
        SDL_Delay(300);
    }
    if (item_at_character_coord <= SAFE_PLACE) {
        monster = monster_list_get_nearest(
            game_state->monster_list, game_state->character->coord
        );
        if (monster != NULL) {
            monster_list_remove(game_state->monster_list, monster);
        }
    }
    for (sound_frequency = 1; sound_frequency <= 20; sound_frequency += 4) {
        sound_sawtooth(ui->audio_state, sound_frequency);
    }
}

void cast_healing(character_t * character) {
    character->attrs[VITALITY] = character->initial_vitality;
    character->attrs[STRENGTH] = character->initial_strength;
    character->attrs[AURA] -= 1;
}

void cast_spell(ui_t * ui, char * char_code_hero, game_state_t * game_state,
                int item_at_character_coord) {
    int spell_number;
    char pressed_key;
    const char * message;
    draw_character_and_stats(
        ui->screen, char_code_hero, game_state->character
    );
    paper(ui->screen->cursor, YELLOW);
    ink(ui->screen->cursor, BLACK);
    tab(ui->screen->cursor, 0, 1);
    free(print_text(ui->screen, "YOU MAY USE MAGICKS"));
    if (game_state->character->inventory[NECRONOMICON] > 0) {
        tab(ui->screen->cursor, 0, 2);
        free(print_text(ui->screen, "FROM NECRONOMICON"));
    }
    if (game_state->character->inventory[SCROLLS] > 0) {
        tab(ui->screen->cursor, 0, 3);
        free(print_text(ui->screen, "FROM THE SCROLLS"));
    }
    tab(ui->screen->cursor, 0, 4);
    free(print_text(ui->screen, "CONSULT THE LORE"));
    do {
        pressed_key = get_keyboard_input(ui->screen, "USE SPELL NUMBER?");
        spell_number = atoi(&pressed_key) - 1;
    } while (
            spell_number == -1 || (
                game_state->character->inventory[NECRONOMICON] == 0 &&
                spell_number < 3
            ) || (
                game_state->character->inventory[SCROLLS] == 0 &&
                spell_number > 2
            ) || spell_number > 5
    );

    game_state->character->spells_remaining[spell_number] -= 1;
    if (game_state->character->spells_remaining[spell_number] < 0) {
        message = ui->strings[8];
        spell_number = 6;
    } else {
        message = "";
    }
    SDL_Rect rect = {
        .x = 0,
        .y = 8 * ui->screen->zoom,
        .w = ui->screen_cols * 8 * ui->screen->zoom,
        .h = 5 * 8 * ui->screen->zoom
    };
    clear_rect(ui->screen, &rect, YELLOW);
    switch (spell_number) {
        case 0:
            cast_superzap(ui, game_state);
            break;
        case 1:
            cast_sanctuary(
                ui->audio_state, game_state, item_at_character_coord
            );
            break;
        case 2:
            cast_teleport(ui, char_code_hero, game_state->character);
            break;
        case 3:
            cast_powersurge(game_state->character, spell_number);
            break;
        case 4:
            cast_metamorphosis(ui, game_state, item_at_character_coord);
            break;
        case 5:
            cast_healing(game_state->character);
            break;
        case 6:
            break;
    }
    game_state->character->attrs[EXPERIENCE] += 0.2;
    draw_message(ui->screen, message);
}

int * init_song_notes() {
    int * song_notes = malloc(sizeof(int) * 18);
    if (song_notes == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "song_notes is NULL!");
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

void game_won(ui_t * ui, char * char_code_hero, game_state_t * game_state) {
    int index, direction,
        * song_notes = init_song_notes();
    paper(ui->screen->cursor, YELLOW);
    ink(ui->screen->cursor, RED);
    tab(ui->screen->cursor, 0, 1);
    free(print_text(ui->screen, " THY QUEST IS OVER! "));
    for (index = 0; index < 18; index += 1) {
        sound_sawtooth(ui->audio_state, song_notes[index]);

        for (direction = 0; direction < 4; direction += 1) {
            game_state->character->facing = direction;
            draw_character_and_stats(
                ui->screen, char_code_hero, game_state->character
            );
            SDL_Delay(300 / 4);
        }
    }
    tab(ui->screen->cursor, 1, 2);
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "outstring is NULL!");
        exit(1);
    }
    sprintf(
        outstring,
        "THY SCORE=%i",
        (int) (
            (game_state->character->treasure * 10) + (
                game_state->character->gold *
                game_state->character->attrs[EXPERIENCE]
            ) + game_state->character->attrs[STRENGTH] +
            game_state->character->attrs[VITALITY] +
            game_state->character->attrs[AGILITY]
        )
    );
    free(print_text(ui->screen, outstring));
    free(outstring);
    free(song_notes);
    game_state->finished = 1;
}

void get_item(ui_t * ui, int ** vertices, char * char_code_hero,
              game_state_t * game_state) {
    int item_to_get;
    coord_t item_to_get_coord = {
        .x = game_state->character->coord.x + vertices[
            game_state->character->facing
        ][0],
        .y = game_state->character->coord.y + vertices[
            game_state->character->facing
        ][1]
    };
    if (item_to_get_coord.x < 0) {
        item_to_get_coord.x = 0;
    }
    if (item_to_get_coord.y < 0) {
        item_to_get_coord.y = 0;
    }
    if (item_to_get_coord.x > 14) {
        item_to_get_coord.x = 14;
    }
    if (item_to_get_coord.y > 14) {
        item_to_get_coord.y = 14;
    }
    item_to_get = dungeon_t_get_item(game_state->dungeon, item_to_get_coord);
    if (item_to_get > WALL && item_to_get < IDOL) {
        dungeon_t_set_item(game_state->dungeon, item_to_get_coord, BLANK);
    }
    if (item_to_get == VASE) {
        game_state->character->inventory[HEALING_SALVE] += 1;
        game_state->character->inventory[POTIONS] += 1;
    }
    if (item_to_get == CHEST) {
        game_state->character->treasure += 1;
    }
    if (item_to_get == IDOL) {
        game_won(ui, char_code_hero, game_state);
    }
    render_coord(ui->screen, game_state->dungeon, item_to_get_coord);
    if (item_to_get > WALL && item_to_get < IDOL) {
        sound_sawtooth(ui->audio_state, item_to_get);
        sound_sawtooth(ui->audio_state, item_to_get + 5);
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

void light_torch(ui_t * ui, game_state_t * game_state) {
    coord_t coord;
    if (game_state->character->torches == 0) {
        draw_message(ui->screen, ui->strings[6]);
        return;
    }
    for (
            coord.y = game_state->character->coord.y - 3;
            coord.y <= game_state->character->coord.y + 3;
            coord.y += 1
    ) {
        for (
                coord.x = game_state->character->coord.x - 3;
                coord.x <= game_state->character->coord.x + 3;
                coord.x += 1
        ) {
            if (coord.x >= 0 && coord.x < 15 && coord.y >= 0 && coord.y < 15) {
                render_coord_and_check_for_monster(
                    ui->screen, game_state, coord
                );
            }
        }
    }
    game_state->character->torches -= 1;
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
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "outstring is NULL!");
        exit(1);
    }
    sprintf(outstring, "FOR LEVEL %d", (int) character->attrs[EXPERIENCE]);
    free(print_text(screen, outstring));
    free(outstring);
}


void draw_interface(ui_t * ui, character_t * character) {
    paper(ui->screen->cursor, RED);
    clear_screen(ui->screen);
    draw_box(ui->screen, 0, 0, 1, ui->screen_cols, WHITE);
    tab(ui->screen->cursor, 0, 0);
    paper(ui->screen->cursor, WHITE);
    ink(ui->screen->cursor, BLACK);
    free(print_text(ui->screen, character->name));
    draw_box(ui->screen, 1, 0, 5, ui->screen_cols, YELLOW);
    draw_box(ui->screen, 6, 1, 15, 15, BLACK);
    paper(ui->screen->cursor, RED);
    ink(ui->screen->cursor, WHITE);
    tab(ui->screen->cursor, 16, 7);
    free(print_text(ui->screen, "STR"));
    tab(ui->screen->cursor, 16, 10);
    free(print_text(ui->screen, "VIT"));
    tab(ui->screen->cursor, 16, 13);
    free(print_text(ui->screen, "AUR"));
    tab(ui->screen->cursor, 16, 16);
    free(print_text(ui->screen, "FACE"));
    tab(ui->screen->cursor, 16, 19);
    free(print_text(ui->screen, "EXP"));
}

int load_level(ui_t * ui, int skip_first_exp_check,
               game_state_t * game_state) {
    int correct_level_loaded;
    coord_t entrance_coord;
    do {

        if (
                !skip_first_exp_check &&
                game_state->character->attrs[EXPERIENCE] <
                    game_state->character->initial_experience + 1
        ) {
            draw_message(ui->screen, ui->strings[10]);
            return 0;
        }
        skip_first_exp_check = 0;
        clear_screen(ui->screen);
        tab(ui->screen->cursor, 0, 3);
        free(print_text(ui->screen, "PREPARE DUNGEON TAPE"));
        get_keyboard_input(ui->screen, ui->strings[9]);
        size_t filesize;
        char * level_dir = get_level_dir();
        char * level_path = malloc(sizeof(char) * (strlen(level_dir) + 7));
        sprintf(level_path, "%s%cLEVEL", level_dir, PATHSEP);
        free(level_dir);
        errno = 0;
        FILE * file_handle = fopen(level_path, "r");
        if (file_handle == NULL) {
            SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "file_handle is NULL!");
            SDL_LogCritical(
                SDL_LOG_CATEGORY_SYSTEM,
                "Error: %d (%s).",
                errno,
                strerror(errno)
            );
            exit(1);
        }
        free(level_path);

        fseek(file_handle, 0, SEEK_END);
        // TODO: Check for fseek errors.
        filesize = ftell(file_handle);
        fseek(file_handle, 0, SEEK_SET);
        char * file_contents = (char *) malloc(sizeof(char) * (filesize + 1));
        if (file_contents == NULL) {
            SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "file_contents is NULL!");
            exit(1);
        }
        fread(file_contents, 1, filesize, file_handle);
        fclose(file_handle);
        dungeon_t_deserialize(
            game_state->dungeon, file_contents, &entrance_coord
        );
        if (
                game_state->dungeon->level_num >
                    game_state->character->attrs[EXPERIENCE]
        ) {
            show_level_too_deep_messages(ui->screen, game_state->character);
            correct_level_loaded = 1;
        } else {
            correct_level_loaded = 0;
        }
        free(file_contents);
    } while (correct_level_loaded);
    draw_interface(ui, game_state->character);
    coord_t_set(&game_state->character->coord, entrance_coord);
    monster_list_clear(game_state->monster_list);
    return 1;
}

int load_level_with_first_exp_check(ui_t * ui, game_state_t * game_state) {
    return load_level(ui, 0, game_state);
}

int load_level_wo_first_exp_check(ui_t * ui, game_state_t * game_state) {
    return load_level(ui, 1, game_state);
}

void load_character(ui_t * ui, character_t * character, int * num_item_types) {
    int index, subindex, file_index;
    clear_screen(ui->screen);
    tab(ui->screen->cursor, 0, 3);
    free(print_text(ui->screen, "PREPARE HERO TAPE"));
    get_keyboard_input(ui->screen, ui->strings[9]);
    char * character_dir = get_character_dir();
    char * character_path = malloc(sizeof(char) * (strlen(character_dir) + 6));
    sprintf(character_path, "%s%cHERO", character_dir, PATHSEP);
    free(character_dir);
    errno = 0;
    FILE * file_handle = fopen(character_path, "r");
    if (file_handle == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "file_handle is NULL!");
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM,
            "Error: %d (%s).",
            errno,
            strerror(errno)
        );
        exit(1);
    }
    free(character_path);
    size_t filesize;
    fseek(file_handle, 0, SEEK_END);
    // TODO: Check for fseek errors.
    filesize = ftell(file_handle);
    fseek(file_handle, 0, SEEK_SET);
    char * file_contents = (char *) malloc(sizeof(char) * (filesize + 1));
    if (file_contents == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "file_contents is NULL!");
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
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "character->name is NULL!");
        exit(1);
    }
    strcpy(character->name, file_contents + file_index + 1);
    character->initial_strength = character->attrs[STRENGTH];
    character->initial_vitality = character->attrs[VITALITY];
    character->initial_experience = character->attrs[EXPERIENCE];
    for (index = 0; index < 2; index += 1) {
        for (subindex = 0; subindex < 3; subindex += 1) {
            character->spells_remaining[index * 3 + subindex] =
                character->inventory[NECRONOMICON + index] *
                character->attrs[AURA];
        }
    }
    if (character->inventory[TORCH] == 1) {
        character->torches = 20;
    }
    free(file_contents);
}

void save_game(screen_t * screen, game_state_t * game_state,
               int num_item_types) {
    int index;
    draw_message(screen, "ONE MOMENT PLEASE");
    char * character_file_contents = (char *) malloc(
            sizeof(char) * (
                12 + num_item_types + strlen(game_state->character->name)
            )
        );
    if (character_file_contents == NULL) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM,
            "character_file_contents is NULL!"
        );
        exit(1);
    }
    char * dungeon_file_contents = dungeon_t_serialize(
            game_state->dungeon, game_state->character->coord
        );
    int s_index = 0;
    character_file_contents[s_index] =
        (char) (CHARACTER_BASE + num_item_types);
    s_index += 1;
    for (index = 0; index < 8; index += 1) {
        character_file_contents[s_index] = (char) (
            game_state->character->attrs[index] + CHARACTER_BASE
        );
        s_index += 1;
    }
    for (index = 0; index < num_item_types; index += 1) {
        character_file_contents[s_index] = (char) (
            game_state->character->inventory[index] + CHARACTER_BASE
        );
        s_index += 1;
    }
    character_file_contents[s_index] = (char) (
        game_state->character->gold + CHARACTER_BASE
    );
    s_index += 1;
    character_file_contents[s_index] = (char) (
        game_state->character->treasure + CHARACTER_BASE
    );
    s_index += 1;
    strcpy(character_file_contents + s_index, game_state->character->name);
    s_index += strlen(game_state->character->name);
    character_file_contents[s_index] = 0;
    get_keyboard_input(screen, "ANY KEY TO SAVE");
    char * character_dir = get_character_dir();
    char * character_path = malloc(sizeof(char) * (strlen(character_dir) + 6));
    sprintf(character_path, "%s%cHERO", character_dir, PATHSEP);
    free(character_dir);
    errno = 0;
    FILE * file_handle = fopen(character_path, "w");
    if (file_handle == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "file_handle is NULL!");
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM,
            "Error: %d (%s).",
            errno,
            strerror(errno)
        );
        exit(1);
    }
    free(character_path);
    int error = fputs(character_file_contents, file_handle);
    if (error < 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM,
            "Error %i writing the character!",
            ferror(file_handle)
        );
    }
    error = fclose(file_handle);
    if (error) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM,
            "Error %i closing the character!",
            error
        );
    }
    char * level_dir = get_level_dir();
    char * level_path = malloc(sizeof(char) * (strlen(level_dir) + 7));
    sprintf(level_path, "%s%cLEVEL", level_dir, PATHSEP);
    free(level_dir);
    errno = 0;
    file_handle = fopen(level_path, "w");
    if (file_handle == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "file_handle is NULL!");
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM,
            "Error: %d (%s).",
            errno,
            strerror(errno)
        );
        exit(1);
    }
    free(level_path);
    error = fputs(dungeon_file_contents, file_handle);
    if (error < 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM,
            "Error %i writing the level!",
            ferror(file_handle)
        );
    }
    error = fclose(file_handle);
    if (error) {
        SDL_LogError(
            SDL_LOG_CATEGORY_SYSTEM,
            "Error %i closing the level!",
            error
        );
    }
    game_state->finished = 1;
}

audio_state_t * init_audio() {
    audio_state_t * audio_state;
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_AUDIO,
            "SDL Init Failure!: %s",
            SDL_GetError()
        );
        exit(1);
    }
    audio_state = init_audio_state(2);

    return audio_state;
}


int ** init_vertices() {
    int i, ** vertices;
    vertices = (int **) malloc(sizeof(int *) * 4);
    if (vertices == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "vertices is NULL!");
        exit(1);
    }
    for (i = 0; i < 4; i += 1) {
        vertices[i] = (int *) malloc(sizeof(int) * 3);
        if (vertices[i] == NULL) {
            SDL_LogCritical(
                SDL_LOG_CATEGORY_SYSTEM,
                "vertices[%i] is NULL!",
                i
            );
            exit(1);
        }
    }

    vertices[0][0] = 0;
    vertices[0][1] = -1;
    vertices[1][0] = 1;
    vertices[1][1] = 0;
    vertices[2][0] = 0;
    vertices[2][1] = 1;
    vertices[3][0] = -1;
    vertices[3][1] = 0;

    return vertices;
}

char * init_char_code_hero() {
    int index;
    char * char_code_hero;
    char_code_hero = (char *) malloc(sizeof(char) * 6);
    if (char_code_hero == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "char_code_hero is NULL!");
        exit(1);
    }
    for (index = 0; index < 5; index += 1) {
        char_code_hero[index] = index + 1;
    }
    char_code_hero[5] = 0;

    return char_code_hero;
}

character_t * init_character() {
    character_t * character = malloc(sizeof(character_t));
    if (character == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "character is NULL!");
        exit(1);
    }
    character->attrs = (double *) malloc(sizeof(double) * 9);
    if (character->attrs == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "character->attrs is NULL!");
        exit(1);
    }
    character->spells_remaining = (int *) malloc(sizeof(int) * 6);
    if (character->spells_remaining == NULL) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM,
            "character->spells_remaining is NULL!"
        );
        exit(1);
    }

    character->facing = 3;
    character->coord.x = 0;
    character->coord.y = 0;
    character->torches = 0;

    return character;
}

dungeon_t * init_dungeon() {
    dungeon_t * dungeon = malloc(sizeof(dungeon_t));
    if (dungeon == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "dungeon is NULL!");
        exit(1);
    }
    return dungeon;
}

const char ** init_strings() {
    const char ** strings = malloc(sizeof(const char *) * 12);
    if (strings == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "strings is NULL!");
        exit(1);
    }

    strings[0] = "A GOOD BLOW";
    strings[1] = "WELL HIT SIRE";
    strings[2] = "THY AIM IS TRUE";
    strings[3] = "MISSED!";
    strings[4] = "HIT THEE!!";
    strings[5] = "THE MONSTER IS SLAIN";
    strings[6] = "NO LIGHT";
    strings[7] = "BROKEN THY ";
    strings[8] = "SPELL EXHAUSTED";
    strings[9] = "PRESS ANY KEY";
    strings[10] = "YOU NEED EXPERIENCE";
    strings[11] = "EXIT FROM THIS LEVEL";

    return strings;
}

const char ** init_item_names() {
    const char ** item_names = malloc(sizeof(const char *) * (11));
    if (item_names == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "item_names is NULL!");
        exit(1);
    }
    item_names[0] = "GR SWORD";
    item_names[1] = "SWORD";
    item_names[2] = "AXE";
    item_names[3] = "MACE";
    item_names[4] = "FLAIL";
    item_names[5] = "DAGGER";
    item_names[6] = "ARMOUR";
    item_names[7] = "ARMOUR";
    item_names[8] = "ARMOUR";
    item_names[9] = "HELMET";
    item_names[10] = "HEADPC.";

    return item_names;
}

int main(int argc, char * argv[]) {
    handle_args(argc, argv);
    int ** vertices,
        num_item_types,
        item_at_character_coord,  // Object at character->coord.x
        trapped;  // Flag to see if we can exit.
    coord_t trap_coord, prev_coord;
    char pressed_key,
         * char_code_hero = NULL;
    const char ** item_names;
    game_state_t * game_state = malloc(sizeof(game_state_t));
    game_state->monster_list = monster_list_init();
    monster_t * monster;
    ui_t * ui = malloc(sizeof(ui_t));
    *ui = (ui_t) {
        .screen = init_screen(),
        .audio_state = init_audio(),
        .strings = init_strings(),
        .screen_cols = 40
    };
    paper(ui->screen->cursor, YELLOW);
    ink(ui->screen->cursor, BLACK);
    clear_screen(ui->screen);
    vertices = init_vertices();
    char_code_hero = init_char_code_hero();
    game_state->character = init_character();
    game_state->dungeon = init_dungeon();
    item_names = init_item_names();

    game_state->finished = 0;
    trap_coord.x = 0;
    trap_coord.y = 0;
    trapped = 0;

    load_character(ui, game_state->character, &num_item_types);
    load_level_wo_first_exp_check(ui, game_state);
    coord_t_set(&prev_coord, game_state->character->coord);
    int game_over = 0;
    do {
        SDL_RenderPresent(ui->screen->ren);
        pressed_key = inkey$();
        if (
                pressed_key == 'a' &&
                game_state->monster_list->first_node != NULL
        ) {
            monster = monster_list_get_nearest(
                game_state->monster_list, game_state->character->coord
            );
            if (monster != NULL) {
                attack_monster(ui, game_state, monster);
            }
        }
        if (
                pressed_key == 'c' &&
                game_state->character->attrs[AURA] > 0 &&
                game_state->character->inventory[NECRONOMICON] +
                    game_state->character->inventory[SCROLLS] > 0
        ) {
            cast_spell(
                ui, char_code_hero, game_state, item_at_character_coord
            );
        }
        if (pressed_key == 'g') {
            get_item(ui, vertices, char_code_hero, game_state);
        }
        if (pressed_key == 'p') {
            drink_potion(game_state->character);
        }
        if (pressed_key == 'r') {
            light_torch(ui, game_state);
        }
        if (pressed_key == 's') {
            save_game(ui->screen, game_state, num_item_types);
        }
        if (pressed_key == 'b') {
            game_state->character->facing -= 1;
        }
        if (pressed_key == 'n') {
            game_state->character->facing += 1;
        }
        if (game_state->character->facing > 3) {
            game_state->character->facing = 0;
        }
        if (game_state->character->facing < 0) {
            game_state->character->facing = 3;
        }
        if (pressed_key == 'm') {
            game_state->character->coord.x += vertices[
                game_state->character->facing
            ][0];
            game_state->character->coord.y += vertices[
                game_state->character->facing
            ][1];
        }
        if (game_state->character->coord.y > 14) {
            game_state->character->coord.y = 14;
        }
        if (game_state->character->coord.y < 0) {
            game_state->character->coord.y = 0;
        }
        if (game_state->character->coord.x < 0) {
            game_state->character->coord.x = 0;
        }
        if (game_state->character->coord.x > 14) {
            game_state->character->coord.x = 14;
        }
        item_at_character_coord = dungeon_t_get_item(
            game_state->dungeon, game_state->character->coord
        );
        if (item_at_character_coord == WALL) {
            render_coord(
                ui->screen, game_state->dungeon, game_state->character->coord
            );
            coord_t_set(&game_state->character->coord, prev_coord);
            game_state->character->attrs[STRENGTH] -= 0.03;
        }
        if (item_at_character_coord == TRAP) {
            coord_t_set(&trap_coord, game_state->character->coord);
            trapped = 1;
        }
        if (trapped == 1) {
            coord_t_set(&game_state->character->coord, trap_coord);
        }
        if (
                game_state->character->attrs[STRENGTH] >
                    game_state->character->initial_strength * 0.8 &&
                rand() % 8 < game_state->character->attrs[LUCK]
        ) {
            trapped = 0;
        }
        if (pressed_key != 0) {
            game_state->character->attrs[STRENGTH] =
                game_state->character->attrs[STRENGTH] * 0.99;
        }
        if (
                game_state->character->attrs[STRENGTH] <
                    game_state->character->initial_strength
        ) {
            game_state->character->attrs[STRENGTH] +=
                game_state->character->attrs[VITALITY] / 1100;
        }
        draw_character_and_stats(
            ui->screen, char_code_hero, game_state->character
        );
        if (coord_t_is_equal(prev_coord, game_state->character->coord) != 1) {
            render_coord_and_check_for_monster(
                ui->screen, game_state, prev_coord
            );
        }
        coord_t_set(&prev_coord, game_state->character->coord);
        if (game_state->monster_list->first_node != NULL) {
            monsters_turn(ui, game_state, item_at_character_coord, item_names);
        }
        if (
                game_state->character->attrs[STRENGTH] > 0 &&
                game_state->finished < 1 &&
                item_at_character_coord != EXIT
        ) {
            game_over = 0;
        } else if (item_at_character_coord == EXIT) {
            draw_message(ui->screen, ui->strings[11]);
            if (load_level_with_first_exp_check(ui, game_state)) {
                coord_t_set(&prev_coord, game_state->character->coord);
            } else {
                coord_t_set(&game_state->character->coord, prev_coord);
            }
            game_over = 0;
        } else {
            game_over = 1;
        }
    } while (!game_over);
    if (game_state->character->attrs[STRENGTH] < 1) {
        character_dies(ui, char_code_hero, game_state->character);
    }
    tab(ui->screen->cursor, 0, 10);

    destroy_audio_state(ui->audio_state);
    destroy_screen(ui->screen);
    free(ui->strings);
    free(ui);

    int i;
    free(game_state->character->name);
    free(game_state->character->inventory);
    for (i = 0; i < 4; i += 1) {
        free(vertices[i]);
    }
    free(vertices);
    free(game_state->character->attrs);
    free(char_code_hero);
    free(game_state->character->spells_remaining);
    free(game_state->dungeon);
    free(item_names);
    free(game_state->character);
    monster_list_clear(game_state->monster_list);
    free(game_state->monster_list);
    free(game_state);
    return 0;
}
