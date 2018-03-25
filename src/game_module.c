#include <SDL.h>
#include <unistd.h>
#include "dungeon_lib.h"

void lines430_430(screen_t *screen, char *message, int W);
void lines480_560(screen_t *screen, double *attrs, char *char_code_hero,
                  int NF, int NX, int NY);
void lines570_610(screen_t *screen, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength, int *MT, int *MV,
                  int **R, int X, int Y);
void lines620_770(screen_t *screen, int char_code_blank, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength, int *MT, int *MV,
                  int *MX, int *MY, int NX, int NY, int O[25], int **R, int RH,
                  const char **T$, int W, const char **W$);
void lines810_860(screen_t *screen, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, char *char_code_hero, int *monster_coord_x,
                  int *monster_coord_y, int *monster_type,
                  int *monster_strength, int *MT, int *MV, int *NF, int NX,
                  int NY, int **R, int W, int X, int Y);
void lines870_930(screen_t *screen, int char_code_blank, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength, int *MT, int *MV,
                  int MX, int MY, int O[25], int **R, const char **T$, int W,
                  int X, int Y);
void lines990_1130(screen_t *screen, int char_code_blank, int char_code_vase,
                   int char_code_safe_place, int *distance_to_monster_x,
                   double *attrs, char *char_code_hero, int *monster_coord_x,
                   int *monster_coord_y, int *spells_remaining,
                   int *monster_type, int *monster_strength, int *MT, int *MV,
                   int MX, int MY, int NF, int *NX, int *NY, int O[25],
                   int **R, int RH, double S1, double S2, const char **T$,
                   int W);
void lines1410_1520(screen_t *screen, int char_code_blank, int char_code_wall,
                    int char_code_vase, int char_code_chest,
                    int char_code_idol, int char_code_safe_place,
                    int **vertices, int *distance_to_monster_x, double *attrs,
                    char *char_code_hero, int *finished, int gold,
                    int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength, int *MT, int *MV,
                    int *NF, int NX, int NY, int O[25], int **R, int *T,
                    int *TR);
void lines1550_1650(screen_t *screen, double *attrs, char *char_code_hero,
                    int *finished, int gold, int *monster_strength, int *NF,
                    int NX, int NY, int *T, int TR);
void lines1660_1680(double *attrs, int O[25], double S1, double S2);
void lines1690_1750(screen_t *screen, int char_code_vase,
                    int char_code_safe_place, int *distance_to_monster_x,
                    int *torches, int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength, int *MT, int *MV,
                    int NX, int NY, int **R, const char **T$, int W);
void lines1760_1950(screen_t *screen, char *character_name,
                    int *distance_to_monster_x, double *attrs,
                    int *dungeon_level, int *NX, int *NY, int OS, int *OX,
                    int *OY, int **R, double S3, const char **T$, int W);
void lines1770_1950(screen_t *screen, char *character_name,
                    int *distance_to_monster_x, double *attrs,
                    int *dungeon_level, int *NX, int *NY, int OS, int *OX,
                    int *OY, int **R, double S3, const char **T$, int W);
void lines2010_2250(screen_t *screen, int character_char_base,
                    char **character_name, double *attrs, int *gold,
                    int *torches, int *spells_remaining, int O[25], int *OT,
                    double *S1, double *S2, double *S3, const char **T$,
                    int *TR, int W);
void lines2260_2490(screen_t *screen, int character_char_base,
                    char *character_name, double *attrs, int *finished,
                    int gold, int dungeon_level, int NX, int NY, int O[25],
                    int OS, int OT, int **R, int TR, int W);
void lines2500_2780(int *character_char_base, int *char_code_blank,
                    int *char_code_wall, int *char_code_vase,
                    int *char_code_chest, int *char_code_idol,
                    int *char_code_way_in, int *char_code_exit,
                    int *char_code_safe_place, int ***vertices,
                    int *distance_to_monster_x, double **attrs,
                    char **char_code_hero, int *finished, int *torches,
                    int **spells_remaining, int *MX, int *MY, int *NF, int *NX,
                    int *NY, int *OS, int ***R, int **T, const char ***T$,
                    int *TF, int *TX, int *TY, int *W, const char ***W$);

int main(int argc, char *argv[]) {
    int character_char_base,
        char_code_blank,
        char_code_wall,  // Symbol for Wall
        char_code_vase,
        char_code_chest,
        char_code_idol,
        char_code_way_in,  // Symbol for Way In
        char_code_exit,  // Symbol for Exit
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
        MT,
        MV,
        MX,
        MY,
        NF,  // Facing. NESW
        NX,
        NY,
        O[25],
        OS,
        OT,
        OX,
        OY,
        ** R,
        RH,  // Object at NX / NY
        * T,
        TF,  // Flag to see if we can exit.
        TR,
        TX,
        TY,
        W,
        X,
        Y;
    double S1, S2, S3, * attrs;
    char * character_name = NULL,
         I$,
         * char_code_hero = NULL,
         * message;
    const char ** T$, **W$;
    // C64: 5 GOSUB 5000:POKE 53281,0
    screen_t *screen = init_screen();
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    clear_screen(screen);
    // lines 5000 on unneeded due to dungeon lib
    // 10 GOSUB2500
    lines2500_2780(
        &character_char_base, &char_code_blank, &char_code_wall,
        &char_code_vase, &char_code_chest, &char_code_idol, &char_code_way_in,
        &char_code_exit, &char_code_safe_place, &vertices,
        &distance_to_monster_x, &attrs, &char_code_hero, &finished, &torches,
        &spells_remaining, &MX, &MY, &NF, &NX, &NY, &OS, &R, &T, &T$, &TF, &TX,
        &TY, &W, &W$
    );
    // 20 GOSUB2010
    lines2010_2250(
        screen, character_char_base, &character_name, attrs, &gold, &torches,
        spells_remaining, O, &OT, &S1, &S2, &S3, T$, &TR, W
    );
    // 30 GOSUB1770
    lines1770_1950(
        screen, character_name, &distance_to_monster_x, attrs, &dungeon_level,
        &NX, &NY, OS, &OX, &OY, R, S3, T$, W
    );
    int game_over = 0;
    do {
        SDL_RenderPresent(screen->ren);
    // 40 LET I$=inkey$
        I$ = inkey$();
    // 50 IF I$="A" AND DX<255 THEN GOSUB870
        if (I$ == 'a' && distance_to_monster_x < 255 ) {
            lines870_930(
                screen, char_code_blank, char_code_vase, char_code_safe_place,
                &distance_to_monster_x, attrs, &monster_coord_x,
                &monster_coord_y, &monster_type, &monster_strength, &MT, &MV,
                MX, MY, O, R, T$, W, X, Y
            );
        }
    // 60 IF I$="C" AND F(7)>0 AND O(17)+O(18)>0 THEN GOSUB990
        if (I$ == 'c' && attrs[7] > 0 && O[17] + O[18] > 0) {
            lines990_1130(
                screen, char_code_blank, char_code_vase, char_code_safe_place,
                &distance_to_monster_x, attrs, char_code_hero,
                &monster_coord_x, &monster_coord_y, spells_remaining,
                &monster_type, &monster_strength, &MT, &MV, MX, MY, NF, &NX,
                &NY, O, R, RH, S1, S2, T$, W
            );
        }
    // 70 IF I$="G" THEN GOSUB1410
        if (I$ == 'g') {
            lines1410_1520(
                screen, char_code_blank, char_code_wall, char_code_vase,
                char_code_chest, char_code_idol, char_code_safe_place,
                vertices, &distance_to_monster_x, attrs, char_code_hero,
                &finished, gold, &monster_coord_x, &monster_coord_y,
                &monster_type, &monster_strength, &MT, &MV, &NF, NX, NY, O, R,
                T, &TR
            );
        }
    // 80 IF I$="P" THEN GOSUB1660
        if (I$ == 'p') {
            lines1660_1680(attrs, O, S1, S2);
        }
    // 90 IF I$="R" THEN GOSUB1690
        if (I$ == 'r') {
            lines1690_1750(
                screen, char_code_vase, char_code_safe_place,
                &distance_to_monster_x, &torches, &monster_coord_x,
                &monster_coord_y, &monster_type, &monster_strength, &MT, &MV,
                NX, NY, R, T$, W
            );
        }
    // 100 IF I$="S" THEN GOSUB2260
        if (I$ == 's') {
            lines2260_2490(
                screen, character_char_base, character_name, attrs, &finished,
                gold, dungeon_level, NX, NY, O, OS, OT, R, TR, W
            );
        }
    // 110 IF I$="B" THEN LET NF=NF-1
        if (I$ == 'b') {
            NF -= 1;
        }
    // 120 IF I$="N" THEN NF=NF+1
        if (I$ == 'n') {
            NF += 1;
        }
    // 130 IF NF>4 THEN LET NF=1
        if (NF > 4) {
            NF = 1;
        }
    // 140 IF NF<1 THEN LET NF=4
        if (NF < 1) {
            NF = 4;
        }
    // 150 IF I$="M" THEN LET NX=NX+D(NF,1): LET NY=NY+D(NF,2)
        if (I$ == 'm') {
            NX += vertices[NF][1];
            NY += vertices[NF][2];
        }
    // 160 IF NY>15 THEN LET NY=15
        if (NY > 15) {
            NY = 15;
        }
    // 170 IF NY<1 THEN LET NY=1
        if (NY < 1) {
            NY = 1;
        }
    // 180 IF NX<1 THEN LET NX=1
        if (NX <  1) {
            NX = 1;
        }
    // 190 IF NX>15 THEN LET NX=15
        if (NX > 15) {
            NX = 15;
        }
    // 200 LET RH=R(NX,NY)
        RH = R[NX][NY];
    // 210 IF RH=C1 THEN LET X=NX:LET Y=NY:GOSUB570:LET NX=OX:LET NY=OY:LET F(1)=F(1)-.03
        if (RH == char_code_wall) {
            X = NX;
            Y = NY;
            lines570_610(
                screen, char_code_vase, char_code_safe_place,
                &distance_to_monster_x, &monster_coord_x, &monster_coord_y,
                &monster_type, &monster_strength, &MT, &MV, R, X, Y
            );
            NX = OX;
            NY = OY;
            attrs[1] -= 0.03;
        }
    // 220 IF RH=C6 THEN LET TX=NX:LET TY=NY:LET TF=1
        if (RH == char_code_exit) {
            TX = NX;
            TY = NY;
            TF = 1;
        }
    // 230 IF TF=1 THEN LET NX=TX:LET NY=TY
        if (TF == 1) {
            NX = TX;
            NY = TY;
        }
    // 240 IF F(1) > S1*.8 AND rnd(8)<F(6) THEN LET TF=0
        if (attrs[1] > S1 * 0.8 && rand() % 8 < attrs[6]) {
            TF = 0;
        }
    // 250 IF I$>"" THEN LET F(1)=F(1)*0.99
        if (I$ != 0) {
            attrs[1] = attrs[1] * 0.99;
        }
    // 260 IF F(1) <S1 THEN LET F(1)=F(1)+(F(2)/1100)
        if (attrs[1] < S1) {
            attrs[1] += attrs[2] / 1100;
        }
    // 270 GOSUB480
        lines480_560(screen, attrs, char_code_hero, NF, NX, NY);
    // 280 IF OX<>NX OR OY<>NY THEN LET X=OX:LET Y=OY:GOSUB570
        if (OX != NX || OY != NY) {
            X = OX;
            Y = OY;
            lines570_610(
                screen, char_code_vase, char_code_safe_place,
                &distance_to_monster_x, &monster_coord_x, &monster_coord_y,
                &monster_type, &monster_strength, &MT, &MV, R, X, Y
            );
        }
    // 290 LET OX=NX:LET OY=NY
        OX = NX;
        OY = NY;
    // 300 IF DX<255 THEN GOSUB620
        if (distance_to_monster_x < 255) {
            lines620_770(
                screen, char_code_blank, char_code_vase, char_code_safe_place,
                &distance_to_monster_x, attrs, &monster_coord_x,
                &monster_coord_y, &monster_type, &monster_strength, &MT, &MV,
                &MX, &MY, NX, NY, O, R, RH, T$, W, W$
            );
        }
    // 310 IF F(1)>0 AND FI<1 AND RH<>C5 THEN GOTO 40
        if (attrs[1] > 0 && finished < 1 && RH != char_code_way_in) {
            game_over = 0;
        }
    // 320 IF RH=C5 THEN LET M$=T$(12):GOSUB430:GOSUB1760:GOTO40
        else if (RH == char_code_way_in) {
            message = (char *) malloc(sizeof(char) * (strlen(T$[12]) + 1));
            if (message == NULL) {
                fprintf(stderr, "message is NULL!\n");
                exit(1);
            }
            strcpy(message, T$[12]);
            message[strlen(T$[12])] = 0;
            lines430_430(screen, message, W);
            free(message);
            lines1760_1950(
                screen, character_name, &distance_to_monster_x, attrs,
                &dungeon_level, &NX, &NY, OS, &OX, &OY, R, S3, T$, W
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
            &monster_coord_y, &monster_type, &monster_strength, &MT, &MV, &NF,
            NX, NY, R, W, X, Y
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
        free(R[i]);
    }
    free(R);
    free(T);
    free(T$);
    free(W$);
    return 0;
}

void lines350_355(int sound_frequency) {
    // C64:
    // 350 POKE VS+1,J:POKE VS+4,33
    // 355 POKE VS+4,32:RETURN

    // TODO: SOUND!
}

void lines360_365(int sound_frequency) {
    // C64:
    // 360 POKE VS+8,J:POKE VS+11,129
    // 365 POKE VS+11,128:RETURN

    // TODO: SOUND!
}

void lines370_420(screen_t *screen, char *I$, char *message, int W) {
    // 370 paper 2:ink 0
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    // 380 PRINT tab(0,5):M$;
    tab(screen->cursor, 0, 5);
    free(print_text(screen, message));
    // 390 LET I$=inkey$
    // 400 IF I$="" THEN GOTO390
    SDL_RenderPresent(screen->ren);
    *I$ = inkey$();
    // 410 PRINT tab(0,5);LEFT(B$, W);:LET M$=""
    tab(screen->cursor, 0, 5);
    print_left$_b$(screen, W);
    // To decrease variable scope, we clear M$ elsewhere.
    // 420 RETURN
}

void lines440_470(screen_t *screen, char *message, int W);

void lines430_430(screen_t *screen, char *message, int W) {
    // 430 paper 2:ink 0
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    lines440_470(screen, message, W);
}

void lines440_470(screen_t *screen, char *message, int W) {
    // 440 PRINT tab(0,5);M$;
    tab(screen->cursor, 0, 5);
    free(print_text(screen, message));
    SDL_RenderPresent(screen->ren);
    // 450 FOR D=1 TO 600:NEXT D
    // C64: 450 FOR D=1 TO 200:NEXT D
    sleep(0.34);
    // 460 PRINT tab(0,5);LEFT$(B$,W);:LET M$=""
    tab(screen->cursor, 0, 5);
    print_left$_b$(screen, W);
    // To decrease variable scope, we clear M$ elsewhere.
    // 470 RETURN
}

void lines480_560(screen_t *screen, double *attrs, char *char_code_hero,
                  int NF, int NX, int NY) {
    // 480 paper 1:ink 3
    paper(screen->cursor, RED);
    ink(screen->cursor, WHITE);
    // 490 PRINT tab(NX,NY+5);MID$(F$,NF,1);
    tab(screen->cursor, NX, NY + 5);
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    outstring[0] = char_code_hero[NF];
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
    outstring[0] = "NESW."[NF - 1];
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

void lines570_610(screen_t *screen, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength, int *MT, int *MV,
                  int **R, int X, int Y) {
    int RM;
    // 570 paper 1:ink 2
    paper(screen->cursor, RED);
    ink(screen->cursor, YELLOW);
    // 580 LET RM=R(X,Y):PRINT tab(X,Y+5);CHR$(RM);
    RM = R[X][Y];
    tab(screen->cursor, X, Y + 5);
    char * outstring = (char *) malloc(sizeof(char) * 2);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    sprintf(outstring, "%c", (char) RM);
    free(print_text(screen, outstring));
    free(outstring);
    // 590 IF ABS(DX)<4 OR RM<=C7 THEN RETURN
    if (abs(*distance_to_monster_x) < 4 || RM <= char_code_safe_place) {
        return;
    }
    // 600 LET MT=RM:LET M=MT-C2:LET MV=M/16:LET MS=M*6:LET DX=3:LET LX=X:LET LY=Y
    *MT = RM;
    *monster_type = *MT - char_code_vase;
    *MV = *monster_type / 16;
    *monster_strength = *monster_type * 6;
    *distance_to_monster_x = 3;
    *monster_coord_x = X;
    *monster_coord_y = Y;
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
                  int *monster_broke_item, int O[25], const char **T$, int W,
                  const char **W$);

void lines620_770(screen_t *screen, int char_code_blank, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength, int *MT, int *MV,
                  int *MX, int *MY, int NX, int NY, int O[25], int **R, int RH,
                  const char **T$, int W, const char **W$) {
    int distance_to_monster_y, damage, item_num, sound_frequency,
        monster_broke_item, RM, SX, SY, X, Y;
    char * message;
    // 620 LET DX=LX-NX:LET SX=SGN(DX):LET DY=LY-NY:LET SY=SGN(DY)
    *distance_to_monster_x = *monster_coord_x - NX;
    SX = sign(*distance_to_monster_x);
    distance_to_monster_y = *monster_coord_y - NY;
    SY = sign(distance_to_monster_y);
    // 630 LET MX=LX-(MV*SX):LET MY=LY-(MV*SY):LET RM=R(MX,MY)
    *MX = *monster_coord_x - (*MV * SX);
    *MY = *monster_coord_y - (*MV * SY);
    RM = R[*MX][*MY];
    // 640 IF RM>C0 AND RM<>MT THEN LET MY=LY:LET MX=LX
    if (RM > char_code_blank && RM != *MT) {
        *MY = *monster_coord_y;
        *MX = *monster_coord_x;
    }
    // 650 LET R(LX,LY)=C0:LET X=LX:LET Y=LY:GOSUB 570
    R[*monster_coord_x][*monster_coord_y] = char_code_blank;
    X = *monster_coord_x;
    Y = *monster_coord_y;
    lines570_610(
        screen, char_code_vase, char_code_safe_place, distance_to_monster_x,
        monster_coord_x, monster_coord_y, monster_type, monster_strength, MT,
        MV, R, X, Y
    );
    // 660 LET R(MX,MY)=MT:LET X=MX:LET Y=MY:GOSUB 570
    R[*MX][*MY] = *MT;
    X = *MX;
    Y = *MY;
    lines570_610(
        screen, char_code_vase, char_code_safe_place, distance_to_monster_x,
        monster_coord_x, monster_coord_y, monster_type, monster_strength, MT,
        MV, R, X, Y
    );
    // 670 LET LX=MX:LET LY=MY:LET H=0
    *monster_coord_x = *MX;
    *monster_coord_y = *MY;
    damage = 0;
    // 680 IF ABS(DX)<=1 AND ABS(DY)<=1 AND RH<>C7 THEN LET H=M*.5:LET J=H:GOSUB350
    if (
            abs(*distance_to_monster_x) <= 1 &&
            abs(distance_to_monster_y) <= 1 &&
            RH != char_code_safe_place
    ) {
        damage = *monster_type * 0.5;
        sound_frequency = damage;
        lines350_355(sound_frequency);
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
    message = (char *) malloc(sizeof(char) * (strlen(T$[5]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, T$[5]);
    lines430_430(screen, message, W);
    free(message);
    lines360_365(sound_frequency);
    // 710 LET H=H/(3+O(9) + O(10) + O(11) + O(12) + O(13) + O(14))
    damage /= (3 + O[9] + O[10] + O[11] + O[12] + O[13] + O[14]);
    // 720 LET F(1)=F(1)-H:LET F(2)=F(2)-(H/101)
    attrs[1] -= damage;
    attrs[2] -= damage / 101;
    // 730 LET I=1:LET WB=0:LET MB=rnd(M)
    item_num = 1;
    // WB is ony set here and not used anywhere.
    monster_broke_item = rand() % *monster_type;
    // 740 LET J=MT:GOSUB350:GOSUB360
    sound_frequency = *MT;
    lines350_355(sound_frequency);
    lines360_365(sound_frequency);
    // 750 IF MB=1 AND O(I)>0 THEN GOSUB780
    int done = 0;
    do {
        if (monster_broke_item == 1 && O[item_num] > 0) {
            lines780_800(
                screen, item_num, sound_frequency, &monster_broke_item, O, T$,
                W, W$
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
                  int *monster_broke_item, int O[25], const char **T$, int W,
                  const char **W$) {
    char * message;
    // 780 LET O(I)=0:LET M$=T$(8)+" "+W$(I):GOSUB430
    O[item_num] = 0;
    message = (char *) malloc(
        sizeof(char) * (strlen(T$[8]) + strlen(W$[item_num]) + 2)
    );
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    sprintf(message, "%s %s", T$[8], W$[item_num]);
    lines430_430(screen, message, W);
    free(message);
    // 790 LET MB=0:GOSUB360:LET J=I:GOSUB350
    *monster_broke_item = 0;
    lines360_365(sound_frequency);
    sound_frequency = item_num;
    lines350_355(sound_frequency);
    // 800 RETURN
}

void lines810_860(screen_t *screen, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, char *char_code_hero, int *monster_coord_x,
                  int *monster_coord_y, int *monster_type,
                  int *monster_strength, int *MT, int *MV, int *NF, int NX,
                  int NY, int **R, int W, int X, int Y) {
    char * message;
    int sound_frequency;
    // 810 LET NF=5;LET F(1)=0:GOSUB 440
    *NF = 5;
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
    lines440_470(screen, message, W);
    free(message);
    // 820 PRINT tab(1,5);"THOU HAST EXPIRED!"
    tab(screen->cursor, 1, 5);
    free(print_text(screen, "THOU HAST EXPIRED!"));
    // 830 FOR J=150 TO 1 STEP-4
    for (sound_frequency = 150; sound_frequency >= 1; sound_frequency -= 4) {
    // 840 GOSUB350:GOSUB360:GOSUB570:GOSUB480
        lines350_355(sound_frequency);
        lines360_365(sound_frequency);
        lines570_610(
            screen, char_code_vase, char_code_safe_place,
            distance_to_monster_x, monster_coord_x, monster_coord_y,
            monster_type, monster_strength, MT, MV, R, X, Y
        );
        lines480_560(screen, attrs, char_code_hero, *NF, NX, NY);
    // 850 NEXT J
    }
    // 860 RETURN
}

void lines940_980(screen_t *screen, int char_code_blank, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength, int *MT, int *MV,
                  int MX, int MY, int **R, const char **T$, int W, int X,
                  int Y);

void lines870_930(screen_t *screen, int char_code_blank, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength, int *MT, int *MV,
                  int MX, int MY, int O[25], int **R, const char **T$, int W,
                  int X, int Y) {
    // 870 LET M$=T$(rnd(3)):GOSUB360
    int damage, t$_ind = rand() % 3 + 1;
    char * message;
    message = (char *) malloc(sizeof(char) * (strlen(T$[t$_ind]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, T$[t$_ind]);
    /*
    The original code did not define sound_frequency before calling the
    subroutine at line 360 here. Since the value of sound_frequency is not
    easily predicted, we just use 100.
    */
    lines360_365(100);
    free(message);
    // 880 LET H=F(1)+O(1) + O(2) + O(3) + O(4) + O(5) + O(6) + O(7) + O(8) + rnd(F(6))
    damage =
        attrs[1] + O[1] + O[2] + O[3] + O[4] + O[5] + O[6] + O[7] + O[8] + \
        (rand() * attrs[6] / RAND_MAX);
    // 890 IF F(3)+F(6)< rnd(M)+2 THEN LET M$=T$(4):LET HT=0
    if (attrs[3] + attrs[6] < rand() % *monster_type + 2) {
        message = (char *) malloc(sizeof(char) * (strlen(T$[4]) + 1));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, T$[4]);
        // Probably a bug in the original game: HT is never set or referenced
        // anywhere else. It should be H instead.
        damage = 0;
    }
    // 900 LET MS=MS-H:GOSUB430
    *monster_strength -= damage;
    lines430_430(screen, message, W);
    free(message);
    // 910 LET F(1)=F(1)-(H/100):LET F(5)=F(5)+0.05
    attrs[1] -= damage / 100;
    attrs[5] += 0.05;
    // 920 IF MS<1 THEN GOSUB940
    if (*monster_strength < 1) {
        lines940_980(
            screen, char_code_blank, char_code_vase, char_code_safe_place,
            distance_to_monster_x, attrs, monster_coord_x, monster_coord_y,
            monster_type, monster_strength, MT, MV, MX, MY, R, T$, W, X, Y
        );
    }
    // 930 RETURN
}

void lines940_980(screen_t *screen, int char_code_blank, int char_code_vase,
                  int char_code_safe_place, int *distance_to_monster_x,
                  double *attrs, int *monster_coord_x, int *monster_coord_y,
                  int *monster_type, int *monster_strength, int *MT, int *MV,
                  int MX, int MY, int **R, const char **T$, int W, int X,
                  int Y) {
    char * message;
    int sound_frequency;
    // 940 LET DX=255:LET MS=0:LET R(MX,MY)=C0
    *distance_to_monster_x = 255;
    *monster_strength = 0;
    R[MX][MY] = char_code_blank;
    // 950 LET F(5)=F(5)+.1
    attrs[5] += 0.1;
    // 960 LET M$=T$(6):GOSUB430
    message = (char *) malloc(sizeof(char) * (strlen(T$[6]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, T$[6]);
    lines430_430(screen, message, W);
    free(message);
    // 970 FOR J=200 TO 150STEP-8:GOSUB350:GOSUB360:NEXT J
    for (sound_frequency = 200; sound_frequency >= 150; sound_frequency -= 8) {
        lines350_355(sound_frequency);
        lines360_365(sound_frequency);
    }
    // 980 GOSUB570:RETURN
    lines570_610(
        screen, char_code_vase, char_code_safe_place, distance_to_monster_x,
        monster_coord_x, monster_coord_y, monster_type, monster_strength, MT,
        MV, R, X, Y
    );
}

void lines1140_1180(screen_t *screen, int char_code_blank, int char_code_vase,
                    int char_code_safe_place, int *distance_to_monster_x,
                    double *attrs, int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength, int *MT, int *MV,
                    int MX, int MY, int **R, const char **T$, int W);
void lines1190_1210(int char_code_blank, int char_code_safe_place, int NX,
                    int NY, int **R, int RH);
void lines1220_1270(screen_t *screen, double *attrs, char *char_code_hero,
                    int NF, int *NX, int *NY);
void lines1280_1290(double *attrs, int *spells_remaining, int SL);
void lines1300_1380(screen_t *screen, int char_code_blank, int char_code_vase,
                    int char_code_safe_place, int *distance_to_monster_x,
                    int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength, int *MT, int *MV,
                    int NX, int NY, int **R, int RH, int X, int Y);
void lines1390_1400(double *attrs, double S1, double S2);

void lines990_1130(screen_t *screen, int char_code_blank, int char_code_vase,
                   int char_code_safe_place, int *distance_to_monster_x,
                   double *attrs, char *char_code_hero, int *monster_coord_x,
                   int *monster_coord_y, int *spells_remaining,
                   int *monster_type, int *monster_strength, int *MT, int *MV,
                   int MX, int MY, int NF, int *NX, int *NY, int O[25],
                   int **R, int RH, double S1, double S2, const char **T$,
                   int W) {
    int row_num, SL, X, Y;
    char I$, * message;
    // 990 GOSUB480:paper 2: ink 0
    lines480_560(screen, attrs, char_code_hero, NF, *NX, *NY);
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    // 1000 PRINT tab(0,10);"YOU MAY USE MAGICKS";
    tab(screen->cursor, 0, 1);
    free(print_text(screen, "YOU MAY USE MAGICKS"));
    // 1010 IF O(17)>0 THEN PRINT tab(0,2);"FROM NECRONOMICON";
    if (O[17] > 0) {
        tab(screen->cursor, 0, 2);
        free(print_text(screen, "FROM NECRONOMICON"));
    }
    // 1020 IF O(18)>0 THEN PRINT tab(0,3);"FROM THE SCROLLS";
    if (O[18] > 0) {
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
        lines370_420(screen, &I$, message, W);
        free(message);
        char * outstring = (char *) malloc(sizeof(char) * 2);
        sprintf(outstring, "%c", I$);
    // 1050 LET SL=VAL(I$)
        SL = atoi(outstring);
        free(outstring);
    // 1060 IF SL=0 OR (O(17)=0 AND SL<5) OR (O(18)=0 AND SL>3) OR SL>6 THEN GOTO 1040
    } while (
            SL == 0 ||
            (O[17] == 0 && SL < 5) ||
            (O[19] == 0 && SL > 3) ||
            SL > 6
    );

    // 1070 LET M(SL)=M(SL)-1:LET X=NX:LET Y=NY
    spells_remaining[SL] -= 1;
    X = *NX;
    Y = *NY;
    // 1080 IF M(SL)<0 THEN LET M$=T$(9):LET SL=7
    if (spells_remaining[SL] < 0) {
        message = (char *) malloc(sizeof(char) * (strlen(T$[9]) + 1));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, T$[9]);
        SL = 7;
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
        print_left$_b$(screen, W);
        newline(screen->cursor);
    }
    lines570_610(
        screen, char_code_vase, char_code_safe_place, distance_to_monster_x,
        monster_coord_x, monster_coord_y, monster_type, monster_strength, MT,
        MV, R, X, Y
    );
    // 1100 ON SL GOSUB1140,1190,1220,1280,1300,1390,1130
    switch (SL) {
        case 1:
            lines1140_1180(
                screen, char_code_blank, char_code_vase, char_code_safe_place,
                distance_to_monster_x, attrs, monster_coord_x, monster_coord_y,
                monster_type, monster_strength, MT, MV, MX, MY, R, T$, W
            );
            break;
        case 2:
            lines1190_1210(
                char_code_blank, char_code_safe_place, *NX, *NY, R, RH
            );
            break;
        case 3:
            lines1220_1270(screen, attrs, char_code_hero, NF, NX, NY);
            break;
        case 4:
            lines1280_1290(attrs, spells_remaining, SL);
            break;
        case 5:
            lines1300_1380(
                screen, char_code_blank, char_code_vase, char_code_safe_place,
                distance_to_monster_x, monster_coord_x, monster_coord_y,
                monster_type, monster_strength, MT, MV, *NX, *NY, R, RH, X, Y
            );
            break;
        case 6:
            lines1390_1400(attrs, S1, S2);
            break;
        case 7:
            // Line 1130 is just a return statement.
            break;
    }
    // 1110 LET F(5)=F(5)+.2
    attrs[5] += 0.2;
    // 1120 GOSUB430
    lines430_430(screen, message, W);
    free(message);
    // 1130 RETURN
}

void lines1140_1180(screen_t *screen, int char_code_blank, int char_code_vase,
                    int char_code_safe_place, int *distance_to_monster_x,
                    double *attrs, int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength, int *MT, int *MV,
                    int MX, int MY, int **R, const char **T$, int W) {
    int sound_frequency, X, Y;
    // 1140 FOR J=1 TO 12
    for (sound_frequency = 1; sound_frequency <= 12; sound_frequency += 1) {
    // 1150 GOSUB350:GOSUB360
        lines350_355(sound_frequency);
        lines360_365(sound_frequency);
    // 1160 NEXT J
    }
    // 1170 IF DX<255 THEN LET X=MX:LET Y=MY:GOSUB940
    if (*distance_to_monster_x < 255) {
        X = MX;
        Y = MY;
        lines940_980(
            screen, char_code_blank, char_code_vase, char_code_safe_place,
            distance_to_monster_x, attrs, monster_coord_x, monster_coord_y,
            monster_type, monster_strength, MT, MV, MX, MY, R, T$, W, X, Y
        );
    }
    // 1180 RETURN
}

void lines1190_1210(int char_code_blank, int char_code_safe_place, int NX,
                    int NY, int **R, int RH) {
    int sound_frequency;
    // 1190 IF RH=C0 THEN LET R(NX,NY)=C7
    if (RH == char_code_blank) {
        R[NX][NY] = char_code_safe_place;
    }
    // 1200 LET J=100:GOSUB350:LET J=200:GOSUB350
    sound_frequency = 100;
    lines350_355(sound_frequency);
    sound_frequency = 200;
    lines350_355(sound_frequency);
    // 1210 RETURN
}

void lines1220_1270(screen_t *screen, double *attrs, char *char_code_hero,
                    int NF, int *NX, int *NY) {
    int sound_frequency;
    // 1220 LET NX=rnd(13):LET NY=rnd(13)
    *NX = rand() % 13;
    *NY = rand() % 13;
    // 1230 FOR J=0 TO 255 STEP8
    for (sound_frequency = 0; sound_frequency <= 255; sound_frequency += 8) {
    // 1240 GOSUB360:GOSUB350
        lines360_365(sound_frequency);
        lines350_355(sound_frequency);
    // 1250 NEXT J
    }
    // 1260 GOSUB480
    lines480_560(screen, attrs, char_code_hero, NF, *NX, *NY);
    // 1270 RETURN
}

void lines1280_1290(double *attrs, int *spells_remaining, int SL) {
    // 1280 LET F(2)=F(2)+rnd(M(SL)):LET F(1)=F(1)+rnd(M(SL)):LET F(7)=F(7)-1
    attrs[2] += rand() * spells_remaining[SL];
    attrs[1] += rand() * spells_remaining[SL];
    attrs[7] -= 1;
    // 1290 RETURN
}

void lines1300_1380(screen_t *screen, int char_code_blank, int char_code_vase,
                    int char_code_safe_place, int *distance_to_monster_x,
                    int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength, int *MT, int *MV,
                    int NX, int NY, int **R, int RH, int X, int Y) {
    int sound_frequency;
    // 1300 FOR J=1 TO 30
    for (sound_frequency = 1; sound_frequency <= 30; sound_frequency += 1) {
    // 1310 LET R(NX,NY)=rnd(8)+1+C0
        R[NX][NY] = rand() % 8 + 1 + char_code_blank;
    // 1320 GOSUB350:GOSUB570
        lines350_355(sound_frequency);
        lines570_610(
            screen, char_code_vase, char_code_safe_place,
            distance_to_monster_x, monster_coord_x, monster_coord_y,
            monster_type, monster_strength, MT, MV, R, X, Y
        );
    // 1330 NEXT J
    }
    // 1340 IF RH<=C7 THEN LET DX=255:LET MS=0
    if (RH <= char_code_safe_place) {
        *distance_to_monster_x = 255;
        *monster_strength = 0;
    }
    // 1350 FOR J = 1 TO 2O STEP4
    for (sound_frequency = 1; sound_frequency <= 20; sound_frequency += 4) {
    // 1360 GOSUB 350
        lines350_355(sound_frequency);
    // 1370 NEXT J
    }
    // 1380 RETURN
}

void lines1390_1400(double *attrs, double S1, double S2) {
    // 1390 LET F(2)=S2:LET F(1)=S1:LET F(7)=F(7)-1
    attrs[2] = S2;
    attrs[1] = S1;
    attrs[7] -= 1;
    // 1400 RETURN
}

void lines1410_1520(screen_t *screen, int char_code_blank, int char_code_wall,
                    int char_code_vase, int char_code_chest,
                    int char_code_idol, int char_code_safe_place,
                    int **vertices, int *distance_to_monster_x, double *attrs,
                    char *char_code_hero, int *finished, int gold,
                    int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength, int *MT, int *MV,
                    int *NF, int NX, int NY, int O[25], int **R, int *T,
                    int *TR) {
    int sound_frequency, item_to_get, item_to_get_coord_x, item_to_get_coord_y,
        X, Y;
    // 1410 LET GX=NX+D(NF,1):LET GY=NY+D(NF,2)
    item_to_get_coord_x = NX + vertices[*NF][1];
    item_to_get_coord_y = NY + vertices[*NF][2];
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
    item_to_get = R[item_to_get_coord_x][item_to_get_coord_y];
    if (item_to_get > char_code_wall && item_to_get < char_code_idol) {
        R[item_to_get_coord_x][item_to_get_coord_y] = char_code_blank;
    }
    // 1470 IF GT=C2 THEN LET O(23)=O(23)+1:LET O(24)=O(24)+1
    if (item_to_get == char_code_vase) {
        O[23] += 1;
        O[24] += 1;
    }
    // 1480 IF GT=C3 THEN LET TR=TR+1
    if (item_to_get == char_code_chest) {
        TR += 1;
    }
    // 1490 IF GT=C4 THEN GOSUB 1550
    if (item_to_get == char_code_idol) {
        lines1550_1650(
            screen, attrs, char_code_hero, finished, gold, monster_strength,
            NF, NX, NY, T, *TR
        );
    }
    // 1500 LET X=GX:LET Y=GY:GOSUB570
    X = item_to_get_coord_x;
    Y = item_to_get_coord_y;
    lines570_610(
        screen, char_code_vase, char_code_safe_place, distance_to_monster_x,
        monster_coord_x, monster_coord_y, monster_type, monster_strength, MT,
        MV, R, X, Y
    );
    // 1510 IF GT>C1 AND GT<C4 THEN LET J=GT:GOSUB350:LET J=GT+5:GOSUB350
    if (item_to_get > char_code_wall && item_to_get < char_code_idol) {
        sound_frequency = item_to_get;
        lines350_355(sound_frequency);
        sound_frequency = item_to_get + 5;
        lines350_355(sound_frequency);
    }
    // 1520 RETURN
}

void lines1550_1650(screen_t *screen, double *attrs, char *char_code_hero,
                    int *finished, int gold, int *monster_strength, int *NF,
                    int NX, int NY, int *T, int TR) {
    int index, sound_frequency, N;
    // 1550 paper 2:ink 1
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, RED);
    // 1560 PRINT tab(0,1);" THY QUEST IS OVER! "
    tab(screen->cursor, 0, 1);
    free(print_text(screen, " THY QUEST IS OVER! "));
    // 1570 FOR I = 1 TO 18
    for (index = 1; index <= 18; index += 1) {
    // 1580 LET J=T(I):GOSUB350
        sound_frequency = T[index];
        lines350_355(sound_frequency);

    // 1590 LET X=NX:LET Y=NY
        // X and Y are overwritten before the above values are used.
    // 1600 FOR N=1 TO 4:LET NF=N:GOSUB480:NEXT N
        for (N = 1; N <=4; N += 1) {
            *NF = N;
            lines480_560(screen, attrs, char_code_hero, *NF, NX, NY);
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
        (int) ((TR * 10) + (gold * attrs[5]) + attrs[1] + attrs[2] + attrs[3])
    );
    free(print_text(screen, outstring));
    free(outstring);
    // 1640 LET FI=1
    *finished = 1;
    // 1650 RETURN
}

void lines1660_1680(double *attrs, int O[25], double S1, double S2) {
    // 1660 IF O(24)>0 AND F(1)<S1 THEN LET F(1)=S1:LET O(24)=O(24)-1
    if (O[24] > 0 && attrs[1] < S1) {
        attrs[1] = S1;
        O[24] -= 1;
    }
    // 1670 IF O(23)>0 AND F(2)<S2 THEN LET F(2)=S2:LET O(23)=O(23)-1
    if (O[23] > 0 && attrs[2] < S2) {
        attrs[2] = S2;
        O[23] -= 1;
    }
    // 1680 RETURN
}

void lines1690_1750(screen_t *screen, int char_code_vase,
                    int char_code_safe_place, int *distance_to_monster_x,
                    int *torches, int *monster_coord_x, int *monster_coord_y,
                    int *monster_type, int *monster_strength, int *MT, int *MV,
                    int NX, int NY, int **R, const char **T$, int W) {
    int X, Y;
    char * message;
    // 1690 IF LT=0 THEN LET M$=T$(7):GOSUB430:RETURN
    if (*torches == 0) {
        message = (char *) malloc(sizeof(char) * (strlen(T$[7]) + 1));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, T$[7]);
        lines430_430(screen, message, W);
        free(message);
        return;
    }
    // 1700 FOR Y=NY-3 TO NY+3
    for (Y = NY - 3; Y <= NY + 3; Y += 1) {
    // 1710 FOR X=NX-3 TO NX+3
        for (X = NX - 3; X <= NX + 3; X += 1) {
    // 1720 IF (X>0 AND X<16) AND (Y>0 AND Y<16)THEN GOSUB570
            if (X > 0 && X < 16 && Y > 0 && Y < 16) {
                lines570_610(
                    screen, char_code_vase, char_code_safe_place,
                    distance_to_monster_x, monster_coord_x, monster_coord_y,
                    monster_type, monster_strength, MT, MV, R, X, Y
                );
            }
    // 1730 NEXT X:NEXT Y
        }
    }
    // 1740 LET LT=LT-1
    *torches -= 1;
    // 1750 RETURN
}

void lines370_420(screen_t *screen, char *I$, char *message, int W);
void lines1960_2000(screen_t *screen, double *attrs);
void lines2790_2920(screen_t *screen, char *character_name, int W);

void lines1760_1770_1950(screen_t *screen, int start_at_1770,
                         char *character_name, int *distance_to_monster_x,
                         double *attrs, int *dungeon_level, int *NX, int *NY,
                         int OS, int *OX, int *OY, int **R, double S3,
                         const char **T$, int W) {
    // The original BASIC code sometimes used 'GOSUB 1760' and sometimes
    // 'GOSUB 1770'. This is further complicated by their use of a
    // 'GOTO 1760' towards the end.
    // We use the 'start_at_1770' flag to handle this.
    int correct_level_loaded, index, entrance_coord_x, entrance_coord_y, X, Y;
    char I$, * message;
    do {

    // 1760 IF F(5)<S3+1 THEN LET M$=T$(11):LET NX=OX:LET NY=OY:GOSUB 430:RETURN
        if (!start_at_1770 && attrs[5] < S3 + 1) {
            message = (char *) malloc(sizeof(char) * (strlen(T$[11]) + 1));
            if (message == NULL) {
                fprintf(stderr, "message is NULL!\n");
                exit(1);
            }
            strcpy(message, T$[11]);
            *NX = *OX;
            *NY = *OY;
            lines430_430(screen, message, W);
            free(message);
            return;
        }
        start_at_1770 = 0;
    // 1770 CLS:PRINT tab(0,3);"PREPARE DUNGEON TAPE"
        clear_screen(screen);
        tab(screen->cursor, 0, 3);
        free(print_text(screen, "PREPARE DUNGEON TAPE"));
    // 1780 LET M$=T$(10):GOSUB370
        message = (char *) malloc(sizeof(char) * (strlen(T$[10]) + 1));
        if (message == NULL) {
            fprintf(stderr, "message is NULL!\n");
            exit(1);
        }
        strcpy(message, T$[10]);
        lines370_420(screen, &I$, message, W);
        free(message);
        size_t filesize;
    // 1790 S=OPENIN"LEVEL"
        FILE *S = fopen("LEVEL", "r");
    // 1800 INPUT#S,S$
        fseek(S, 0, SEEK_END);
        // TODO: Check for fseek errors.
        filesize = ftell(S);
        fseek(S, 0, SEEK_SET);
        char * S$ = (char *) malloc(sizeof(char) * (filesize + 1));
        if (S$ == NULL) {
            fprintf(stderr, "S$ is NULL!\n");
            exit(1);
        }
        fread(S$, 1, filesize, S);
    // 1810 CLOSE#S
        fclose(S);
    // 1820 LET I=1
        index = 1;
    // 1830 FOR Y=1 TO 15
        for (Y = 1; Y <= 15; Y += 1) {
    // 1840 FOR X=1 TO 15
            for (X = 1; X <= 15; X += 1) {
    // 1850 LET R(X,Y)=ASC(MID$(S$,I,1))
                R[X][Y] = (int) S$[index - 1];
    // 1860 LET I=I+1
                index += 1;
    // 1870 NEXT X
            }
    // 1880 NEXT Y
        }
    // 1890 LET IX=ASC(MID$(S$,I,1))-OS
        entrance_coord_x = (int) S$[index - 1] - OS;
    // 1900 LET IY=ASC(MID$(S$,I+1,1))-OS
        entrance_coord_y = (int) S$[index] - OS;
    // 1910 LET LE=ASC(MID$(S$,I+2,1))-OS
        *dungeon_level = (int) S$[index + 1] - OS;
    // 1920 IF LE>F(5) THEN GOSUB 1960:GOTO 1760
        if (*dungeon_level > attrs[5]) {
            lines1960_2000(screen, attrs);
            correct_level_loaded = 1;
        } else {
            correct_level_loaded = 0;
        }
    } while (correct_level_loaded);
    // 1930 GOSUB2790
    lines2790_2920(screen, character_name, W);
    // 1940 LET NX=IX:LET NY=IY:LET OX=NX:LET OY=NY:LET DX=255
    *NX = entrance_coord_x;
    *NY = entrance_coord_y;
    *OX = *NX;
    *OY = *NY;
    *distance_to_monster_x = 255;
    // 1950 RETURN
}

void lines1760_1950(screen_t *screen, char *character_name,
                    int *distance_to_monster_x, double *attrs,
                    int *dungeon_level, int *NX, int *NY, int OS, int *OX,
                    int *OY, int **R, double S3, const char **T$, int W) {
    lines1760_1770_1950(
        screen, 0, character_name, distance_to_monster_x, attrs, dungeon_level,
        NX, NY, OS, OX, OY, R, S3, T$, W
    );
}

void lines1770_1950(screen_t *screen, char *character_name,
                    int *distance_to_monster_x, double *attrs,
                    int *dungeon_level, int *NX, int *NY, int OS, int *OX,
                    int *OY, int **R, double S3, const char **T$, int W) {
    lines1760_1770_1950(
        screen, 1, character_name, distance_to_monster_x, attrs, dungeon_level,
        NX, NY, OS, OX, OY, R, S3, T$, W
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
                    int *torches, int *spells_remaining, int O[25], int *OT,
                    double *S1, double *S2, double *S3, const char **T$,
                    int *TR, int W) {
    char I$, * message;
    int index, subindex, P;
    // 2010 CLS:PRINT tab(0,3);"PREPARE HERO TAPE"
    clear_screen(screen);
    tab(screen->cursor, 0, 3);
    free(print_text(screen, "PREPARE HERO TAPE"));
    // 2020 LET M$=T$(10):GOSUB370
    message = (char *) malloc(sizeof(char) * (strlen(T$[10]) + 1));
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, T$[10]);
    lines370_420(screen, &I$, message, W);
    free(message);
    // 2030 S=OPENIN "HERO"
    FILE *S = fopen("HERO", "r");
    // 2040 INPUT#S,S$
    size_t filesize;
    fseek(S, 0, SEEK_END);
    // TODO: Check for fseek errors.
    filesize = ftell(S);
    fseek(S, 0, SEEK_SET);
    char * S$ = (char *) malloc(sizeof(char) * (filesize + 1));
    if (S$ == NULL) {
        fprintf(stderr, "S$ is NULL!\n");
        exit(1);
    }
    fread(S$, 1, filesize, S);
    // 2050 CLOSE#S
    fclose(S);
    free(S);
    S$[filesize] = 0;
    // 2060 LET P=2
    P = 2;
    // 2070 LET OT=ASC(MID$(S$,1,1))-AS
    *OT = (int) S$[0] - character_char_base;
    // 2080 FOR I= 1 TO 8
    for (index = 1; index <= 8; index += 1) {
    // 2090 LET F(I)=ASC(MID$(S$,P,1)) - AS
        attrs[index] = (int) S$[P - 1] - character_char_base;
    // 2100 LET P=P+1
        P += 1;
    // 2110 NEXT I
    }
    // 2120 FOR I=1 TO OT
    for (index = 1; index <= *OT; index += 1) {
    // 2130 LET O(I)=ASC(MID$(S$,P,1))-AS
        O[index] = (int) S$[P - 1] - character_char_base;
    // 2140 LET P=P+1
        P += 1;
    // 2150 NEXT I
    }
    // 2160 LET GC=ASC(MID$(S$,P,1))-AS
    *gold = (int) S$[P - 1] - character_char_base;
    // 2170 LET TR=ASC(MID$(S$,P+1,1))-AS
    *TR = (int) S$[P] - character_char_base;
    // 2180 LET C$=RIGHT$(S$,LEN(S$)-(P+1))
    *character_name = (char *) malloc(sizeof(char) * (P + 2));
    if (*character_name == NULL) {
        fprintf(stderr, "character_name is NULL!\n");
        exit(1);
    }
    strcpy(*character_name, S$ + P + 1);
    // 2190 LET S1=F(1):LET S2=F(2):LET S3=F(5)
    *S1 = attrs[1];
    *S2 = attrs[2];
    *S3 = attrs[5];
    // 2200 FOR I=1 TO 2
    for (index = 1; index <= 2; index += 1) {
    // 2210 FOR J=1 TO 3
        for (subindex = 1; subindex <= 3; subindex += 1) {
    // 2220 LET M((I-1)*3+J)=O(16+I)*F(7)
            spells_remaining[(index - 1) * 3 + subindex] =
                O[16 + index] * attrs[7];
    // 2230 NEXT J:NEXT I
        }
    }
    // 2240 IF O(16)=1 THEN LET LT=20
    if (O[16] == 1) {
        *torches = 20;
    }
    // 2250 RETURN
    free(S$);
}

void lines2260_2490(screen_t *screen, int character_char_base,
                    char *character_name, double *attrs, int *finished,
                    int gold, int dungeon_level, int NX, int NY, int O[25],
                    int OS, int OT, int **R, int TR, int W) {
    int index, X, Y;
    char I$, * message;
    // 2260 LET M$="ONE MOMENT PLEASE":GOSUB430
    message = (char *) malloc(sizeof(char) * 18);
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, "ONE MOMENT PLEASE");
    lines430_430(screen, message, W);
    free(message);
    // 2270 LET S$="":LET T$=""
    char * S$ = (char *) malloc(
        sizeof(char) * (12 + OT + strlen(character_name))
    );
    if (S$ == NULL) {
        fprintf(stderr, "S$ is NULL!\n");
        exit(1);
    }
    char * T$ = (char *) malloc(sizeof(char) * 229);
    if (T$ == NULL) {
        fprintf(stderr, "T$ is NULL!\n");
        exit(1);
    }
    int s_index = 0;
    int t_index = 0;
    // 2280 FOR Y=1 TO 15
    for (Y = 1; Y <= 15; Y += 1) {
    // 2290 FOR X=1 TO 15
        for (X = 1; X <= 15; X += 1) {
    // 2300 LET T$=T$+CHR$(R(X,Y))
            T$[t_index] = (char) R[X][Y];
            t_index += 1;
    // 2310 NEXT X:NEXT Y
        }
    }
    // 2320 LET T$=T$+CHR$(OS+NX)
    T$[t_index] = (char) (OS + NX);
    t_index += 1;
    // 2330 LET T$=T$+CHR$(OS+NY)
    T$[t_index] = (char) (OS + NY);
    t_index += 1;
    // 2340 LET T$=T$+CHR$(OS+LE)
    T$[t_index] = (char) (OS + dungeon_level);
    t_index += 1;
    T$[t_index] = 0;
    // 2350 LET S$=S$+CHR$(AS+OT)
    S$[s_index] = (char) (character_char_base + OT);
    s_index += 1;
    // 2360 FOR I=1 TO 8
    for (index = 1; index <= 8; index += 1) {
    // 2370 LET S$=S$+CHR$(F(I)+AS)
        S$[s_index] = (char) (attrs[index] + character_char_base);
        s_index += 1;
    // 2380 NEXT I
    }
    // 2390 FOR I=1 TO OT
    for (index = 1; index <= OT; index += 1) {
    // 2400 LET S$=S$+CHR$(O(I)+AS)
        S$[s_index] = (char) (O[index] + character_char_base);
        s_index += 1;
    // 2410 NEXT I
    }
    // 2420 LET S$=S$+CHR$(GC+AS);
    S$[s_index] = (char) (gold + character_char_base);
    s_index += 1;
    // 2430 LET S$=S$+CHR$(TR+AS);
    S$[s_index] = (char) (TR + character_char_base);
    s_index += 1;
    // 2440 LET S$=S$+C$
    strcpy(S$ + s_index, character_name);
    s_index += strlen(character_name);
    S$[s_index] = 0;
    // 2450 LET M$="ANY KEY TO SAVE":GOSUB 370
    message = (char *) malloc(sizeof(char) * 16);
    if (message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(message, "ANY KEY TO SAVE");
    lines370_420(screen, &I$, message, W);
    free(message);
    // 2460 S=OPENOUT"HERO":PRINT#S,S$:CLOSE#S
    FILE *S = fopen("HERO", "w");
    int error = fputs(S$, S);
    if (error) {
        fprintf(stderr, "Error %i writing the character!", error);
    }
    error = fclose(S);
    if (error) {
        fprintf(stderr, "Error %i closing the character!", error);
    }
    free(S);
    // 2470 S=OPENOUT"LEVEL":PRINT#S,T$:CLOSE#S
    S = fopen("LEVEL", "w");
    error = fputs(T$, S);
    if (error) {
        fprintf(stderr, "Error %i writing the level!", error);
    }
    error = fclose(S);
    if (error) {
        fprintf(stderr, "Error %i closing the level!", error);
    }
    free(S);
    // 2480 LET FI=1
    *finished = 1;
    // 2490 RETURN
}

void lines2930_3200(int *character_char_base, int *char_code_blank,
                    int *char_code_wall, int *char_code_vase,
                    int *char_code_chest, int *char_code_idol,
                    int *char_code_way_in, int *char_code_exit,
                    int *char_code_safe_place, int OS);

void lines2500_2780(int *character_char_base, int *char_code_blank,
                    int *char_code_wall, int *char_code_vase,
                    int *char_code_chest, int *char_code_idol,
                    int *char_code_way_in, int *char_code_exit,
                    int *char_code_safe_place, int ***vertices,
                    int *distance_to_monster_x, double **attrs,
                    char **char_code_hero, int *finished, int *torches,
                    int **spells_remaining, int *MX, int *MY, int *NF, int *NX,
                    int *NY, int *OS, int ***R, int **T, const char ***T$,
                    int *TF, int *TX, int *TY, int *W, const char ***W$) {
    int index;
    // 2500 LET C$="ROLE PLAYING GAME":LET B$=""
    // C$ is overwritten before being accessed again.
    // dungeon_lib removes the need for B$
    // 2510 LET W=40:LET OS=96
    *W = 40;
    *OS = 96;
    // 2520 FOR I=1 TO W:LET B$=B$+" ":NEXT I
    // dungeon_lib removes the need for B$
    // 2530 DIM R(15,15),F(8),O(24)
    *R = (int **) malloc(sizeof(int *) * 16);
    if (*R == NULL) {
        fprintf(stderr, "*R is NULL!\n");
        exit(1);
    }
    int i;
    for (i = 0; i < 16; i += 1) {
        (*R)[i] = (int *) malloc(sizeof(int) * 16);
        if ((*R)[i] == NULL) {
            fprintf(stderr, "(*R)[%i] is NULL!\n", i);
            exit(1);
        }
    }
    *attrs = (double *) malloc(sizeof(double) * 9);
    if (attrs == NULL) {
        fprintf(stderr, "*attrs is NULL!\n");
        exit(1);
    }
    // 2540 DIM W$(11),T$(12)
    *W$ = (const char **) malloc(sizeof(const char *) * (12));
    if (*W$ == NULL) {
        fprintf(stderr, "*W$ is NULL!\n");
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
    *T = (int *) malloc(sizeof(int) * 19);
    if (*T == NULL) {
        fprintf(stderr, "*T is NULL!\n");
        exit(1);
    }
    // 2560 DATA"GR SWORD","SWORD","AXE","MACE","FLAIL","DAGGER","ARMOUR","ARMOUR"
    // 2570 DATA"ARMOUR","HELMET","HEADPC."
    // 2580 FOR I = 1 TO 11
    // 2590 READ W$(I)
    // 2600 NEXT I
    (*W$)[1] = "GR SWORD";
    (*W$)[2] = "SWORD";
    (*W$)[3] = "AXE";
    (*W$)[4] = "MACE";
    (*W$)[5] = "FLAIL";
    (*W$)[6] = "DAGGER";
    (*W$)[7] = "ARMOUR";
    (*W$)[8] = "ARMOUR";
    (*W$)[9] = "ARMOUR";
    (*W$)[10] = "HELMET";
    (*W$)[11] = "HEADPC.";

    // 2610 DATA"A GOOD BLOW","WELL HIT SIRE","THY AIM IS TRUE","MISSED!","HIT THEE!!"
    // 2620 DATA"THE MONSTER IS SLAIN","NO LIGHT","BROKEN THY ","SPELL EXHAUSTED"
    // 2630 DATA"PRESS ANY KEY","YOU NEED EXPERIENCE","EXIT FROM THIS LEVEL"
    // 2640 FOR I = 1 TO 12
    // 2650 READ T$(I)
    // 2660 NEXT I

    *T$ = (const char **) malloc(sizeof(const char *) * (13));
    if (*T$ == NULL) {
        fprintf(stderr, "*T$ is NULL!\n");
        exit(1);
    }

    (*T$)[1] = "A GOOD BLOW";
    (*T$)[2] = "WELL HIT SIRE";
    (*T$)[3] = "THY AIM IS TRUE";
    (*T$)[4] = "MISSED!";
    (*T$)[5] = "HIT THEE!!";
    (*T$)[6] = "THE MONSTER IS SLAIN";
    (*T$)[7] = "NO LIGHT";
    (*T$)[8] = "BROKEN THY ";
    (*T$)[9] = "SPELL EXHAUSTED";
    (*T$)[10] = "PRESS ANY KEY";
    (*T$)[11] = "YOU NEED EXPERIENCE";
    (*T$)[12] = "EXIT FROM THIS LEVEL";


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
    *NF = 0;
    // 2700 LET TX=0:LET TY=0:LET TF=0:LET TR=0
    *TX = 0;
    *TY = 0;
    *TF = 0;
    // The value of TR set here is not used. It is overwritten when the
    // character is loaded.
    // 2710 LET MX=0:LET MY=0:LET DY=12:LET F$=""
    *MX = 0;
    *MY = 0;
    // The value of DY set here is never used.
    *char_code_hero = (char *) malloc(sizeof(char) * 7);
    if (*char_code_hero == NULL) {
        fprintf(stderr, "char_code_hero is NULL!\n");
        exit(1);
    }
    // 2720 LET NX=1:LET NY=1:LET RE=0:LET LT=0
    *NX = 1;
    *NY = 1;
    // RE is never actually used.
    *torches = 0;
    // 2730 FOR I = 1 TO 5
    for (index = 1; index <= 5; index += 1) {
    // 2740 LET F$=F$+CHR$(OS+I)
        (*char_code_hero)[index] = *OS + index;
    // 2750 NEXT I
    }
    (*char_code_hero)[6] = 0;
    // 2760 DATA69,117,73,121,81,129,69,117,73,121,81,129,89,137,97,145,101,149
    // 2770 FOR I=1 TO 18:READ T(I):NEXT I:GOSUB 2930
    (*T)[1] = 69;
    (*T)[2] = 117;
    (*T)[3] = 73;
    (*T)[4] = 121;
    (*T)[5] = 81;
    (*T)[6] = 129;
    (*T)[7] = 69;
    (*T)[8] = 117;
    (*T)[9] = 73;
    (*T)[10] = 121;
    (*T)[11] = 81;
    (*T)[12] = 129;
    (*T)[13] = 89;
    (*T)[14] = 137;
    (*T)[15] = 97;
    (*T)[16] = 145;
    (*T)[17] = 101;
    (*T)[18] = 149;
    lines2930_3200(
        character_char_base, char_code_blank, char_code_wall, char_code_vase,
        char_code_chest, char_code_idol, char_code_way_in, char_code_exit,
        char_code_safe_place, *OS
    );
    // 2780 RETURN
}

void lines2790_2920(screen_t *screen, char *character_name, int W) {
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
    print_left$_b$(screen, W - strlen(character_name));
    // 2820 paper 2:ink 3
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, WHITE);
    // 2830 FOR I=1 TO 5:PRINT LEFT$(B$,W);:NEXT I
    for (index = 1; index <= 5; index += 1) {
        print_left$_b$(screen, W);
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
                    int *char_code_way_in, int *char_code_exit,
                    int *char_code_safe_place, int OS) {
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
    *char_code_blank = OS + 6;
    // 3180 LET C1=C0+1:LET C2=C0+2:LET C3=C0+3:LET C4=C0+4
    *char_code_wall = *char_code_blank + 1;
    *char_code_vase = *char_code_blank + 2;
    *char_code_chest = *char_code_blank + 3;
    *char_code_idol = *char_code_blank + 4;
    // 3190 LET C5=C0+6:LET C6=C0+7:LET C7=C0+8:LET C8=C0+12
    *char_code_way_in = *char_code_blank + 6;
    *char_code_exit = *char_code_blank + 7;
    *char_code_safe_place = *char_code_blank + 8;
    // C8 is never acyually used.

    // 3200 RETURN
}
