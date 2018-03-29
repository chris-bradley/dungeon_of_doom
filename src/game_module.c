#include <SDL.h>
#include <unistd.h>
#include "dungeon_lib.h"

void draw_message(screen_t *screen, char *message, int screen_cols);
void draw_character_and_stats(screen_t *screen, double *attrs,
                              char *char_code_hero, int character_facing,
                              int character_coord_x, int character_coord_y);
void render_coord_and_check_for_monster(screen_t *screen, int char_code_vase,
                                        int char_code_safe_place,
                                        int *distance_to_monster_x,
                                        int *monster_coord_x,
                                        int *monster_coord_y,
                                        int *monster_type,
                                        int *monster_strength,
                                        int *monster_char_code,
                                        int *monster_speed,
                                        int **dungeon_contents, int coord_x,
                                        int coord_y);
void lines620_770(screen_t *screen, int char_code_blank, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength,
                  int *monster_char_code, int *monster_speed,
                  int *monster_next_coord_x, int *monster_next_coord_y,
                  int character_coord_x, int character_coord_y,
                  int inventory[25], int **dungeon_contents,
                  int item_at_character_coord, const char **strings,
                  int screen_cols, const char **item_names);
void lines810_860(screen_t *screen, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, char *char_code_hero, int *monster_coord_x,
                  int *monster_coord_y, int *monster_type,
                  int *monster_strength, int *monster_char_code,
                  int *monster_speed, int *character_facing,
                  int character_coord_x, int character_coord_y,
                  int **dungeon_contents, int screen_cols, int coord_x,
                  int coord_y);
void lines870_930(screen_t *screen, int char_code_blank, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength,
                  int *monster_char_code, int *monster_speed,
                  int monster_next_coord_x, int monster_next_coord_y,
                  int inventory[25], int **dungeon_contents,
                  const char **strings, int screen_cols, int coord_x,
                  int coord_y);
void lines990_1130(screen_t *screen, int char_code_blank, int char_code_vase,
                   int char_code_safe_place, int *distance_to_monster_x,
                   double *attrs, char *char_code_hero, int *monster_coord_x,
                   int *monster_coord_y, int *spells_remaining,
                   int *monster_type, int *monster_strength,
                   int *monster_char_code, int *monster_speed,
                   int monster_next_coord_x, int monster_next_coord_y,
                   int character_facing, int *character_coord_x,
                   int *character_coord_y, int inventory[25],
                   int **dungeon_contents, int item_at_character_coord,
                   double initial_strength, double initial_vitality,
                   const char **strings, int screen_cols);
void lines1410_1520(screen_t *screen, int char_code_blank, int char_code_wall,
                    int char_code_vase, int char_code_chest,
                    int char_code_idol, int char_code_safe_place,
                    int **vertices, int *distance_to_monster_x, double *attrs,
                    char *char_code_hero, int *finished, int gold,
                    int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength,
                    int *monster_char_code, int *monster_speed,
                    int *character_facing, int character_coord_x,
                    int character_coord_y, int inventory[25],
                    int **dungeon_contents, int *song_notes, int *treasure);
void lines1550_1650(screen_t *screen, double *attrs, char *char_code_hero,
                    int *finished, int gold, int *monster_strength,
                    int *character_facing, int character_coord_x,
                    int character_coord_y, int *song_notes, int treasure);
void lines1660_1680(double *attrs, int inventory[25], double initial_strength,
                    double initial_vitality);
void lines1690_1750(screen_t *screen, int char_code_vase,
                    int char_code_safe_place, int *distance_to_monster_x,
                    int *torches, int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength,
                    int *monster_char_code, int *monster_speed,
                    int character_coord_x, int character_coord_y,
                    int **dungeon_contents, const char **strings,
                    int screen_cols);
void lines1760_1950(screen_t *screen, char *character_name,
                    int *distance_to_monster_x, double *attrs,
                    int *dungeon_level, int *character_coord_x,
                    int *character_coord_y, int dungeon_char_base,
                    int *character_prev_coord_x, int *character_prev_coord_y,
                    int **dungeon_contents, double initial_experience,
                    const char **strings, int screen_cols);
void lines1770_1950(screen_t *screen, char *character_name,
                    int *distance_to_monster_x, double *attrs,
                    int *dungeon_level, int *character_coord_x,
                    int *character_coord_y, int dungeon_char_base,
                    int *character_prev_coord_x, int *character_prev_coord_y,
                    int **dungeon_contents, double initial_experience,
                    const char **strings, int screen_cols);
void lines2010_2250(screen_t *screen, int character_char_base,
                    char **character_name, double *attrs, int *gold,
                    int *torches, int *spells_remaining, int inventory[25],
                    int *num_item_types, double *initial_strength,
                    double *initial_vitality, double *initial_experience,
                    const char **strings, int *treasure, int screen_cols);
void lines2260_2490(screen_t *screen, int character_char_base,
                    char *character_name, double *attrs, int *finished,
                    int gold, int dungeon_level, int character_coord_x,
                    int character_coord_y, int inventory[25],
                    int dungeon_char_base, int num_item_types,
                    int **dungeon_contents, int treasure, int screen_cols);
void lines2500_2780(int *character_char_base, int *char_code_blank,
                    int *char_code_wall, int *char_code_vase,
                    int *char_code_chest, int *char_code_idol,
                    int *char_code_exit, int *char_code_trap,
                    int *char_code_safe_place, int ***vertices,
                    int *distance_to_monster_x, double **attrs,
                    char **char_code_hero, int *finished, int *torches,
                    int **spells_remaining, int *monster_next_coord_x,
                    int *monster_next_coord_y, int *character_facing,
                    int *character_coord_x, int *character_coord_y,
                    int *dungeon_char_base, int ***dungeon_contents,
                    int **song_notes, const char ***strings, int *trapped,
                    int *trap_coord_x, int *trap_coord_y, int *screen_cols,
                    const char ***item_names);

int main(int argc, char *argv[]) {
    int character_char_base,
        char_code_blank,
        char_code_wall,  // Symbol for Wall
        char_code_vase,
        char_code_chest,
        char_code_idol,
        char_code_exit,  // Symbol for Exit
        char_code_trap,  // Symbol for Trap
        char_code_safe_place,
        ** vertices,
        distance_to_monster_x,
        finished,
        gold,
        dungeon_level,
        torches,
        monster_coord_x,
        monster_coord_y,
        monster_type,
        * spells_remaining,
        monster_strength,
        monster_char_code,
        monster_speed,
        monster_next_coord_x,
        monster_next_coord_y,
        character_facing,  // Facing. NESW
        character_coord_x,
        character_coord_y,
        inventory[25],
        dungeon_char_base,
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
    // C64: 5 GOSUB 5000:POKE 53281,0
    screen_t *screen = init_screen();
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    clear_screen(screen);
    // lines 5000 on unneeded due to dungeon lib
    // 10 GOSUB2500
    lines2500_2780(
        &character_char_base, &char_code_blank, &char_code_wall,
        &char_code_vase, &char_code_chest, &char_code_idol, &char_code_exit,
        &char_code_trap, &char_code_safe_place, &vertices,
        &distance_to_monster_x, &attrs, &char_code_hero, &finished, &torches,
        &spells_remaining, &monster_next_coord_x, &monster_next_coord_y,
        &character_facing, &character_coord_x, &character_coord_y,
        &dungeon_char_base, &dungeon_contents, &song_notes, &strings, &trapped,
        &trap_coord_x, &trap_coord_y, &screen_cols, &item_names
    );
    // 20 GOSUB2010
    lines2010_2250(
        screen, character_char_base, &character_name, attrs, &gold, &torches,
        spells_remaining, inventory, &num_item_types, &initial_strength,
        &initial_vitality, &initial_experience, strings, &treasure, screen_cols
    );
    // 30 GOSUB1770
    lines1770_1950(
        screen, character_name, &distance_to_monster_x, attrs, &dungeon_level,
        &character_coord_x, &character_coord_y, dungeon_char_base,
        &character_prev_coord_x, &character_prev_coord_y, dungeon_contents,
        initial_experience, strings, screen_cols
    );
    int game_over = 0;
    do {
        SDL_RenderPresent(screen->ren);
    // 40 LET I$=inkey$
        pressed_key = inkey$();
    // 50 IF I$="A" AND DX<255 THEN GOSUB870
        if (pressed_key == 'a' && distance_to_monster_x < 255 ) {
            lines870_930(
                screen, char_code_blank, char_code_vase, char_code_safe_place,
                &distance_to_monster_x, attrs, &monster_coord_x,
                &monster_coord_y, &monster_type, &monster_strength,
                &monster_char_code, &monster_speed, monster_next_coord_x,
                monster_next_coord_y, inventory, dungeon_contents, strings,
                screen_cols, coord_x, coord_y
            );
        }
    // 60 IF I$="C" AND F(7)>0 AND O(17)+O(18)>0 THEN GOSUB990
        if (
                pressed_key == 'c' &&
                attrs[7] > 0 &&
                inventory[17] + inventory[18] > 0
        ) {
            lines990_1130(
                screen, char_code_blank, char_code_vase, char_code_safe_place,
                &distance_to_monster_x, attrs, char_code_hero,
                &monster_coord_x, &monster_coord_y, spells_remaining,
                &monster_type, &monster_strength, &monster_char_code,
                &monster_speed, monster_next_coord_x, monster_next_coord_y,
                character_facing, &character_coord_x, &character_coord_y,
                inventory, dungeon_contents, item_at_character_coord,
                initial_strength, initial_vitality, strings, screen_cols
            );
        }
    // 70 IF I$="G" THEN GOSUB1410
        if (pressed_key == 'g') {
            lines1410_1520(
                screen, char_code_blank, char_code_wall, char_code_vase,
                char_code_chest, char_code_idol, char_code_safe_place,
                vertices, &distance_to_monster_x, attrs, char_code_hero,
                &finished, gold, &monster_coord_x, &monster_coord_y,
                &monster_type, &monster_strength, &monster_char_code,
                &monster_speed, &character_facing, character_coord_x,
                character_coord_y, inventory, dungeon_contents, song_notes,
                &treasure
            );
        }
    // 80 IF I$="P" THEN GOSUB1660
        if (pressed_key == 'p') {
            lines1660_1680(
                attrs, inventory, initial_strength, initial_vitality
            );
        }
    // 90 IF I$="R" THEN GOSUB1690
        if (pressed_key == 'r') {
            lines1690_1750(
                screen, char_code_vase, char_code_safe_place,
                &distance_to_monster_x, &torches, &monster_coord_x,
                &monster_coord_y, &monster_type, &monster_strength,
                &monster_char_code, &monster_speed, character_coord_x,
                character_coord_y, dungeon_contents, strings, screen_cols
            );
        }
    // 100 IF I$="S" THEN GOSUB2260
        if (pressed_key == 's') {
            lines2260_2490(
                screen, character_char_base, character_name, attrs, &finished,
                gold, dungeon_level, character_coord_x, character_coord_y,
                inventory, dungeon_char_base, num_item_types, dungeon_contents,
                treasure, screen_cols
            );
        }
    // 110 IF I$="B" THEN LET NF=NF-1
        if (pressed_key == 'b') {
            character_facing -= 1;
        }
    // 120 IF I$="N" THEN NF=NF+1
        if (pressed_key == 'n') {
            character_facing += 1;
        }
    // 130 IF NF>4 THEN LET NF=1
        if (character_facing > 4) {
            character_facing = 1;
        }
    // 140 IF NF<1 THEN LET NF=4
        if (character_facing < 1) {
            character_facing = 4;
        }
    // 150 IF I$="M" THEN LET NX=NX+D(NF,1): LET NY=NY+D(NF,2)
        if (pressed_key == 'm') {
            character_coord_x += vertices[character_facing][1];
            character_coord_y += vertices[character_facing][2];
        }
    // 160 IF NY>15 THEN LET NY=15
        if (character_coord_y > 15) {
            character_coord_y = 15;
        }
    // 170 IF NY<1 THEN LET NY=1
        if (character_coord_y < 1) {
            character_coord_y = 1;
        }
    // 180 IF NX<1 THEN LET NX=1
        if (character_coord_x <  1) {
            character_coord_x = 1;
        }
    // 190 IF NX>15 THEN LET NX=15
        if (character_coord_x > 15) {
            character_coord_x = 15;
        }
    // 200 LET RH=R(NX,NY)
        item_at_character_coord =
            dungeon_contents[character_coord_x][character_coord_y];
    // 210 IF RH=C1 THEN LET X=NX:LET Y=NY:GOSUB570:LET NX=OX:LET NY=OY:LET F(1)=F(1)-.03
        if (item_at_character_coord == char_code_wall) {
            coord_x = character_coord_x;
            coord_y = character_coord_y;
            render_coord_and_check_for_monster(
                screen, char_code_vase, char_code_safe_place,
                &distance_to_monster_x, &monster_coord_x, &monster_coord_y,
                &monster_type, &monster_strength, &monster_char_code,
                &monster_speed, dungeon_contents, coord_x, coord_y
            );
            character_coord_x = character_prev_coord_x;
            character_coord_y = character_prev_coord_y;
            attrs[1] -= 0.03;
        }
    // 220 IF RH=C6 THEN LET TX=NX:LET TY=NY:LET TF=1
        if (item_at_character_coord == char_code_trap) {
            trap_coord_x = character_coord_x;
            trap_coord_y = character_coord_y;
            trapped = 1;
        }
    // 230 IF TF=1 THEN LET NX=TX:LET NY=TY
        if (trapped == 1) {
            character_coord_x = trap_coord_x;
            character_coord_y = trap_coord_y;
        }
    // 240 IF F(1) > S1*.8 AND rnd(8)<F(6) THEN LET TF=0
        if (attrs[1] > initial_strength * 0.8 && rand() % 8 < attrs[6]) {
            trapped = 0;
        }
    // 250 IF I$>"" THEN LET F(1)=F(1)*0.99
        if (pressed_key != 0) {
            attrs[1] = attrs[1] * 0.99;
        }
    // 260 IF F(1) <S1 THEN LET F(1)=F(1)+(F(2)/1100)
        if (attrs[1] < initial_strength) {
            attrs[1] += attrs[2] / 1100;
        }
    // 270 GOSUB480
        draw_character_and_stats(
            screen, attrs, char_code_hero, character_facing, character_coord_x,
            character_coord_y
        );
    // 280 IF OX<>NX OR OY<>NY THEN LET X=OX:LET Y=OY:GOSUB570
        if (
                character_prev_coord_x != character_coord_x ||
                character_prev_coord_y != character_coord_y
        ) {
            coord_x = character_prev_coord_x;
            coord_y = character_prev_coord_y;
            render_coord_and_check_for_monster(
                screen, char_code_vase, char_code_safe_place,
                &distance_to_monster_x, &monster_coord_x, &monster_coord_y,
                &monster_type, &monster_strength, &monster_char_code,
                &monster_speed, dungeon_contents, coord_x, coord_y
            );
        }
    // 290 LET OX=NX:LET OY=NY
        character_prev_coord_x = character_coord_x;
        character_prev_coord_y = character_coord_y;
    // 300 IF DX<255 THEN GOSUB620
        if (distance_to_monster_x < 255) {
            lines620_770(
                screen, char_code_blank, char_code_vase, char_code_safe_place,
                &distance_to_monster_x, attrs, &monster_coord_x,
                &monster_coord_y, &monster_type, &monster_strength,
                &monster_char_code, &monster_speed, &monster_next_coord_x,
                &monster_next_coord_y, character_coord_x, character_coord_y,
                inventory, dungeon_contents, item_at_character_coord, strings,
                screen_cols, item_names
            );
        }
    // 310 IF F(1)>0 AND FI<1 AND RH<>C5 THEN GOTO 40
        if (
                attrs[1] > 0 && finished < 1 &&
                item_at_character_coord != char_code_exit
        ) {
            game_over = 0;
        }
    // 320 IF RH=C5 THEN LET M$=T$(12):GOSUB430:GOSUB1760:GOTO40
        else if (item_at_character_coord == char_code_exit) {
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
            lines1760_1950(
                screen, character_name, &distance_to_monster_x, attrs,
                &dungeon_level, &character_coord_x, &character_coord_y,
                dungeon_char_base, &character_prev_coord_x,
                &character_prev_coord_y, dungeon_contents, initial_experience,
                strings, screen_cols
            );
            game_over = 0;
        } else {
            game_over = 1;
        }
    } while (!game_over);
    // 330 IF F(1)<1 THEN GOSUB810
    if (attrs[1] < 1) {
        lines810_860(
            screen, char_code_vase, char_code_safe_place,
            &distance_to_monster_x, attrs, char_code_hero, &monster_coord_x,
            &monster_coord_y, &monster_type, &monster_strength,
            &monster_char_code, &monster_speed, &character_facing,
            character_coord_x, character_coord_y, dungeon_contents,
            screen_cols, coord_x, coord_y
        );
    }
    // 340 PRINT tab(0,10);:STOP
    tab(screen->cursor, 0, 10);
    destroy_screen(screen);

    int i;
    free(character_name);
    for (i = 0; i < 3; i += 1) {
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
    return 0;
}

void sound_sawtooth(int sound_frequency) {
    // C64:
    // 350 POKE VS+1,J:POKE VS+4,33
    // 355 POKE VS+4,32:RETURN

    // TODO: SOUND!
}

void sound_noise(int sound_frequency) {
    // C64:
    // 360 POKE VS+8,J:POKE VS+11,129
    // 365 POKE VS+11,128:RETURN

    // TODO: SOUND!
}

void get_keyboard_input(screen_t *screen, char *pressed_key, char *message,
                        int screen_cols) {
    // 370 paper 2:ink 0
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    // 380 PRINT tab(0,5):M$;
    tab(screen->cursor, 0, 5);
    free(print_text(screen, message));
    // 390 LET I$=inkey$
    // 400 IF I$="" THEN GOTO390
    SDL_RenderPresent(screen->ren);
    *pressed_key = inkey$();
    // 410 PRINT tab(0,5);LEFT(B$, W);:LET M$=""
    tab(screen->cursor, 0, 5);
    print_left$_b$(screen, screen_cols);
    // To decrease variable scope, we clear M$ elsewhere.
    // 420 RETURN
}

void draw_message_wo_colour_change(screen_t *screen, char *message,
                                   int screen_cols);

void draw_message(screen_t *screen, char *message, int screen_cols) {
    // 430 paper 2:ink 0
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    draw_message_wo_colour_change(screen, message, screen_cols);
}

void draw_message_wo_colour_change(screen_t *screen, char *message,
                                   int screen_cols) {
    // 440 PRINT tab(0,5);M$;
    tab(screen->cursor, 0, 5);
    free(print_text(screen, message));
    SDL_RenderPresent(screen->ren);
    // 450 FOR D=1 TO 600:NEXT D
    // C64: 450 FOR D=1 TO 200:NEXT D
    sleep(0.34);
    // 460 PRINT tab(0,5);LEFT$(B$,W);:LET M$=""
    tab(screen->cursor, 0, 5);
    print_left$_b$(screen, screen_cols);
    // To decrease variable scope, we clear M$ elsewhere.
    // 470 RETURN
}

void draw_character_and_stats(screen_t *screen, double *attrs,
                              char *char_code_hero, int character_facing,
                              int character_coord_x, int character_coord_y) {
    // 480 paper 1:ink 3
    paper(screen->cursor, RED);
    ink(screen->cursor, WHITE);
    // 490 PRINT tab(NX,NY+5);MID$(F$,NF,1);
    tab(screen->cursor, character_coord_x, character_coord_y + 5);
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    outstring[0] = char_code_hero[character_facing];
    outstring[1] = 0;
    free(print_text(screen, outstring));
    // 500 paper 2:ink 0
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    // 510 PRINT tab(16,8);INT(F(1));" ";
    tab(screen->cursor, 16, 8);
    sprintf(outstring, "%i ", (int) attrs[1]);
    free(print_text(screen, outstring));

    // 520 PRINT tab(16,11);INT(F(2));" ";
    tab(screen->cursor, 16, 11);
    sprintf(outstring, "%i ", (int) attrs[2]);
    free(print_text(screen, outstring));
    // 530 PRINT tab(16,14);INT (F(7));" ";
    tab(screen->cursor, 16, 14);
    sprintf(outstring, "%i ", (int) attrs[7]);
    free(print_text(screen, outstring));
    // 540 PRINT tab(16,17);MID$("NESW.",NF,1);
    tab(screen->cursor, 16, 17);
    outstring[0] = "NESW."[character_facing - 1];
    outstring[1] = 0;
    free(print_text(screen, outstring));
    // 550 PRINT tab(16,20);INT(F(5));
    tab(screen->cursor, 16, 20);
    sprintf(outstring, "%i ", (int) attrs[5]);
    free(print_text(screen, outstring));
    // 560 RETURN
    free(outstring);
    SDL_RenderPresent(screen->ren);
}

void render_coord_and_check_for_monster(screen_t *screen, int char_code_vase,
                                        int char_code_safe_place,
                                        int *distance_to_monster_x,
                                        int *monster_coord_x,
                                        int *monster_coord_y,
                                        int *monster_type,
                                        int *monster_strength,
                                        int *monster_char_code,
                                        int *monster_speed,
                                        int **dungeon_contents, int coord_x,
                                        int coord_y) {
    int item_at_coord;
    // 570 paper 1:ink 2
    paper(screen->cursor, RED);
    ink(screen->cursor, YELLOW);
    // 580 LET RM=R(X,Y):PRINT tab(X,Y+5);CHR$(RM);
    item_at_coord = dungeon_contents[coord_x][coord_y];
    tab(screen->cursor, coord_x, coord_y + 5);
    char * outstring = (char *) malloc(sizeof(char) * 2);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    sprintf(outstring, "%c", (char) item_at_coord);
    free(print_text(screen, outstring));
    free(outstring);
    // 590 IF ABS(DX)<4 OR RM<=C7 THEN RETURN
    if (
            abs(*distance_to_monster_x) < 4 ||
            item_at_coord <= char_code_safe_place
    ) {
        return;
    }
    // 600 LET MT=RM:LET M=MT-C2:LET MV=M/16:LET MS=M*6:LET DX=3:LET LX=X:LET LY=Y
    *monster_char_code = item_at_coord;
    *monster_type = *monster_char_code - char_code_vase;
    *monster_speed = *monster_type / 16;
    *monster_strength = *monster_type * 6;
    *distance_to_monster_x = 3;
    *monster_coord_x = coord_x;
    *monster_coord_y = coord_y;
    // 610 RETURN
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

void lines780_800(screen_t *screen, int item_num, int sound_frequency,
                  int *monster_broke_item, int inventory[25], \
                  const char **strings, int screen_cols,
                  const char **item_names);

void lines620_770(screen_t *screen, int char_code_blank, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength,
                  int *monster_char_code, int *monster_speed,
                  int *monster_next_coord_x, int *monster_next_coord_y,
                  int character_coord_x, int character_coord_y,
                  int inventory[25], int **dungeon_contents,
                  int item_at_character_coord, const char **strings,
                  int screen_cols, const char **item_names) {
    int distance_to_monster_y, damage, item_num, sound_frequency,
        monster_broke_item, item_at_monster_next_coord, direction_to_monster_x,
        direction_to_monster_y, coord_x, coord_y;
    char * message;
    // 620 LET DX=LX-NX:LET SX=SGN(DX):LET DY=LY-NY:LET SY=SGN(DY)
    *distance_to_monster_x = *monster_coord_x - character_coord_x;
    direction_to_monster_x = sign(*distance_to_monster_x);
    distance_to_monster_y = *monster_coord_y - character_coord_y;
    direction_to_monster_y = sign(distance_to_monster_y);
    // 630 LET MX=LX-(MV*SX):LET MY=LY-(MV*SY):LET RM=R(MX,MY)
    *monster_next_coord_x =
        *monster_coord_x - (*monster_speed * direction_to_monster_x);
    *monster_next_coord_y =
        *monster_coord_y - (*monster_speed * direction_to_monster_y);
    item_at_monster_next_coord =
        dungeon_contents[*monster_next_coord_x][*monster_next_coord_y];
    // 640 IF RM>C0 AND RM<>MT THEN LET MY=LY:LET MX=LX
    if (
            item_at_monster_next_coord > char_code_blank &&
            item_at_monster_next_coord != *monster_char_code
    ) {
        *monster_next_coord_y = *monster_coord_y;
        *monster_next_coord_x = *monster_coord_x;
    }
    // 650 LET R(LX,LY)=C0:LET X=LX:LET Y=LY:GOSUB 570
    dungeon_contents[*monster_coord_x][*monster_coord_y] = char_code_blank;
    coord_x = *monster_coord_x;
    coord_y = *monster_coord_y;
    render_coord_and_check_for_monster(
        screen, char_code_vase, char_code_safe_place, distance_to_monster_x,
        monster_coord_x, monster_coord_y, monster_type, monster_strength,
        monster_char_code, monster_speed, dungeon_contents, coord_x, coord_y
    );
    // 660 LET R(MX,MY)=MT:LET X=MX:LET Y=MY:GOSUB 570
    dungeon_contents[*monster_next_coord_x][*monster_next_coord_y] =
        *monster_char_code;
    coord_x = *monster_next_coord_x;
    coord_y = *monster_next_coord_y;
    render_coord_and_check_for_monster(
        screen, char_code_vase, char_code_safe_place, distance_to_monster_x,
        monster_coord_x, monster_coord_y, monster_type, monster_strength,
        monster_char_code, monster_speed, dungeon_contents, coord_x, coord_y
    );
    // 670 LET LX=MX:LET LY=MY:LET H=0
    *monster_coord_x = *monster_next_coord_x;
    *monster_coord_y = *monster_next_coord_y;
    damage = 0;
    // 680 IF ABS(DX)<=1 AND ABS(DY)<=1 AND RH<>C7 THEN LET H=M*.5:LET J=H:GOSUB350
    if (
            abs(*distance_to_monster_x) <= 1 &&
            abs(distance_to_monster_y) <= 1 &&
            item_at_character_coord != char_code_safe_place
    ) {
        damage = *monster_type * 0.5;
        sound_frequency = damage;
        sound_sawtooth(sound_frequency);
    } else {
        // This line was not in the original. We set J here on the unlikely
        // chance that the next condition is True while the previous condition
        // was False. This is unlikely because H would be zero, so the sum of
        // F[6] and F[3] would have to be zero or less. In case that does
        // happen, we make sure that J is defined.
        sound_frequency = 0;
    }
    // 690 IF H*12<F(6)+F(3) THEN RETURN
    if (damage * 12 < attrs[6] + attrs[3]) {
        return;
    }
    // 700 LET M$=T$(5):GOSUB430:GOSUB360
    message = (char *) malloc(sizeof(char) * (strlen(strings[5]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, strings[5]);
    draw_message(screen, message, screen_cols);
    free(message);
    sound_noise(sound_frequency);
    // 710 LET H=H/(3+O(9) + O(10) + O(11) + O(12) + O(13) + O(14))
    damage /= (
        3 + inventory[9] + inventory[10] + inventory[11] + inventory[12] +
        inventory[13] + inventory[14]
    );
    // 720 LET F(1)=F(1)-H:LET F(2)=F(2)-(H/101)
    attrs[1] -= damage;
    attrs[2] -= damage / 101;
    // 730 LET I=1:LET WB=0:LET MB=rnd(M)
    item_num = 1;
    // WB is ony set here and not used anywhere.
    monster_broke_item = rand() % *monster_type;
    // 740 LET J=MT:GOSUB350:GOSUB360
    sound_frequency = *monster_char_code;
    sound_sawtooth(sound_frequency);
    sound_noise(sound_frequency);
    // 750 IF MB=1 AND O(I)>0 THEN GOSUB780
    int done = 0;
    do {
        if (monster_broke_item == 1 && inventory[item_num] > 0) {
            lines780_800(
                screen, item_num, sound_frequency, &monster_broke_item,
                inventory, strings, screen_cols, item_names
            );
        }
    // 760 IF I<11 THEN LET I=I+1:GOTO750
        if (item_num < 11) {
            item_num += 1;
            done = 1;
        }
    } while (!done);
    // 770 RETURN
}

void lines780_800(screen_t *screen, int item_num, int sound_frequency,
                  int *monster_broke_item, int inventory[25],
                  const char **strings, int screen_cols,
                  const char **item_names) {
    char * message;
    // 780 LET O(I)=0:LET M$=T$(8)+" "+W$(I):GOSUB430
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
    // 790 LET MB=0:GOSUB360:LET J=I:GOSUB350
    *monster_broke_item = 0;
    sound_noise(sound_frequency);
    sound_frequency = item_num;
    sound_sawtooth(sound_frequency);
    // 800 RETURN
}

void lines810_860(screen_t *screen, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, char *char_code_hero, int *monster_coord_x,
                  int *monster_coord_y, int *monster_type,
                  int *monster_strength, int *monster_char_code,
                  int *monster_speed, int *character_facing,
                  int character_coord_x, int character_coord_y,
                  int **dungeon_contents, int screen_cols, int coord_x,
                  int coord_y) {
    char * message;
    int sound_frequency;
    // 810 LET NF=5;LET F(1)=0:GOSUB 440
    *character_facing = 5;
    attrs[1] = 0;
    // This is one of two places where we call a method for rendering M$
    // without setting it first. The original BASIC code emptied M$ after each
    // time it was rendered. To aid de-globalization of variables, we empty M$
    // in those two places instead.
    message = (char *) malloc(sizeof(char));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, "");
    draw_message_wo_colour_change(screen, message, screen_cols);
    free(message);
    // 820 PRINT tab(1,5);"THOU HAST EXPIRED!"
    tab(screen->cursor, 1, 5);
    free(print_text(screen, "THOU HAST EXPIRED!"));
    // 830 FOR J=150 TO 1 STEP-4
    for (sound_frequency = 150; sound_frequency >= 1; sound_frequency -= 4) {
    // 840 GOSUB350:GOSUB360:GOSUB570:GOSUB480
        sound_sawtooth(sound_frequency);
        sound_noise(sound_frequency);
        render_coord_and_check_for_monster(
            screen, char_code_vase, char_code_safe_place,
            distance_to_monster_x, monster_coord_x, monster_coord_y,
            monster_type, monster_strength, monster_char_code, monster_speed,
            dungeon_contents, coord_x, coord_y
        );
        draw_character_and_stats(
            screen, attrs, char_code_hero, *character_facing,
            character_coord_x, character_coord_y
        );
    // 850 NEXT J
    }
    // 860 RETURN
}

void lines940_980(screen_t *screen, int char_code_blank, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength,
                  int *monster_char_code, int *monster_speed,
                  int monster_next_coord_x, int monster_next_coord_y,
                  int **dungeon_contents, const char **strings,
                  int screen_cols, int coord_x,
                  int coord_y);

void lines870_930(screen_t *screen, int char_code_blank, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength,
                  int *monster_char_code, int *monster_speed,
                  int monster_next_coord_x, int monster_next_coord_y,
                  int inventory[25], int **dungeon_contents,
                  const char **strings, int screen_cols, int coord_x,
                  int coord_y) {
    // 870 LET M$=T$(rnd(3)):GOSUB360
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
    sound_noise(100);
    free(message);
    // 880 LET H=F(1)+O(1) + O(2) + O(3) + O(4) + O(5) + O(6) + O(7) + O(8) + rnd(F(6))
    damage =
        attrs[1] + inventory[1] + inventory[2] + inventory[3] + inventory[4] +
        inventory[5] + inventory[6] + inventory[7] + inventory[8] +
        (rand() * attrs[6] / RAND_MAX);
    // 890 IF F(3)+F(6)< rnd(M)+2 THEN LET M$=T$(4):LET HT=0
    if (attrs[3] + attrs[6] < rand() % *monster_type + 2) {
        message = (char *) malloc(sizeof(char) * (strlen(strings[4]) + 1));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, strings[4]);
        // Probably a bug in the original game: HT is never set or referenced
        // anywhere else. It should be H instead.
        damage = 0;
    }
    // 900 LET MS=MS-H:GOSUB430
    *monster_strength -= damage;
    draw_message(screen, message, screen_cols);
    free(message);
    // 910 LET F(1)=F(1)-(H/100):LET F(5)=F(5)+0.05
    attrs[1] -= damage / 100;
    attrs[5] += 0.05;
    // 920 IF MS<1 THEN GOSUB940
    if (*monster_strength < 1) {
        lines940_980(
            screen, char_code_blank, char_code_vase, char_code_safe_place,
            distance_to_monster_x, attrs, monster_coord_x, monster_coord_y,
            monster_type, monster_strength, monster_char_code, monster_speed,
            monster_next_coord_x, monster_next_coord_y, dungeon_contents,
            strings, screen_cols, coord_x, coord_y
        );
    }
    // 930 RETURN
}

void lines940_980(screen_t *screen, int char_code_blank, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength,
                  int *monster_char_code, int *monster_speed,
                  int monster_next_coord_x, int monster_next_coord_y,
                  int **dungeon_contents, const char **strings,
                  int screen_cols, int coord_x, int coord_y) {
    char * message;
    int sound_frequency;
    // 940 LET DX=255:LET MS=0:LET R(MX,MY)=C0
    *distance_to_monster_x = 255;
    *monster_strength = 0;
    dungeon_contents[monster_next_coord_x][monster_next_coord_y] =
        char_code_blank;
    // 950 LET F(5)=F(5)+.1
    attrs[5] += 0.1;
    // 960 LET M$=T$(6):GOSUB430
    message = (char *) malloc(sizeof(char) * (strlen(strings[6]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, strings[6]);
    draw_message(screen, message, screen_cols);
    free(message);
    // 970 FOR J=200 TO 150STEP-8:GOSUB350:GOSUB360:NEXT J
    for (sound_frequency = 200; sound_frequency >= 150; sound_frequency -= 8) {
        sound_sawtooth(sound_frequency);
        sound_noise(sound_frequency);
    }
    // 980 GOSUB570:RETURN
    render_coord_and_check_for_monster(
        screen, char_code_vase, char_code_safe_place, distance_to_monster_x,
        monster_coord_x, monster_coord_y, monster_type, monster_strength,
        monster_char_code, monster_speed, dungeon_contents, coord_x, coord_y
    );
}

void lines1140_1180(screen_t *screen, int char_code_blank, int char_code_vase,
                    int char_code_safe_place, int *distance_to_monster_x,
                    double *attrs, int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength,
                    int *monster_char_code, int *monster_speed,
                    int monster_next_coord_x, int monster_next_coord_y,
                    int **dungeon_contents, const char **strings,
                    int screen_cols);
void lines1190_1210(int char_code_blank, int char_code_safe_place,
                    int character_coord_x, int character_coord_y,
                    int **dungeon_contents, int item_at_character_coord);
void lines1220_1270(screen_t *screen, double *attrs, char *char_code_hero,
                    int character_facing, int *character_coord_x,
                    int *character_coord_y);
void lines1280_1290(double *attrs, int *spells_remaining, int spell_number);
void lines1300_1380(screen_t *screen, int char_code_blank, int char_code_vase,
                    int char_code_safe_place, int *distance_to_monster_x,
                    int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength,
                    int *monster_char_code, int *monster_speed,
                    int character_coord_x, int character_coord_y,
                    int **dungeon_contents, int item_at_character_coord,
                    int coord_x, int coord_y);
void lines1390_1400(double *attrs, double initial_strength,
                    double initial_vitality);

void lines990_1130(screen_t *screen, int char_code_blank, int char_code_vase,
                   int char_code_safe_place, int *distance_to_monster_x,
                   double *attrs, char *char_code_hero, int *monster_coord_x,
                   int *monster_coord_y, int *spells_remaining,
                   int *monster_type, int *monster_strength,
                   int *monster_char_code, int *monster_speed,
                   int monster_next_coord_x, int monster_next_coord_y,
                   int character_facing, int *character_coord_x,
                   int *character_coord_y, int inventory[25],
                   int **dungeon_contents, int item_at_character_coord,
                   double initial_strength, double initial_vitality,
                   const char **strings, int screen_cols) {
    int row_num, spell_number, coord_x, coord_y;
    char pressed_key, * message;
    // 990 GOSUB480:paper 2: ink 0
    draw_character_and_stats(
        screen, attrs, char_code_hero, character_facing, *character_coord_x,
        *character_coord_y
    );
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    // 1000 PRINT tab(0,10);"YOU MAY USE MAGICKS";
    tab(screen->cursor, 0, 1);
    free(print_text(screen, "YOU MAY USE MAGICKS"));
    // 1010 IF O(17)>0 THEN PRINT tab(0,2);"FROM NECRONOMICON";
    if (inventory[17] > 0) {
        tab(screen->cursor, 0, 2);
        free(print_text(screen, "FROM NECRONOMICON"));
    }
    // 1020 IF O(18)>0 THEN PRINT tab(0,3);"FROM THE SCROLLS";
    if (inventory[18] > 0) {
        tab(screen->cursor, 0, 3);
        free(print_text(screen, "FROM THE SCROLLS"));
    }
    // 1030 PRINT tab(0,4);"CONSULT THE LORE"
    tab(screen->cursor, 0, 4);
    free(print_text(screen, "CONSULT THE LORE"));
    // 1040 LET M$="USE SPELL NUMBER?":GOSUB370
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
    // 1050 LET SL=VAL(I$)
        spell_number = atoi(outstring);
        free(outstring);
    // 1060 IF SL=0 OR (O(17)=0 AND SL<5) OR (O(18)=0 AND SL>3) OR SL>6 THEN GOTO 1040
    } while (
            spell_number == 0 ||
            (inventory[17] == 0 && spell_number < 5) ||
            (inventory[19] == 0 && spell_number > 3) ||
            spell_number > 6
    );

    // 1070 LET M(SL)=M(SL)-1:LET X=NX:LET Y=NY
    spells_remaining[spell_number] -= 1;
    coord_x = *character_coord_x;
    coord_y = *character_coord_y;
    // 1080 IF M(SL)<0 THEN LET M$=T$(9):LET SL=7
    if (spells_remaining[spell_number] < 0) {
        message = (char *) malloc(sizeof(char) * (strlen(strings[9]) + 1));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, strings[9]);
        spell_number = 7;
    } else {
        // This is the other place where we may call a method for rendering M$
        // without setting it first. The original BASIC code emptied M$ after
        // each time it was rendered. In this case, we empty after the call to
        // lines940_980(), since we don't know when it will be used next.
        message = (char *) malloc(sizeof(char));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, "");
    }
    // 1090 FOR J=1 TO 5:PRINT tab(0,J);LEFT$(B$, W);:NEXT J:GOSUB570
    for (row_num = 1; row_num <= 5; row_num += 1) {
        tab(screen->cursor, 0, row_num);
        print_left$_b$(screen, screen_cols);
        newline(screen->cursor);
    }
    render_coord_and_check_for_monster(
        screen, char_code_vase, char_code_safe_place, distance_to_monster_x,
        monster_coord_x, monster_coord_y, monster_type, monster_strength,
        monster_char_code, monster_speed, dungeon_contents, coord_x, coord_y
    );
    // 1100 ON SL GOSUB1140,1190,1220,1280,1300,1390,1130
    switch (spell_number) {
        case 1:
            lines1140_1180(
                screen, char_code_blank, char_code_vase, char_code_safe_place,
                distance_to_monster_x, attrs, monster_coord_x, monster_coord_y,
                monster_type, monster_strength, monster_char_code,
                monster_speed, monster_next_coord_x, monster_next_coord_y,
                dungeon_contents, strings, screen_cols
            );
            break;
        case 2:
            lines1190_1210(
                char_code_blank, char_code_safe_place, *character_coord_x,
                *character_coord_y, dungeon_contents, item_at_character_coord
            );
            break;
        case 3:
            lines1220_1270(
                screen, attrs, char_code_hero, character_facing,
                character_coord_x, character_coord_y
            );
            break;
        case 4:
            lines1280_1290(attrs, spells_remaining, spell_number);
            break;
        case 5:
            lines1300_1380(
                screen, char_code_blank, char_code_vase, char_code_safe_place,
                distance_to_monster_x, monster_coord_x, monster_coord_y,
                monster_type, monster_strength, monster_char_code,
                monster_speed, *character_coord_x, *character_coord_y,
                dungeon_contents, item_at_character_coord, coord_x, coord_y
            );
            break;
        case 6:
            lines1390_1400(attrs, initial_strength, initial_vitality);
            break;
        case 7:
            // Line 1130 is just a return statement.
            break;
    }
    // 1110 LET F(5)=F(5)+.2
    attrs[5] += 0.2;
    // 1120 GOSUB430
    draw_message(screen, message, screen_cols);
    free(message);
    // 1130 RETURN
}

void lines1140_1180(screen_t *screen, int char_code_blank, int char_code_vase,
                    int char_code_safe_place, int *distance_to_monster_x,
                    double *attrs, int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength,
                    int *monster_char_code, int *monster_speed,
                    int monster_next_coord_x, int monster_next_coord_y,
                    int **dungeon_contents, const char **strings,
                    int screen_cols) {
    int sound_frequency, coord_x, coord_y;
    // 1140 FOR J=1 TO 12
    for (sound_frequency = 1; sound_frequency <= 12; sound_frequency += 1) {
    // 1150 GOSUB350:GOSUB360
        sound_sawtooth(sound_frequency);
        sound_noise(sound_frequency);
    // 1160 NEXT J
    }
    // 1170 IF DX<255 THEN LET X=MX:LET Y=MY:GOSUB940
    if (*distance_to_monster_x < 255) {
        coord_x = monster_next_coord_x;
        coord_y = monster_next_coord_y;
        lines940_980(
            screen, char_code_blank, char_code_vase, char_code_safe_place,
            distance_to_monster_x, attrs, monster_coord_x, monster_coord_y,
            monster_type, monster_strength, monster_char_code, monster_speed,
            monster_next_coord_x, monster_next_coord_y, dungeon_contents,
            strings, screen_cols, coord_x, coord_y
        );
    }
    // 1180 RETURN
}

void lines1190_1210(int char_code_blank, int char_code_safe_place,
                    int character_coord_x, int character_coord_y,
                    int **dungeon_contents, int item_at_character_coord) {
    int sound_frequency;
    // 1190 IF RH=C0 THEN LET R(NX,NY)=C7
    if (item_at_character_coord == char_code_blank) {
        dungeon_contents[character_coord_x][character_coord_y] =
            char_code_safe_place;
    }
    // 1200 LET J=100:GOSUB350:LET J=200:GOSUB350
    sound_frequency = 100;
    sound_sawtooth(sound_frequency);
    sound_frequency = 200;
    sound_sawtooth(sound_frequency);
    // 1210 RETURN
}

void lines1220_1270(screen_t *screen, double *attrs, char *char_code_hero,
                    int character_facing, int *character_coord_x,
                    int *character_coord_y) {
    int sound_frequency;
    // 1220 LET NX=rnd(13):LET NY=rnd(13)
    *character_coord_x = rand() % 13;
    *character_coord_y = rand() % 13;
    // 1230 FOR J=0 TO 255 STEP8
    for (sound_frequency = 0; sound_frequency <= 255; sound_frequency += 8) {
    // 1240 GOSUB360:GOSUB350
        sound_noise(sound_frequency);
        sound_sawtooth(sound_frequency);
    // 1250 NEXT J
    }
    // 1260 GOSUB480
    draw_character_and_stats(
        screen, attrs, char_code_hero, character_facing, *character_coord_x,
        *character_coord_y
    );
    // 1270 RETURN
}

void lines1280_1290(double *attrs, int *spells_remaining, int spell_number) {
    // 1280 LET F(2)=F(2)+rnd(M(SL)):LET F(1)=F(1)+rnd(M(SL)):LET F(7)=F(7)-1
    attrs[2] += rand() * spells_remaining[spell_number];
    attrs[1] += rand() * spells_remaining[spell_number];
    attrs[7] -= 1;
    // 1290 RETURN
}

void lines1300_1380(screen_t *screen, int char_code_blank, int char_code_vase,
                    int char_code_safe_place, int *distance_to_monster_x,
                    int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength,
                    int *monster_char_code, int *monster_speed,
                    int character_coord_x, int character_coord_y,
                    int **dungeon_contents, int item_at_character_coord,
                    int coord_x, int coord_y) {
    int sound_frequency;
    // 1300 FOR J=1 TO 30
    for (sound_frequency = 1; sound_frequency <= 30; sound_frequency += 1) {
    // 1310 LET R(NX,NY)=rnd(8)+1+C0
        dungeon_contents[character_coord_x][character_coord_y] =
            rand() % 8 + 1 + char_code_blank;
    // 1320 GOSUB350:GOSUB570
        sound_sawtooth(sound_frequency);
        render_coord_and_check_for_monster(
            screen, char_code_vase, char_code_safe_place,
            distance_to_monster_x, monster_coord_x, monster_coord_y,
            monster_type, monster_strength, monster_char_code, monster_speed,
            dungeon_contents, coord_x, coord_y
        );
    // 1330 NEXT J
    }
    // 1340 IF RH<=C7 THEN LET DX=255:LET MS=0
    if (item_at_character_coord <= char_code_safe_place) {
        *distance_to_monster_x = 255;
        *monster_strength = 0;
    }
    // 1350 FOR J = 1 TO 2O STEP4
    for (sound_frequency = 1; sound_frequency <= 20; sound_frequency += 4) {
    // 1360 GOSUB 350
        sound_sawtooth(sound_frequency);
    // 1370 NEXT J
    }
    // 1380 RETURN
}

void lines1390_1400(double *attrs, double initial_strength,
                    double initial_vitality) {
    // 1390 LET F(2)=S2:LET F(1)=S1:LET F(7)=F(7)-1
    attrs[2] = initial_vitality;
    attrs[1] = initial_strength;
    attrs[7] -= 1;
    // 1400 RETURN
}

void lines1410_1520(screen_t *screen, int char_code_blank, int char_code_wall,
                    int char_code_vase, int char_code_chest,
                    int char_code_idol, int char_code_safe_place,
                    int **vertices, int *distance_to_monster_x, double *attrs,
                    char *char_code_hero, int *finished, int gold,
                    int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength,
                    int *monster_char_code, int *monster_speed,
                    int *character_facing, int character_coord_x,
                    int character_coord_y, int inventory[25],
                    int **dungeon_contents, int *song_notes, int *treasure) {
    int sound_frequency, item_to_get, item_to_get_coord_x, item_to_get_coord_y,
        coord_x, coord_y;
    // 1410 LET GX=NX+D(NF,1):LET GY=NY+D(NF,2)
    item_to_get_coord_x = character_coord_x + vertices[*character_facing][1];
    item_to_get_coord_y = character_coord_y + vertices[*character_facing][2];
    // 1420 IF GX<0 THEN LET GX=0
    if (item_to_get_coord_x < 0) {
        item_to_get_coord_x = 0;
    }
    // 1430 IF GY<0 THEN LET GY=0
    if (item_to_get_coord_y < 0) {
        item_to_get_coord_y = 0;
    }
    // 1440 IF GX>15 THEN LET GX=15
    if (item_to_get_coord_x > 15) {
        item_to_get_coord_x = 15;
    }
    // 1450 IF GY>15 THEN LET GY=15
    if (item_to_get_coord_y > 15) {
        item_to_get_coord_y = 15;
    }
    // 1460 LET GT=R(GX,GY):IF GT>C1 AND GT<C4 THEN LET R(GX,GY)=C0
    item_to_get = dungeon_contents[item_to_get_coord_x][item_to_get_coord_y];
    if (item_to_get > char_code_wall && item_to_get < char_code_idol) {
        dungeon_contents[item_to_get_coord_x][item_to_get_coord_y] =
            char_code_blank;
    }
    // 1470 IF GT=C2 THEN LET O(23)=O(23)+1:LET O(24)=O(24)+1
    if (item_to_get == char_code_vase) {
        inventory[23] += 1;
        inventory[24] += 1;
    }
    // 1480 IF GT=C3 THEN LET TR=TR+1
    if (item_to_get == char_code_chest) {
        treasure += 1;
    }
    // 1490 IF GT=C4 THEN GOSUB 1550
    if (item_to_get == char_code_idol) {
        lines1550_1650(
            screen, attrs, char_code_hero, finished, gold, monster_strength,
            character_facing, character_coord_x, character_coord_y, song_notes,
            *treasure
        );
    }
    // 1500 LET X=GX:LET Y=GY:GOSUB570
    coord_x = item_to_get_coord_x;
    coord_y = item_to_get_coord_y;
    render_coord_and_check_for_monster(
        screen, char_code_vase, char_code_safe_place, distance_to_monster_x,
        monster_coord_x, monster_coord_y, monster_type, monster_strength,
        monster_char_code, monster_speed, dungeon_contents, coord_x, coord_y
    );
    // 1510 IF GT>C1 AND GT<C4 THEN LET J=GT:GOSUB350:LET J=GT+5:GOSUB350
    if (item_to_get > char_code_wall && item_to_get < char_code_idol) {
        sound_frequency = item_to_get;
        sound_sawtooth(sound_frequency);
        sound_frequency = item_to_get + 5;
        sound_sawtooth(sound_frequency);
    }
    // 1520 RETURN
}

void lines1550_1650(screen_t *screen, double *attrs, char *char_code_hero,
                    int *finished, int gold, int *monster_strength,
                    int *character_facing, int character_coord_x,
                    int character_coord_y, int *song_notes, int treasure) {
    int index, sound_frequency, direction;
    // 1550 paper 2:ink 1
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, RED);
    // 1560 PRINT tab(0,1);" THY QUEST IS OVER! "
    tab(screen->cursor, 0, 1);
    free(print_text(screen, " THY QUEST IS OVER! "));
    // 1570 FOR I = 1 TO 18
    for (index = 1; index <= 18; index += 1) {
    // 1580 LET J=T(I):GOSUB350
        sound_frequency = song_notes[index];
        sound_sawtooth(sound_frequency);

    // 1590 LET X=NX:LET Y=NY
        // X and Y are overwritten before the above values are used.
    // 1600 FOR N=1 TO 4:LET NF=N:GOSUB480:NEXT N
        for (direction = 1; direction <=4; direction += 1) {
            *character_facing = direction;
            draw_character_and_stats(
                screen, attrs, char_code_hero, *character_facing,
                character_coord_x, character_coord_y
            );
        }
    // 1610 NEXT I
    }
    // 1620 LET MS=0
    *monster_strength = 0;
    // 1630 PRINT tab(1,2);"THY SCORE=";INT((TR*10)+(GC*F(5))+F(1)+F(2)+F(3))
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
            (treasure * 10) + (gold * attrs[5]) + attrs[1] + attrs[2] +
            attrs[3]
        )
    );
    free(print_text(screen, outstring));
    free(outstring);
    // 1640 LET FI=1
    *finished = 1;
    // 1650 RETURN
}

void lines1660_1680(double *attrs, int inventory[25], double initial_strength,
                    double initial_vitality) {
    // 1660 IF O(24)>0 AND F(1)<S1 THEN LET F(1)=S1:LET O(24)=O(24)-1
    if (inventory[24] > 0 && attrs[1] < initial_strength) {
        attrs[1] = initial_strength;
        inventory[24] -= 1;
    }
    // 1670 IF O(23)>0 AND F(2)<S2 THEN LET F(2)=S2:LET O(23)=O(23)-1
    if (inventory[23] > 0 && attrs[2] < initial_vitality) {
        attrs[2] = initial_vitality;
        inventory[23] -= 1;
    }
    // 1680 RETURN
}

void lines1690_1750(screen_t *screen, int char_code_vase,
                    int char_code_safe_place, int *distance_to_monster_x,
                    int *torches, int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength,
                    int *monster_char_code, int *monster_speed,
                    int character_coord_x, int character_coord_y,
                    int **dungeon_contents, const char **strings,
                    int screen_cols) {
    int coord_x, coord_y;
    char * message;
    // 1690 IF LT=0 THEN LET M$=T$(7):GOSUB430:RETURN
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
    // 1700 FOR Y=NY-3 TO NY+3
    for (
            coord_y = character_coord_y - 3;
            coord_y <= character_coord_y + 3;
            coord_y += 1
    ) {
    // 1710 FOR X=NX-3 TO NX+3
        for (
                coord_x = character_coord_x - 3;
                coord_x <= character_coord_x + 3;
                coord_x += 1
        ) {
    // 1720 IF (X>0 AND X<16) AND (Y>0 AND Y<16)THEN GOSUB570
            if (coord_x > 0 && coord_x < 16 && coord_y > 0 && coord_y < 16) {
                render_coord_and_check_for_monster(
                    screen, char_code_vase, char_code_safe_place,
                    distance_to_monster_x, monster_coord_x, monster_coord_y,
                    monster_type, monster_strength, monster_char_code,
                    monster_speed, dungeon_contents, coord_x, coord_y
                );
            }
    // 1730 NEXT X:NEXT Y
        }
    }
    // 1740 LET LT=LT-1
    *torches -= 1;
    // 1750 RETURN
}

void get_keyboard_input(screen_t *screen, char *pressed_key, char *message,
                        int screen_cols);
void lines1960_2000(screen_t *screen, double *attrs);
void lines2790_2920(screen_t *screen, char *character_name, int screen_cols);

void lines1760_1770_1950(screen_t *screen, int start_at_1770,
                         char *character_name, int *distance_to_monster_x,
                         double *attrs, int *dungeon_level,
                         int *character_coord_x, int *character_coord_y,
                         int dungeon_char_base, int *character_prev_coord_x,
                         int *character_prev_coord_y, int **dungeon_contents,
                         double initial_experience, const char **strings,
                         int screen_cols) {
    // The original BASIC code sometimes used 'GOSUB 1760' and sometimes
    // 'GOSUB 1770'. This is further complicated by their use of a
    // 'GOTO 1760' towards the end.
    // We use the 'start_at_1770' flag to handle this.
    int correct_level_loaded, index, entrance_coord_x, entrance_coord_y,
        coord_x, coord_y;
    char pressed_key, * message;
    do {

    // 1760 IF F(5)<S3+1 THEN LET M$=T$(11):LET NX=OX:LET NY=OY:GOSUB 430:RETURN
        if (!start_at_1770 && attrs[5] < initial_experience + 1) {
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
        start_at_1770 = 0;
    // 1770 CLS:PRINT tab(0,3);"PREPARE DUNGEON TAPE"
        clear_screen(screen);
        tab(screen->cursor, 0, 3);
        free(print_text(screen, "PREPARE DUNGEON TAPE"));
    // 1780 LET M$=T$(10):GOSUB370
        message = (char *) malloc(sizeof(char) * (strlen(strings[10]) + 1));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, strings[10]);
        get_keyboard_input(screen, &pressed_key, message, screen_cols);
        free(message);
        size_t filesize;
    // 1790 S=OPENIN"LEVEL"
        FILE *file_handle = fopen("LEVEL", "r");
    // 1800 INPUT#S,S$
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
    // 1810 CLOSE#S
        fclose(file_handle);
    // 1820 LET I=1
        index = 1;
    // 1830 FOR Y=1 TO 15
        for (coord_y = 1; coord_y <= 15; coord_y += 1) {
    // 1840 FOR X=1 TO 15
            for (coord_x = 1; coord_x <= 15; coord_x += 1) {
    // 1850 LET R(X,Y)=ASC(MID$(S$,I,1))
                dungeon_contents[coord_x][coord_y] =
                    (int) file_contents[index - 1];
    // 1860 LET I=I+1
                index += 1;
    // 1870 NEXT X
            }
    // 1880 NEXT Y
        }
    // 1890 LET IX=ASC(MID$(S$,I,1))-OS
        entrance_coord_x = (int) file_contents[index - 1] - dungeon_char_base;
    // 1900 LET IY=ASC(MID$(S$,I+1,1))-OS
        entrance_coord_y = (int) file_contents[index] - dungeon_char_base;
    // 1910 LET LE=ASC(MID$(S$,I+2,1))-OS
        *dungeon_level = (int) file_contents[index + 1] - dungeon_char_base;
    // 1920 IF LE>F(5) THEN GOSUB 1960:GOTO 1760
        if (*dungeon_level > attrs[5]) {
            lines1960_2000(screen, attrs);
            correct_level_loaded = 1;
        } else {
            correct_level_loaded = 0;
        }
    } while (correct_level_loaded);
    // 1930 GOSUB2790
    lines2790_2920(screen, character_name, screen_cols);
    // 1940 LET NX=IX:LET NY=IY:LET OX=NX:LET OY=NY:LET DX=255
    *character_coord_x = entrance_coord_x;
    *character_coord_y = entrance_coord_y;
    *character_prev_coord_x = *character_coord_x;
    *character_prev_coord_y = *character_coord_y;
    *distance_to_monster_x = 255;
    // 1950 RETURN
}

void lines1760_1950(screen_t *screen, char *character_name,
                    int *distance_to_monster_x, double *attrs,
                    int *dungeon_level, int *character_coord_x,
                    int *character_coord_y, int dungeon_char_base,
                    int *character_prev_coord_x, int *character_prev_coord_y,
                    int **dungeon_contents, double initial_experience,
                    const char **strings, int screen_cols) {
    lines1760_1770_1950(
        screen, 0, character_name, distance_to_monster_x, attrs, dungeon_level,
        character_coord_x, character_coord_y, dungeon_char_base,
        character_prev_coord_x, character_prev_coord_y, dungeon_contents,
        initial_experience, strings, screen_cols
    );
}

void lines1770_1950(screen_t *screen, char *character_name,
                    int *distance_to_monster_x, double *attrs,
                    int *dungeon_level, int *character_coord_x,
                    int *character_coord_y, int dungeon_char_base,
                    int *character_prev_coord_x, int *character_prev_coord_y,
                    int **dungeon_contents, double initial_experience,
                    const char **strings, int screen_cols) {
    lines1760_1770_1950(
        screen, 1, character_name, distance_to_monster_x, attrs, dungeon_level,
        character_coord_x, character_coord_y, dungeon_char_base,
        character_prev_coord_x, character_prev_coord_y, dungeon_contents,
        initial_experience, strings, screen_cols
    );
}

void lines1960_2000(screen_t *screen, double *attrs) {
    // 1960 PRINT:PRINT"LEVEL TOO DEEP"
    newline(screen->cursor);
    free(print_text(screen, "LEVEL TOO DEEP"));
    newline(screen->cursor);
    // 1970 PRINT"REWIND TAPE"
    free(print_text(screen, "REWIND TAPE"));
    newline(screen->cursor);
    // 1980 PRINT"TO POSITION"
    free(print_text(screen, "TO POSITION"));
    newline(screen->cursor);
    // 1990 PRINT"FOR LEVEL";F(5)
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    sprintf(outstring, "FOR LEVEL %d", (int) attrs[5]);
    free(print_text(screen, outstring));
    free(outstring);
    // 2000 RETURN
}

void lines2010_2250(screen_t *screen, int character_char_base,
                    char **character_name, double *attrs, int *gold,
                    int *torches, int *spells_remaining, int inventory[25],
                    int *num_item_types, double *initial_strength,
                    double *initial_vitality, double *initial_experience,
                    const char **strings, int *treasure, int screen_cols) {
    char pressed_key, * message;
    int index, subindex, file_index;
    // 2010 CLS:PRINT tab(0,3);"PREPARE HERO TAPE"
    clear_screen(screen);
    tab(screen->cursor, 0, 3);
    free(print_text(screen, "PREPARE HERO TAPE"));
    // 2020 LET M$=T$(10):GOSUB370
    message = (char *) malloc(sizeof(char) * (strlen(strings[10]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, strings[10]);
    get_keyboard_input(screen, &pressed_key, message, screen_cols);
    free(message);
    // 2030 S=OPENIN "HERO"
    FILE *file_handle = fopen("HERO", "r");
    // 2040 INPUT#S,S$
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
    // 2050 CLOSE#S
    fclose(file_handle);
    free(file_handle);
    file_contents[filesize] = 0;
    // 2060 LET P=2
    file_index = 2;
    // 2070 LET OT=ASC(MID$(S$,1,1))-AS
    *num_item_types = (int) file_contents[0] - character_char_base;
    // 2080 FOR I= 1 TO 8
    for (index = 1; index <= 8; index += 1) {
    // 2090 LET F(I)=ASC(MID$(S$,P,1)) - AS
        attrs[index] =
            (int) file_contents[file_index - 1] - character_char_base;
    // 2100 LET P=P+1
        file_index += 1;
    // 2110 NEXT I
    }
    // 2120 FOR I=1 TO OT
    for (index = 1; index <= *num_item_types; index += 1) {
    // 2130 LET O(I)=ASC(MID$(S$,P,1))-AS
        inventory[index] =
            (int) file_contents[file_index - 1] - character_char_base;
    // 2140 LET P=P+1
        file_index += 1;
    // 2150 NEXT I
    }
    // 2160 LET GC=ASC(MID$(S$,P,1))-AS
    *gold = (int) file_contents[file_index - 1] - character_char_base;
    // 2170 LET TR=ASC(MID$(S$,P+1,1))-AS
    *treasure = (int) file_contents[file_index] - character_char_base;
    // 2180 LET C$=RIGHT$(S$,LEN(S$)-(P+1))
    *character_name = (char *) malloc(sizeof(char) * (file_index + 2));
    if (*character_name == NULL) {
        fprintf(stderr, "character_name is NULL!\n");
        exit(1);
    }
    strcpy(*character_name, file_contents + file_index + 1);
    // 2190 LET S1=F(1):LET S2=F(2):LET S3=F(5)
    *initial_strength = attrs[1];
    *initial_vitality = attrs[2];
    *initial_experience = attrs[5];
    // 2200 FOR I=1 TO 2
    for (index = 1; index <= 2; index += 1) {
    // 2210 FOR J=1 TO 3
        for (subindex = 1; subindex <= 3; subindex += 1) {
    // 2220 LET M((I-1)*3+J)=O(16+I)*F(7)
            spells_remaining[(index - 1) * 3 + subindex] =
                inventory[16 + index] * attrs[7];
    // 2230 NEXT J:NEXT I
        }
    }
    // 2240 IF O(16)=1 THEN LET LT=20
    if (inventory[16] == 1) {
        *torches = 20;
    }
    // 2250 RETURN
    free(file_contents);
}

void lines2260_2490(screen_t *screen, int character_char_base,
                    char *character_name, double *attrs, int *finished,
                    int gold, int dungeon_level, int character_coord_x,
                    int character_coord_y, int inventory[25],
                    int dungeon_char_base, int num_item_types,
                    int **dungeon_contents, int treasure, int screen_cols) {
    int index, coord_x, coord_y;
    char pressed_key, * message;
    // 2260 LET M$="ONE MOMENT PLEASE":GOSUB430
    message = (char *) malloc(sizeof(char) * 18);
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, "ONE MOMENT PLEASE");
    draw_message(screen, message, screen_cols);
    free(message);
    // 2270 LET S$="":LET T$=""
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
    // 2280 FOR Y=1 TO 15
    for (coord_y = 1; coord_y <= 15; coord_y += 1) {
    // 2290 FOR X=1 TO 15
        for (coord_x = 1; coord_x <= 15; coord_x += 1) {
    // 2300 LET T$=T$+CHR$(R(X,Y))
            dungeon_file_contents[t_index] =
                (char) dungeon_contents[coord_x][coord_y];
            t_index += 1;
    // 2310 NEXT X:NEXT Y
        }
    }
    // 2320 LET T$=T$+CHR$(OS+NX)
    dungeon_file_contents[t_index] =
        (char) (dungeon_char_base + character_coord_x);
    t_index += 1;
    // 2330 LET T$=T$+CHR$(OS+NY)
    dungeon_file_contents[t_index] =
        (char) (dungeon_char_base + character_coord_y);
    t_index += 1;
    // 2340 LET T$=T$+CHR$(OS+LE)
    dungeon_file_contents[t_index] =
        (char) (dungeon_char_base + dungeon_level);
    t_index += 1;
    dungeon_file_contents[t_index] = 0;
    // 2350 LET S$=S$+CHR$(AS+OT)
    character_file_contents[s_index] =
        (char) (character_char_base + num_item_types);
    s_index += 1;
    // 2360 FOR I=1 TO 8
    for (index = 1; index <= 8; index += 1) {
    // 2370 LET S$=S$+CHR$(F(I)+AS)
        character_file_contents[s_index] =
            (char) (attrs[index] + character_char_base);
        s_index += 1;
    // 2380 NEXT I
    }
    // 2390 FOR I=1 TO OT
    for (index = 1; index <= num_item_types; index += 1) {
    // 2400 LET S$=S$+CHR$(O(I)+AS)
        character_file_contents[s_index] = (char)
            (inventory[index] + character_char_base);
        s_index += 1;
    // 2410 NEXT I
    }
    // 2420 LET S$=S$+CHR$(GC+AS);
    character_file_contents[s_index] = (char) (gold + character_char_base);
    s_index += 1;
    // 2430 LET S$=S$+CHR$(TR+AS);
    character_file_contents[s_index] = (char) (treasure + character_char_base);
    s_index += 1;
    // 2440 LET S$=S$+C$
    strcpy(character_file_contents + s_index, character_name);
    s_index += strlen(character_name);
    character_file_contents[s_index] = 0;
    // 2450 LET M$="ANY KEY TO SAVE":GOSUB 370
    message = (char *) malloc(sizeof(char) * 16);
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, "ANY KEY TO SAVE");
    get_keyboard_input(screen, &pressed_key, message, screen_cols);
    free(message);
    // 2460 S=OPENOUT"HERO":PRINT#S,S$:CLOSE#S
    FILE *file_handle = fopen("HERO", "w");
    int error = fputs(character_file_contents, file_handle);
    if (error) {
        fprintf(stderr, "Error %i writing the character!", error);
    }
    error = fclose(file_handle);
    if (error) {
        fprintf(stderr, "Error %i closing the character!", error);
    }
    free(file_handle);
    // 2470 S=OPENOUT"LEVEL":PRINT#S,T$:CLOSE#S
    file_handle = fopen("LEVEL", "w");
    error = fputs(dungeon_file_contents, file_handle);
    if (error) {
        fprintf(stderr, "Error %i writing the level!", error);
    }
    error = fclose(file_handle);
    if (error) {
        fprintf(stderr, "Error %i closing the level!", error);
    }
    free(file_handle);
    // 2480 LET FI=1
    *finished = 1;
    // 2490 RETURN
}

void lines2930_3200(int *character_char_base, int *char_code_blank,
                    int *char_code_wall, int *char_code_vase,
                    int *char_code_chest, int *char_code_idol,
                    int *char_code_exit, int *char_code_trap,
                    int *char_code_safe_place, int dungeon_char_base);

void lines2500_2780(int *character_char_base, int *char_code_blank,
                    int *char_code_wall, int *char_code_vase,
                    int *char_code_chest, int *char_code_idol,
                    int *char_code_exit, int *char_code_trap,
                    int *char_code_safe_place, int ***vertices,
                    int *distance_to_monster_x, double **attrs,
                    char **char_code_hero, int *finished, int *torches,
                    int **spells_remaining, int *monster_next_coord_x,
                    int *monster_next_coord_y, int *character_facing,
                    int *character_coord_x, int *character_coord_y,
                    int *dungeon_char_base, int ***dungeon_contents,
                    int **song_notes, const char ***strings, int *trapped,
                    int *trap_coord_x, int *trap_coord_y, int *screen_cols,
                    const char ***item_names) {
    int index;
    // 2500 LET C$="ROLE PLAYING GAME":LET B$=""
    // C$ is overwritten before being accessed again.
    // dungeon_lib removes the need for B$
    // 2510 LET W=40:LET OS=96
    *screen_cols = 40;
    *dungeon_char_base = 96;
    // 2520 FOR I=1 TO W:LET B$=B$+" ":NEXT I
    // dungeon_lib removes the need for B$
    // 2530 DIM R(15,15),F(8),O(24)
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
    // 2540 DIM W$(11),T$(12)
    *item_names = (const char **) malloc(sizeof(const char *) * (12));
    if (*item_names == NULL) {
        fprintf(stderr, "*item_names is NULL!\n");
        exit(1);
    }
    // 2550 DIM M(6),D(4,2),T(18)
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
    // 2560 DATA"GR SWORD","SWORD","AXE","MACE","FLAIL","DAGGER","ARMOUR","ARMOUR"
    // 2570 DATA"ARMOUR","HELMET","HEADPC."
    // 2580 FOR I = 1 TO 11
    // 2590 READ W$(I)
    // 2600 NEXT I
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

    // 2610 DATA"A GOOD BLOW","WELL HIT SIRE","THY AIM IS TRUE","MISSED!","HIT THEE!!"
    // 2620 DATA"THE MONSTER IS SLAIN","NO LIGHT","BROKEN THY ","SPELL EXHAUSTED"
    // 2630 DATA"PRESS ANY KEY","YOU NEED EXPERIENCE","EXIT FROM THIS LEVEL"
    // 2640 FOR I = 1 TO 12
    // 2650 READ T$(I)
    // 2660 NEXT I

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


    // 2670 DATA0,-1,1,0,0,1,-1,0
    // 2680 FOR I=1 TO 4:READ D(I,1),D(I,2):NEXT I
    (*vertices)[1][1] = 0;
    (*vertices)[1][2] = -1;
    (*vertices)[2][1] = 1;
    (*vertices)[2][2] = 0;
    (*vertices)[3][1] = 0;
    (*vertices)[3][2] = 1;
    (*vertices)[4][1] = -1;
    (*vertices)[4][2] = 0;

    // 2690 LET FI=0:LET DX=255:LET NF=0
    *finished = 0;
    *distance_to_monster_x = 255;
    *character_facing = 0;
    // 2700 LET TX=0:LET TY=0:LET TF=0:LET TR=0
    *trap_coord_x = 0;
    *trap_coord_y = 0;
    *trapped = 0;
    // The value of TR set here is not used. It is overwritten when the
    // character is loaded.
    // 2710 LET MX=0:LET MY=0:LET DY=12:LET F$=""
    *monster_next_coord_x = 0;
    *monster_next_coord_y = 0;
    // The value of DY set here is never used.
    *char_code_hero = (char *) malloc(sizeof(char) * 7);
    if (*char_code_hero == NULL) {
        fprintf(stderr, "char_code_hero is NULL!\n");
        exit(1);
    }
    // 2720 LET NX=1:LET NY=1:LET RE=0:LET LT=0
    *character_coord_x = 1;
    *character_coord_y = 1;
    // RE is never actually used.
    *torches = 0;
    // 2730 FOR I = 1 TO 5
    for (index = 1; index <= 5; index += 1) {
    // 2740 LET F$=F$+CHR$(OS+I)
        (*char_code_hero)[index] = *dungeon_char_base + index;
    // 2750 NEXT I
    }
    (*char_code_hero)[6] = 0;
    // 2760 DATA69,117,73,121,81,129,69,117,73,121,81,129,89,137,97,145,101,149
    // 2770 FOR I=1 TO 18:READ T(I):NEXT I:GOSUB 2930
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
    lines2930_3200(
        character_char_base, char_code_blank, char_code_wall, char_code_vase,
        char_code_chest, char_code_idol, char_code_exit, char_code_trap,
        char_code_safe_place, *dungeon_char_base
    );
    // 2780 RETURN
}

void lines2790_2920(screen_t *screen, char *character_name, int screen_cols) {
    int index;
    // 2790 paper 1:CLS
    paper(screen->cursor, RED);
    clear_screen(screen);
    tab(screen->cursor, 1, 1);
    // 2800 paper 3:ink 0
    paper(screen->cursor, WHITE);
    ink(screen->cursor, BLACK);
    // 2810 PRINT C$;LEFT$(B$(W-LEN(C$));
    free(print_text(screen, character_name));
    print_left$_b$(screen, screen_cols - strlen(character_name));
    // 2820 paper 2:ink 3
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, WHITE);
    // 2830 FOR I=1 TO 5:PRINT LEFT$(B$,W);:NEXT I
    for (index = 1; index <= 5; index += 1) {
        print_left$_b$(screen, screen_cols);
        newline(screen->cursor);
    }
    // 2840 paper 0:ink 1
    paper(screen->cursor, BLACK);
    ink(screen->cursor, RED);
    // 2850 FOR I=1 TO 15:PRINT tab(1,5+I);LEFT$(B$,15);:NEXT I
    for (index = 1; index <= 15; index += 1) {
        tab(screen->cursor, 1, 5 + index);
        print_left$_b$(screen, 15);
    }
    // 2860 paper 1:ink 3
    paper(screen->cursor, RED);
    ink(screen->cursor, WHITE);
    // 2870 PRINT tab(16,7);"STR";
    tab(screen->cursor, 16, 7);
    free(print_text(screen, "STR"));
    // 2880 PRINT tab(16,10);"VIT";
    tab(screen->cursor, 16, 10);
    free(print_text(screen, "VIT"));
    // 2890 PRINT tab(16,13);"AUR";
    tab(screen->cursor, 16, 13);
    free(print_text(screen, "AUR"));
    // 2900 PRINT tab(16,16);"FACE";
    tab(screen->cursor, 16, 16);
    free(print_text(screen, "FACE"));
    // 2910 PRINT tab(16,19);"EXP";
    tab(screen->cursor, 16, 19);
    free(print_text(screen, "EXP"));
    // 2920 RETURN
}

void lines2930_3200(int *character_char_base, int *char_code_blank,
                    int *char_code_wall, int *char_code_vase,
                    int *char_code_chest, int *char_code_idol,
                    int *char_code_exit, int *char_code_trap,
                    int *char_code_safe_place, int dungeon_char_base) {
    // 2930 REM ** USER DEF'D CHARACTERS **
    // 2940 GOSUB 4000
    // Not needed due to dungeon_lib
    // 2950 FOR I=0 To 143:READ A:POKE 36532+I,255-A:NEXT I
    // 2960 VS=54272
    // 2970 POKE VS+24,15
    // 2980 POKE VS+5,9:POKE VS+6,0
    // 2990 POKE VS+12,9:POKE VS+13,0
    // 3000 POKE VS+7,0:POKE VS,0
    // Our font file handles the above
    // 3170 LET AS=65:LET CO=OS+6
    *character_char_base = 65;
    *char_code_blank = dungeon_char_base + 6;
    // 3180 LET C1=C0+1:LET C2=C0+2:LET C3=C0+3:LET C4=C0+4
    *char_code_wall = *char_code_blank + 1;
    *char_code_vase = *char_code_blank + 2;
    *char_code_chest = *char_code_blank + 3;
    *char_code_idol = *char_code_blank + 4;
    // 3190 LET C5=C0+6:LET C6=C0+7:LET C7=C0+8:LET C8=C0+12
    *char_code_exit = *char_code_blank + 6;
    *char_code_trap = *char_code_blank + 7;
    *char_code_safe_place = *char_code_blank + 8;
    // C8 is never acyually used.

    // 3200 RETURN
}
