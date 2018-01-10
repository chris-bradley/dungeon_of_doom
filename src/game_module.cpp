#include <SDL.h>
#include <unistd.h>
#include "dungeon_lib.h"

void lines430_430(screen_t *screen);
void lines480_560(screen_t *screen);
void lines570_610(screen_t *screen);
void lines620_770(screen_t *screen);
void lines810_860(screen_t *screen);
void lines870_930(screen_t *screen);
void lines990_1130(screen_t *screen);
void lines1410_1520(screen_t *screen, int C1);
void lines1550_1650(screen_t *screen);
void lines1660_1680();
void lines1690_1750(screen_t *screen);
void lines1760_1950(screen_t *screen);
void lines1770_1950(screen_t *screen);
void lines2010_2250(screen_t *screen);
void lines2260_2490(screen_t *screen);
void lines2500_2780(int *C1, int *C5, int *C6);

int DX,
    NF,  // Facing. NESW
    NX,
    NY,
    OX,
    OY,
    RH,  // Object at NX / NY
    TF,  // Flag to see if we can exit.
    TX,
    TY,
    X,
    Y;
double FI, S1;
char * C$;
char * F$;
char I$;
char * M$;
const char ** T$;
const char ** W$;
double * F;

int ** D;
int * M;
int O[25];
int ** R;
int * T;

int main(int argc, char *argv[]) {
    int C1,  // Symbol for Wall
        C5,  // Symbol for Way In
        C6;  // Symbol for Exit
    // C64: 5 GOSUB 5000:POKE 53281,0
    screen_t *screen = NULL;

    if (init_screen(&screen) < 0) {
        return 1;
    }
    paper(screen->cursor, 2);
    ink(screen->cursor, 0);
    clear_screen(screen);
    // lines 5000 on unneeded due to dungeon lib
    // 10 GOSUB2500
    lines2500_2780(&C1, &C5, &C6);
    // 20 GOSUB2010
    lines2010_2250(screen);
    // 30 GOSUB1770
    lines1770_1950(screen);
    int game_over = 0;
    do {
        SDL_RenderPresent(screen->ren);
    // 40 LET I$=inkey$
        I$ = inkey$();
    // 50 IF I$="A" AND DX<255 THEN GOSUB870
        if (I$ == 'a' && DX < 255 ) {
            lines870_930(screen);
        }
    // 60 IF I$="C" AND F(7)>0 AND O(17)+O(18)>0 THEN GOSUB990
        if (I$ == 'c' && F[7] > 0 && O[17] + O[18] > 0) {
            lines990_1130(screen);
        }
    // 70 IF I$="G" THEN GOSUB1410
        if (I$ == 'g') {
            lines1410_1520(screen, C1);
        }
    // 80 IF I$="P" THEN GOSUB1660
        if (I$ == 'p') {
            lines1660_1680();
        }
    // 90 IF I$="R" THEN GOSUB1690
        if (I$ == 'r') {
            lines1690_1750(screen);
        }
    // 100 IF I$="S" THEN GOSUB2260
        if (I$ == 's') {
            lines2260_2490(screen);
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
            NX += D[NF][1];
            NY += D[NF][2];
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
        if (RH == C1) {
            X = NX;
            Y = NY;
            lines570_610(screen);
            NX = OX;
            NY = OY;
            F[1] -= 0.03;
        }
    // 220 IF RH=C6 THEN LET TX=NX:LET TY=NY:LET TF=1
        if (RH == C6) {
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
        if (F[1] > S1 * 0.8 && rand() % 8 < F[6]) {
            TF = 0;
        }
    // 250 IF I$>"" THEN LET F(1)=F(1)*0.99
        if (I$ != 0) {
            F[1] = F[1] * 0.99;
        }
    // 260 IF F(1) <S1 THEN LET F(1)=F(1)+(F(2)/1100)
        if (F[1] < S1) {
            F[1] += F[2] / 1100;
        }
    // 270 GOSUB480
        lines480_560(screen);
    // 280 IF OX<>NX OR OY<>NY THEN LET X=OX:LET Y=OY:GOSUB570
        if (OX != NX || OY != NY) {
            X = OX;
            Y = OY;
            lines570_610(screen);
        }
    // 290 LET OX=NX:LET OY=NY
        OX = NX;
        OY = NY;
    // 300 IF DX<255 THEN GOSUB620
        if (DX < 255) {
            lines620_770(screen);
        }
    // 310 IF F(1)>0 AND FI<1 AND RH<>C5 THEN GOTO 40
        if (F[1] > 0 && FI < 1 && RH != C5) {
            game_over = 0;
        }
    // 320 IF RH=C5 THEN LET M$=T$(12):GOSUB430:GOSUB1760:GOTO40
        else if (RH == C5) {
            free(M$);
            M$ = (char *) malloc(sizeof(char) * (strlen(T$[12]) + 1));
            if (M$ == NULL) {
                fprintf(stderr, "M$ is NULL!\n");
                exit(1);
            }
            strcpy(M$, T$[12]);
            M$[strlen(T$[12])] = 0;
            lines430_430(screen);
            lines1760_1950(screen);
            game_over = 0;
        } else {
            game_over = 1;
        }
    } while (!game_over);
    // 330 IF F(1)<1 THEN GOSUB810
    if (F[1] < 1) {
        lines810_860(screen);
    }
    // 340 PRINT tab(0,10);:STOP
    tab(screen->cursor, 0, 10);
    destroy_screen(screen);

    int i;
    free(C$);
    for (i = 0; i < 3; i += 1) {
        free(D[i]);
    }
    free(D);
    free(F);
    free(F$);
    free(M);
    for (i = 0; i < 16; i += 1) {
        free(R[i]);
    }
    free(R);
    free(T);
    free(M$);
    free(T$);
    free(W$);
    return 0;
}

void lines350_355() {
    // C64:
    // 350 POKE VS+1,J:POKE VS+4,33
    // 355 POKE VS+4,32:RETURN

    // TODO: SOUND!
}

void lines360_365() {
    // C64:
    // 360 POKE VS+8,J:POKE VS+11,129
    // 365 POKE VS+11,128:RETURN

    // TODO: SOUND!
}

int W;

void lines370_420(screen_t *screen) {
    // 370 paper 2:ink 0
    paper(screen->cursor, 2);
    ink(screen->cursor, 0);
    // 380 PRINT tab(0,5):M$;
    tab(screen->cursor, 0, 5);
    print_text(screen, M$);
    // 390 LET I$=inkey$
    // 400 IF I$="" THEN GOTO390
    SDL_RenderPresent(screen->ren);
    I$ = inkey$();
    // 410 PRINT tab(0,5);LEFT(B$, W);:LET M$=""
    tab(screen->cursor, 0, 5);
    print_left$_b$(screen, W);
    free(M$);
    M$ = (char *) malloc(sizeof(char) * 2);
    if (M$ == NULL) {
        fprintf(stderr, "M$ is NULL!\n");
        exit(1);
    }
    strcpy(M$, "");
    // 420 RETURN
}

void lines440_470(screen_t *screen);

int S2;
void lines430_430(screen_t *screen) {
    // 430 paper 2:ink 0
    paper(screen->cursor, 2);
    ink(screen->cursor, 0);
    lines440_470(screen);
}

void lines440_470(screen_t *screen) {
    // 440 PRINT tab(0,5);M$;
    tab(screen->cursor, 0, 5);
    print_text(screen, M$);
    SDL_RenderPresent(screen->ren);
    // 450 FOR D=1 TO 600:NEXT D
    // C64: 450 FOR D=1 TO 200:NEXT D
    sleep(0.34);
    // 460 PRINT tab(0,5);LEFT$(B$,W);:LET M$=""
    tab(screen->cursor, 0, 5);
    print_left$_b$(screen, W);
    free(M$);
    M$ = (char *) malloc(sizeof(char) * 2);
    if (M$ == NULL) {
        fprintf(stderr, "M$ is NULL!\n");
        exit(1);
    }
    strcpy(M$, "");
    // 470 RETURN
}

void lines480_560(screen_t *screen) {
    // 480 paper 1:ink 3
    paper(screen->cursor, 1);
    ink(screen->cursor, 3);
    // 490 PRINT tab(NX,NY+5);MID$(F$,NF,1);
    tab(screen->cursor, NX, NY + 5);
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    outstring[0] = F$[NF];
    outstring[1] = 0;
    print_text(screen, outstring);
    // 500 paper 2:ink 0
    paper(screen->cursor, 2);
    ink(screen->cursor, 0);
    // 510 PRINT tab(16,8);INT(F(1));" ";
    tab(screen->cursor, 16, 8);
    sprintf(outstring, "%i ", (int) F[1]);
    print_text(screen, outstring);

    // 520 PRINT tab(16,11);INT(F(2));" ";
    tab(screen->cursor, 16, 11);
    sprintf(outstring, "%i ", (int) F[2]);
    print_text(screen, outstring);
    // 530 PRINT tab(16,14);INT (F(7));" ";
    tab(screen->cursor, 16, 14);
    sprintf(outstring, "%i ", (int) F[7]);
    print_text(screen, outstring);
    // 540 PRINT tab(16,17);MID$("NESW.",NF,1);
    tab(screen->cursor, 16, 17);
    outstring[0] = "NESW."[NF - 1];
    outstring[1] = 0;
    print_text(screen, outstring);
    // 550 PRINT tab(16,20);INT(F(5));
    tab(screen->cursor, 16, 20);
    sprintf(outstring, "%i ", (int) F[5]);
    print_text(screen, outstring);
    // 560 RETURN
    free(outstring);
    SDL_RenderPresent(screen->ren);
}

int C2, C7, LX, LY, M_, MS, MT, MV, RM;

void lines570_610(screen_t *screen) {
    // 570 paper 1:ink 2
    paper(screen->cursor, 1);
    ink(screen->cursor, 2);
    // 580 LET RM=R(X,Y):PRINT tab(X,Y+5);CHR$(RM);
    RM = R[X][Y];
    tab(screen->cursor, X, Y + 5);
    char * outstring = (char *) malloc(sizeof(char) * 2);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    sprintf(outstring, "%c", (char) RM);
    print_text(screen, outstring);
    free(outstring);
    // 590 IF ABS(DX)<4 OR RM<=C7 THEN RETURN
    if (abs(DX) < 4 || RM <= C7) {
        return;
    }
    // 600 LET MT=RM:LET M=MT-C2:LET MV=M/16:LET MS=M*6:LET DX=3:LET LX=X:LET LY=Y
    MT = RM;
    M_ = MT - C2;
    MV = M_ / 16;
    MS = M_ * 6;
    DX = 3;
    LX = X;
    LY = Y;
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

void lines780_800();

int C0, DY, H, I, J, MB, MX, MY, SX, SY, WB;

void lines620_770(screen_t *screen) {
    // 620 LET DX=LX-NX:LET SX=SGN(DX):LET DY=LY-NY:LET SY=SGN(DY)
    DX = LX - NX;
    SX = sign(DX);
    DY = LY - NY;
    SY = sign(DY);
    // 630 LET MX=LX-(MV*SX):LET MY=LY-(MV*SY):LET RM=R(MX,MY)
    MX = LX - (MV * SX);
    MY = LY - (MV * SY);
    RM = R[MX][MY];
    // 640 IF RM>C0 AND RM<>MT THEN LET MY=LY-(MV*SY):LET RM=R(MX,MY)
    if (RM > C0 && RM != MT) {
        MY = LY - (MV * SY);
        RM = R[MX][MY];
    }
    // 650 LET R(LX,LY)=C0:LET X=LX:LET Y=LY:GOSUB 570
    R[LX][LY] = C0;
    X = LX;
    Y = LY;
    lines570_610(screen);
    // 660 LET R(MX,MY)=MT:LET X=MX:LET Y=MY:GOSUB 570
    R[MX][MY] = MT;
    X = MX;
    Y = MY;
    lines570_610(screen);
    // 670 LET LX=MX:LET LY=MY:LET H=0
    LX = MX;
    LY = MY;
    H = 0;
    // 680 IF ABS(DX)<=1 AND ABS(DY)<=1 AND RH<>C7 THEN LET H=M*.5:LET J=H:GOSUB350
    if (abs(DX) <= 1 && abs(DY) <= 1 && RH != C7) {
        H = M_ * 0.5;
        J = H;
        lines350_355();
    }
    // 690 IF H*12<F(6)+F(3) THEN RETURN
    if (H * 12 < F[7] + F[3]) {
        return;
    }
    // 700 LET M$=T$(5):GOSUB430:GOSUB360
    free(M$);
    M$ = (char *) malloc(sizeof(char) * (strlen(T$[5]) + 1));
    if (M$ == NULL) {
        fprintf(stderr, "M$ is NULL!\n");
        exit(1);
    }
    strcpy(M$, T$[5]);
    lines430_430(screen);
    lines360_365();
    // 710 LET H=H/(3+O(9) + O(10) + O(11) + O(12) + O(13) + O(14))
    H /= (3 + O[9] + O[10] + O[11] + O[12] + O[13] + O[14]);
    // 720 LET F(1)=F(1)-H:LET F(2)=F(2)-(H/101)
    F[1] -= H;
    F[2] -= H / 101;
    // 730 LET I=1:LET WB=0:LET MB=rnd(M)
    I = 1;
    WB = 0;
    MB = rand() % M_;
    // 740 LET J=MT:GOSUB350:GOSUB360
    J = MT;
    lines350_355();
    lines360_365();
    // 750 IF MB=1 AND O(I)>0 THEN GOSUB780
    int done = 0;
    do {
        if (MB == 1 && O[I] > 0) {
            lines780_800();
        }
        if (I < 11) {
            I += 1;
            done = 1;
        }
    } while (!done);
    // 760 IF I<11 THEN LET I=I+1:GOTO750
    // 770 RETURN
}

void lines780_800() {
    // 780 LET O(I)=0:LET M$=T$(8)+" "+W$(I):GOSUB430
    O[I] = 0;
    free(M$);
    M$ = (char *) malloc(sizeof(char) * (strlen(T$[8]) + strlen(W$[I]) + 2));
    if (M$ == NULL) {
        fprintf(stderr, "M$ is NULL!\n");
        exit(1);
    }
    sprintf(M$, "%s %s", T$[8], W$[I]);
    // 790 LET MB=0:GOSUB360:LET J=I:GOSUB350
    MB = 0;
    lines360_365();
    J = I;
    lines350_355();
    // 800 RETURN
}

void lines810_860(screen_t *screen) {
    // 810 LET NF=5;LET F(1)=0:GOSUB 440
    NF = 5;
    F[1] = 0;
    lines440_470(screen);
    // 820 PRINT tab(1,5);"THOU HAST EXPIRED!"
    tab(screen->cursor, 1, 5);
    print_text(screen, "THOU HAST EXPIRED!");
    // 830 FOR J=150 TO 1 STEP-4
    for (J = 150; J >= 1; J -= 4) {
    // 840 GOSUB350:GOSUB360:GOSUB570:GOSUB480
        lines350_355();
        lines360_365();
        lines570_610(screen);
        lines480_560(screen);
    // 850 NEXT J
    }
    // 860 RETURN
}

void lines940_980(screen_t *screen);

int HT;

void lines870_930(screen_t *screen) {
    // 870 LET M$=T$(rnd(3)):GOSUB360
    free(M$);
    int t$_ind = rand() % 3 + 1;
    M$ = (char *) malloc(sizeof(char) * (strlen(T$[t$_ind]) + 1));
    if (M$ == NULL) {
        fprintf(stderr, "M$ is NULL!\n");
        exit(1);
    }
    strcpy(M$, T$[t$_ind]);
    lines360_365();
    // 880 LET H=F(1)+O(1) + O(2) + O(3) + O(4) + O(5) + O(6) + O(7) + O(8) + rnd(F(6))
    H =
        F[1] + O[1] + O[2] + O[3] + O[4] + O[5] + O[6] + O[7] + O[8] + \
        (rand() * F[6] / RAND_MAX);
    // 890 IF F(3)+F(6)< rnd(M)+2 THEN LET M$=T$(4):LET HT=0
    if (F[3] + F[6] < rand() % M_ + 2) {
        free(M$);
        M$ = (char *) malloc(sizeof(char) * (strlen(T$[4]) + 1));
        if (M$ == NULL) {
            fprintf(stderr, "M$ is NULL!\n");
            exit(1);
        }
        strcpy(M$, T$[4]);
        HT = 0;
    }
    // 900 LET MS=MS-H:GOSUB430
    MS -= H;
    lines430_430(screen);
    // 910 LET F(1)=F(1)-(H/100):LET F(5)=F(5)+0.05
    F[1] -= H / 100;
    F[5] += 0.05;
    // 920 IF MS<1 THEN GOSUB940
    if (MS < 1) {
        lines940_980(screen);
    }
    // 930 RETURN
}

void lines940_980(screen_t *screen) {
    // 940 LET DX=255:LET MS=0:LET R(MX,MY)=C0
    DX = 255;
    MS = 0;
    R[MX][MY] = C0;
    // 950 LET F(5)=F(5)+.1
    F[5] += 0.1;
    // 960 LET M$=T$(6):GOSUB430
    free(M$);
    M$ = (char *) malloc(sizeof(char) * (strlen(T$[6]) + 1));
    if (M$ == NULL) {
        fprintf(stderr, "M$ is NULL!\n");
        exit(1);
    }
    strcpy(M$, T$[6]);
    lines430_430(screen);
    // 970 FOR J=200 TO 150STEP-8:GOSUB350:GOSUB360:NEXT J
    for (J = 200; J >= 150; J -= 8) {
        lines350_355();
        lines360_365();
    }
    // 980 GOSUB570:RETURN
    lines570_610(screen);
}

void lines1140_1180(screen_t *screen);
void lines1190_1210();
void lines1220_1270(screen_t *screen);
void lines1280_1290();
void lines1300_1380(screen_t *screen);
void lines1390_1400();

int SL;

void lines990_1130(screen_t *screen) {
    // 990 GOSUB480:paper 2: ink 0
    lines480_560(screen);
    paper(screen->cursor, 2);
    ink(screen->cursor, 0);
    // 1000 PRINT tab(0,10);"YOU MAY USE MAGICKS";
    tab(screen->cursor, 0, 1);
    print_text(screen, "YOU MAY USE MAGICKS");
    // 1010 IF O(17)>0 THEN PRINT tab(0,2);"FROM NECRONOMICON";
    if (O[17] > 0) {
        tab(screen->cursor, 0, 2);
        print_text(screen, "FROM NECRONOMICON");
    }
    // 1020 IF O(18)>0 THEN PRINT tab(0,3);"FROM THE SCROLLS";
    if (O[18] > 0) {
        tab(screen->cursor, 0, 3);
        print_text(screen, "FROM THE SCROLLS");
    }
    // 1030 PRINT tab(0,4);"CONSULT THE LORE"
    tab(screen->cursor, 0, 4);
    print_text(screen, "CONSULT THE LORE");
    // 1040 LET M$="USE SPELL NUMBER?":GOSUB370
    do {
        free(M$);
        M$ = (char *) malloc(sizeof(char) * 18);
        if (M$ == NULL) {
            fprintf(stderr, "M$ is NULL!\n");
            exit(1);
        }
        strcpy(M$, "USE SPELL NUMBER?");
        lines370_420(screen);
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
    M[SL] -= 1;
    X = NX;
    Y = NY;
    // 1080 IF M(SL)<0 THEN LET M$=T$(9):LET SL=7
    if (M[SL] < 0) {
        free(M$);
        M$ = (char *) malloc(sizeof(char) * (strlen(T$[9]) + 1));
        if (M$ == NULL) {
            fprintf(stderr, "M$ is NULL!\n");
            exit(1);
        }
        strcpy(M$, T$[9]);
        SL = 7;
    }
    // 1090 FOR J=1 TO 5:PRINT tab(0,J);LEFT$(B$, W);:NEXT J:GOSUB570
    for (J = 1; J <= 5; J += 1) {
        tab(screen->cursor, 0, J);
        print_left$_b$(screen, W);
        newline(screen->cursor);
    }
    lines570_610(screen);
    // 1100 ON SL GOSUB1140,1190,1220,1280,1300,1390,1130
    switch (SL) {
        case 1:
            lines1140_1180(screen);
            break;
        case 2:
            lines1190_1210();
            break;
        case 3:
            lines1220_1270(screen);
            break;
        case 4:
            lines1280_1290();
            break;
        case 5:
            lines1300_1380(screen);
            break;
        case 6:
            lines1390_1400();
            break;
        case 7:
            // Line 1130 is just a return statement.
            break;
    }
    // 1110 LET F(5)=F(5)+.2
    F[5] += 0.2;
    // 1120 GOSUB430
    lines430_430(screen);
    // 1130 RETURN
}

void lines1140_1180(screen_t *screen) {
    // 1140 FOR J=1 TO 12
    for (J = 1; J <= 12; J += 1) {
    // 1150 GOSUB350:GOSUB360
        lines350_355();
        lines360_365();
    // 1160 NEXT J
    }
    // 1170 IF DX<255 THEN LET X=MX:LET Y=MY:GOSUB940
    if (DX < 255) {
        X = MX;
        Y = MY;
        lines940_980(screen);
    }
    // 1180 RETURN
}

void lines1190_1210() {
    // 1190 IF RH=C0 THEN LET R(NX,NY)=C7
    if (RH == C0) {
        R[NX][NY] = C7;
    }
    // 1200 LET J=100:GOSUB350:LET J=200:GOSUB350
    J = 100;
    lines350_355();
    J = 200;
    lines350_355();
    // 1210 RETURN
}

void lines1220_1270(screen_t *screen) {
    // 1220 LET NX=rnd(13):LET NY=rnd(13)
    NX = rand() % 13;
    NY = rand() % 13;
    // 1230 FOR J=0 TO 255 STEP8
    for (J = 0; J <= 255; J += 8) {
    // 1240 GOSUB360:GOSUB350
        lines360_365();
        lines350_355();
    // 1250 NEXT J
    }
    // 1260 GOSUB480
    lines480_560(screen);
    // 1270 RETURN
}

void lines1280_1290() {
    // 1280 LET F(2)=F(2)+rnd(M(SL)):LET F(1)=F(1)+rnd(M(SL)):LET F(7)=F(7)-1
    F[2] += rand() * M[SL];
    F[1] += rand() * M[SL];
    F[7] -= 1;
    // 1290 RETURN
}

void lines1300_1380(screen_t *screen) {
    // 1300 FOR J=1 TO 30
    for (J = 1; J <= 30; J += 1) {
    // 1310 LET R(NX,NY)=rnd(8)+1+C0
        R[NX][NY] = rand() % 8 + 1 + C0;
    // 1320 GOSUB350:GOSUB570
        lines350_355();
        lines570_610(screen);
    // 1330 NEXT J
    }
    // 1340 IF RH<=C7 THEN LET DX=255:LET MS=0
    if (RH <= C7) {
        DX = 255;
        MS = 0;
    }
    // 1350 FOR J = 1 TO 2O STEP4
    for (J = 1; J <= 20; J += 4) {
    // 1360 GOSUB 350
        lines350_355();
    // 1370 NEXT J
    }
    // 1380 RETURN
}

void lines1390_1400() {
    // 1390 LET F(2)=S2:LET F(1)=S1:LET F(7)=F(7)-1
    F[2] = S2;
    F[1] = S1;
    F[7] -= 1;
    // 1400 RETURN
}

int C3, C4, GT, GX, GY, TR;

void lines1410_1520(screen_t *screen, int C1) {
    // 1410 LET GX=NX+D(NF,1):LET GY=NY+D(NF,2)
    GX = NX + D[NF][1];
    GY = NY + D[NF][2];
    // 1420 IF GX<0 THEN LET GX=0
    if (GX < 0) {
        GX = 0;
    }
    // 1430 IF GY<0 THEN LET GY=0
    if (GY < 0) {
        GY = 0;
    }
    // 1440 IF GX>15 THEN LET GX=15
    if (GX > 15) {
        GX = 15;
    }
    // 1450 IF GY>15 THEN LET GY=15
    if (GY > 15) {
        GY = 15;
    }
    // 1460 LET GT=R(GX,GY):IF GT>C1 AND GT<C4 THEN LET R(GX,GY)=C0
    GT = R[GX][GY];
    if (GT > C1 && GT < C4) {
        R[GX][GY] = C0;
    }
    // 1470 IF GT=C2 THEN LET O(23)=O(23)+1:LET O(24)=O(24)+1
    if (GT == C2) {
        O[23] += 1;
        O[24] += 1;
    }
    // 1480 IF GT=C3 THEN LET TR=TR+1
    if (GT == C3) {
        TR += 1;
    }
    // 1490 IF GT=C4 THEN GOSUB 1550
    if (GT == C4) {
        lines1550_1650(screen);
    }
    // 1500 LET X=GX:LET Y=GY:GOSUB570
    X = GX;
    Y = GY;
    lines570_610(screen);
    // 1510 IF GT>C1 AND GT<C4 THEN LET J=GT:GOSUB350:LET J=GT+5:GOSUB350
    if (GT > C1 && GT < C4) {
        J = GT;
        lines350_355();
        J = GT + 5;
        lines350_355();
    }
    // 1520 RETURN
}

int GC, N;

void lines1550_1650(screen_t *screen) {
    // 1550 paper 2:ink 1
    paper(screen->cursor, 2);
    ink(screen->cursor, 1);
    // 1560 PRINT tab(0,10);" THY QUEST IS OVER! "
    tab(screen->cursor, 0, 10);
    print_text(screen, " THY QUEST IS OVER! ");
    // 1570 FOR I = 1 TO 18
    for (I = 1; I <= 18; I += 1) {
    // 1580 LET J=T(I):GOSUB350
        J = T[I];
        lines350_355();

    // 1590 LET X=NX:LET Y=NY
        X = NX;
        Y = NY;
    // 1600 FOR N=1 TO 4:LET NF=N:GOSUB480:NEXT N
        for (N = 1; N <=4; N += 1) {
            NF = N;
            lines480_560(screen);
        }
    // 1610 NEXT I
    }
    // 1620 LET MS=0
    MS = 0;
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
        (int) ((TR * 10) + (GC * F[5]) + F[1] + F[2] + F[3])
    );
    print_text(screen, outstring);
    free(outstring);
    // 1640 LET FI=1
    FI = 1;
    // 1650 RETURN
}

void lines1660_1680() {
    // 1660 IF O(24)>0 AND F(1)<S1 THEN LET F(1)=S1:LET O(24)=O(24)-1
    if (O[24] > 0 && F[1] < S1) {
        F[1] = S1;
        O[24] -= 1;
    }
    // 1670 IF O(23)>0 AND F(2)<S2 THEN LET F(2)=S2:LET O(23)=O(23)-1
    if (O[23] > 0 && F[2] < S2) {
        F[2] = S2;
        O[23] -= 1;
    }
    // 1680 RETURN
}

int LT;

void lines1690_1750(screen_t *screen) {
    // 1690 IF LT=0 THEN LET M$=T$(7):GOSUB430:RETURN
    if (LT == 0) {
        free(M$);
        M$ = (char *) malloc(sizeof(char) * (strlen(T$[7]) + 1));
        if (M$ == NULL) {
            fprintf(stderr, "M$ is NULL!\n");
            exit(1);
        }
        strcpy(M$, T$[7]);
        lines430_430(screen);
        return;
    }
    // 1700 FOR Y=NY-3 TO NY+3
    for (Y = NY - 3; Y <= NY + 3; Y += 1) {
    // 1710 FOR X=NX-3 TO NX+3
        for (X = NX - 3; X <= NX + 3; X += 1) {
    // 1720 IF (X>0 AND X<16) AND (Y>0 AND Y<16)THEN GOSUB570
            if (X > 0 && X < 16 && Y > 0 && Y < 16) {
                lines570_610(screen);
            }
    // 1730 NEXT X:NEXT Y
        }
    }
    // 1740 LET LT=LT-1
    LT -= 1;
    // 1750 RETURN
}

void lines370_420(screen_t *screen);
void lines1960_2000(screen_t *screen);
void lines2790_2920(screen_t *screen);

int IX, IY, LE, OS, S3;

void lines1760_1770_1950(screen_t *screen,
                         int start_at_1770) {
    // The original BASIC code sometimes used 'GOSUB 1760' and sometimes
    // 'GOSUB 1770'. This is further complicated by their use of a
    // 'GOTO 1760' towards the end.
    // We use the 'start_at_1770' flag to handle this.
    int correct_level_loaded;
    do {

    // 1760 IF F(5)<S3+1 THEN LET M$=T$(11):LET NX=OX:LET NY=OY:GOSUB 430:RETURN
        if (!start_at_1770 && F[5] < S3 + 1) {
            free(M$);
            M$ = (char *) malloc(sizeof(char) * (strlen(T$[11]) + 1));
            if (M$ == NULL) {
                fprintf(stderr, "M$ is NULL!\n");
                exit(1);
            }
            strcpy(M$, T$[11]);
            NX = OX;
            NY = OY;
            lines430_430(screen);
            return;
        }
        start_at_1770 = 0;
    // 1770 CLS:PRINT tab(0,3);"PREPARE DUNGEON TAPE"
        clear_screen(screen);
        tab(screen->cursor, 0, 3);
        print_text(screen, "PREPARE DUNGEON TAPE");
    // 1780 LET M$=T$(10):GOSUB370
        free(M$);
        M$ = (char *) malloc(sizeof(char) * (strlen(T$[10]) + 1));
        if (M$ == NULL) {
            fprintf(stderr, "M$ is NULL!\n");
            exit(1);
        }
        strcpy(M$, T$[10]);
        lines370_420(screen);
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
        I = 1;
    // 1830 FOR Y=1 TO 15
        for (Y = 1; Y <= 15; Y += 1) {
    // 1840 FOR X=1 TO 15
            for (X = 1; X <= 15; X += 1) {
    // 1850 LET R(X,Y)=ASC(MID$(S$,I,1))
                R[X][Y] = (int) S$[I - 1];
    // 1860 LET I=I+1
                I += 1;
    // 1870 NEXT X
            }
    // 1880 NEXT Y
        }
    // 1890 LET IX=ASC(MID$(S$,I,1))-OS
        IX = (int) S$[I - 1] - OS;
    // 1900 LET IY=ASC(MID$(S$,I+1,1))-OS
        IY = (int) S$[I] - OS;
    // 1910 LET LE=ASC(MID$(S$,I+2,1))-OS
        LE = (int) S$[I + 1] - OS;
    // 1920 IF LE>F(5) THEN GOSUB 1960:GOTO 1760
        if (LE > F[5]) {
            lines1960_2000(screen);
            correct_level_loaded = 1;
        } else {
            correct_level_loaded = 0;
        }
    } while (correct_level_loaded);
    // 1930 GOSUB2790
    lines2790_2920(screen);
    // 1940 LET NX=IX:LET NY=IY:LET OX=NX:LET OY=NY:LET DX=255
    NX = IX;
    NY = IY;
    OX = NX;
    OY = NY;
    DX = 255;
    // 1950 RETURN
}

void lines1760_1950(screen_t *screen) {
    lines1760_1770_1950(screen, 0);
}

void lines1770_1950(screen_t *screen) {
    lines1760_1770_1950(screen, 1);
}

void lines1960_2000(screen_t *screen) {
    // 1960 PRINT:PRINT"LEVEL TOO DEEP"
    newline(screen->cursor);
    print_text(screen, "LEVEL TOO DEEP");
    newline(screen->cursor);
    // 1970 PRINT"REWIND TAPE"
    print_text(screen, "REWIND TAPE");
    newline(screen->cursor);
    // 1980 PRINT"TO POSITION"
    print_text(screen, "TO POSITION");
    newline(screen->cursor);
    // 1990 PRINT"FOR LEVEL";F(5)
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    sprintf(outstring, "FOR LEVEL %d", (int) F[5]);
    print_text(screen, outstring);
    free(outstring);
    // 2000 RETURN
}

int AS, OT, P;

void lines2010_2250(screen_t *screen) {
    // 2010 CLS:PRINT tab(0,3);"PREPARE HERO TAPE"
    clear_screen(screen);
    tab(screen->cursor, 0, 3);
    print_text(screen, "PREPARE HERO TAPE");
    // 2020 LET M$=T$(10):GOSUB370
    free(M$);
    M$ = (char *) malloc(sizeof(char) * (strlen(T$[10]) + 1));
    if (M$ == NULL) {
        fprintf(stderr, "M$ is NULL!\n");
        exit(1);
    }
    strcpy(M$, T$[10]);
    lines370_420(screen);
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
    OT = (int) S$[0] - AS;
    // 2080 FOR I= 1 TO 8
    for (I = 1; I <= 8; I += 1) {
    // 2090 LET F(I)=ASC(MID$(S$,P,1)) - AS
        F[I] = (int) S$[P - 1] - AS;
    // 2100 LET P=P+1
        P += 1;
    // 2110 NEXT I
    }
    // 2120 FOR I=1 TO OT
    for (I = 1; I <= OT; I += 1) {
    // 2130 LET O(I)=ASC(MID$(S$,P,1))-AS
        O[I] = (int) S$[P - 1] - AS;
    // 2140 LET P=P+1
        P += 1;
    // 2150 NEXT I
    }
    // 2160 LET GC=ASC(MID$(S$,P,1))-AS
    GC = (int) S$[P - 1] - AS;
    // 2170 LET TR=ASC(MID$(S$,P+1,1))-AS
    TR = (int) S$[P] - AS;
    // 2180 LET C$=RIGHT$(S$,LEN(S$)-(P+1))
    free(C$);
    C$ = (char *) malloc(sizeof(char) * (P + 2));
    if (C$ == NULL) {
        fprintf(stderr, "C$ is NULL!\n");
        exit(1);
    }
    strcpy(C$, S$ + P + 1);
    // 2190 LET S1=F(1):LET S2=F(2):LET S3=F(5)
    S1 = F[1];
    S2 = F[2];
    S3 = F[5];
    // 2200 FOR I=1 TO 2
    for (I = 1; I <= 2; I += 1) {
    // 2210 FOR J=1 TO 3
        for (J = 1; J <= 3; J += 1) {
    // 2220 LET M((I-1)*3+J)=O(16+I)*F(7)
            M[(I - 1) * 3 + J] = O[16 + I] * F[7];
    // 2230 NEXT J:NEXT I
        }
    }
    // 2240 IF O(16)=1 THEN LET LT=20
    if (O[16] == 1) {
        LT = 20;
    }
    // 2250 RETURN
    free(S$);
}

void lines2260_2490(screen_t *screen) {
    // 2260 LET M$="ONE MOMENT PLEASE":GOSUB430
    free(M$);
    M$ = (char *) malloc(sizeof(char) * 18);
    if (M$ == NULL) {
        fprintf(stderr, "M$ is NULL!\n");
        exit(1);
    }
    strcpy(M$, "ONE MOMENT PLEASE");
    lines430_430(screen);
    // 2270 LET S$="":LET T$=""
    char * S$ = (char *) malloc(sizeof(char) * (12 + OT + strlen(C$)));
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
    T$[t_index] = (char) (OS + LE);
    t_index += 1;
    T$[t_index] = 0;
    // 2350 LET S$=S$+CHR$(AS+OT)
    S$[s_index] = (char) (AS + OT);
    s_index += 1;
    // 2360 FOR I=1 TO 8
    for (I = 1; I <= 8; I += 1) {
    // 2370 LET S$=S$+CHR$(F(I)+AS)
        S$[s_index] = (char) (F[I] + AS);
        s_index += 1;
    // 2380 NEXT I
    }
    // 2390 FOR I=1 TO OT
    for (I = 1; I <= OT; I += 1) {
    // 2400 LET S$=S$+CHR$(O(I)+AS)
        S$[s_index] = (char) (O[I] + AS);
        s_index += 1;
    // 2410 NEXT I
    }
    // 2420 LET S$=S$+CHR$(GC+AS);
    S$[s_index] = (char) (GC + AS);
    s_index += 1;
    // 2430 LET S$=S$+CHR$(TR+AS);
    S$[s_index] = (char) (TR + AS);
    s_index += 1;
    // 2440 LET S$=S$+C$
    strcpy(S$ + s_index, C$);
    s_index += strlen(C$);
    S$[s_index] = 0;
    // 2450 LET M$="ANY KEY TO SAVE":GOSUB 370
    free(M$);
    M$ = (char *) malloc(sizeof(char) * 16);
    if (M$ == NULL) {
        fprintf(stderr, "M$ is NULL!\n");
        exit(1);
    }
    strcpy(M$, "ANY KEY TO SAVE");
    lines370_420(screen);
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
    FI = 1;
    // 2490 RETURN
}

void lines2930_3200(int *C1, int *C5, int *C6);

int RE;

void lines2500_2780(int *C1, int *C5, int *C6) {
    // 2500 LET C$="ROLE PLAYING GAME":LET B$=""
    free(C$);
    C$ = (char *) malloc(sizeof(char) * 18);
    if (C$ == NULL) {
        fprintf(stderr, "C$ is NULL!\n");
        exit(1);
    }
    strcpy(C$, "ROLE PLAYING GAME");
    // dungeon_lib removes the need for B$
    // 2510 LET W=40:LET OS=96
    W = 40;
    OS = 96;
    // 2520 FOR I=1 TO W:LET B$=B$+" ":NEXT I
    // dungeon_lib removes the need for B$
    // 2530 DIM R(15,15),F(8),O(24)
    R = (int **) malloc(sizeof(int *) * 16);
    if (R == NULL) {
        fprintf(stderr, "R is NULL!\n");
        exit(1);
    }
    int i;
    for (i = 0; i < 16; i += 1) {
        R[i] = (int *) malloc(sizeof(int) * 16);
        if (R[i] == NULL) {
            fprintf(stderr, "R[%i] is NULL!\n", i);
            exit(1);
        }
    }
    F = (double *) malloc(sizeof(double) * 9);
    if (F == NULL) {
        fprintf(stderr, "F is NULL!\n");
        exit(1);
    }
    // 2540 DIM W$(11),T$(12)
    W$ = (const char **) malloc(sizeof(const char *) * (12));
    if (W$ == NULL) {
        fprintf(stderr, "W$ is NULL!\n");
        exit(1);
    }
    // 2550 DIM M(6),D(4,2),T(18)
    M = (int *) malloc(sizeof(int) * 7);
    if (M == NULL) {
        fprintf(stderr, "M is NULL!\n");
        exit(1);
    }
    D = (int **) malloc(sizeof(int *) * 5);
    if (D == NULL) {
        fprintf(stderr, "D is NULL!\n");
        exit(1);
    }
    for (i = 0; i < 5; i += 1) {
        D[i] = (int *) malloc(sizeof(int) * 3);
        if (D[i] == NULL) {
            fprintf(stderr, "D[%i] is NULL!\n", i);
            exit(1);
        }
    }
    T = (int *) malloc(sizeof(int) * 19);
    if (T == NULL) {
        fprintf(stderr, "T is NULL!\n");
        exit(1);
    }
    // 2560 DATA"GR SWORD","SWORD","AXE","MACE","FLAIL","DAGGER","ARMOUR","ARMOUR"
    // 2570 DATA"ARMOUR","HELMET","HEADPC."
    // 2580 FOR I = 1 TO 11
    // 2590 READ W$(I)
    // 2600 NEXT I
    W$[1] = "GR SWORD";
    W$[2] = "SWORD";
    W$[3] = "AXE";
    W$[4] = "MACE";
    W$[5] = "FLAIL";
    W$[6] = "DAGGER";
    W$[7] = "ARMOUR";
    W$[8] = "ARMOUR";
    W$[9] = "ARMOUR";
    W$[10] = "HELMET";
    W$[11] = "HEADPC.";

    // 2610 DATA"A GOOD BLOW","WELL HIT SIRE","THY AIM IS TRUE","MISSED!","HIT THEE!!"
    // 2620 DATA"THE MONSTER IS SLAIN","NO LIGHT","BROKEN THY ","SPELL EXHAUSTED"
    // 2630 DATA"PRESS ANY KEY","YOU NEED EXPERIENCE","EXIT FROM THIS LEVEL"
    // 2640 FOR I = 1 TO 12
    // 2650 READ T$(I)
    // 2660 NEXT I

    T$ = (const char **) malloc(sizeof(const char *) * (13));
    if (T$ == NULL) {
        fprintf(stderr, "T$ is NULL!\n");
        exit(1);
    }

    T$[1] = "A GOOD BLOW";
    T$[2] = "WELL HIT SIRE";
    T$[3] = "THY AIM IS TRUE";
    T$[4] = "MISSED!";
    T$[5] = "HIT THEE!!";
    T$[6] = "THE MONSTER IS SLAIN";
    T$[7] = "NO LIGHT";
    T$[8] = "BROKEN THY ";
    T$[9] = "SPELL EXHAUSTED";
    T$[10] = "PRESS ANY KEY";
    T$[11] = "YOU NEED EXPERIENCE";
    T$[12] = "EXIT FROM THIS LEVEL";


    // 2670 DATA0,-1,1,0,0,1,-1,0
    // 2680 FOR I=1 TO 4:READ D(I,1),D(I,2):NEXT I
    D[1][1] = 0;
    D[1][2] = -1;
    D[2][1] = 1;
    D[2][2] = 0;
    D[3][1] = 0;
    D[3][2] = 1;
    D[4][1] = -1;
    D[4][2] = 0;

    // 2690 LET FI=0:LET DX=255:LET NF=0
    FI = 0;
    DX = 255;
    NF = 0;
    // 2700 LET TX=0:LET TY=0:LET TF=0:LET TR=0
    TX = 0;
    TY = 0;
    TF = 0;
    TR = 0;
    // 2710 LET MX=0:LET MY=0:LET DY=12:LET F$=""
    MX = 0;
    MY = 0;
    DY = 12;
    F$ = (char *) malloc(sizeof(char) * 7);
    if (F$ == NULL) {
        fprintf(stderr, "F$ is NULL!\n");
        exit(1);
    }
    // 2720 LET NX=1:LET NY=1:LET RE=0:LET LT=0
    NX = 1;
    NY = 1;
    RE = 0;
    LT = 0;
    // 2730 FOR I = 1 TO 5
    for (I = 1; I <= 5; I += 1) {
    // 2740 LET F$=F$+CHR$(OS+I)
        F$[I] = OS + I;
    // 2750 NEXT I
    }
    F$[6] = 0;
    // 2760 DATA69,117,73,121,81,129,69,117,73,121,81,129,89,137,97,145,101,149
    // 2770 FOR I=1 TO 18:READ T(I):NEXT I:GOSUB 2930
    T[1] = 69;
    T[2] = 117;
    T[3] = 73;
    T[4] = 121;
    T[5] = 81;
    T[6] = 129;
    T[7] = 69;
    T[8] = 117;
    T[9] = 73;
    T[10] = 121;
    T[11] = 81;
    T[12] = 129;
    T[13] = 89;
    T[14] = 137;
    T[15] = 97;
    T[16] = 145;
    T[17] = 101;
    T[18] = 149;
    lines2930_3200(C1, C5, C6);
    // 2780 RETURN
}

void lines2790_2920(screen_t *screen) {
    // 2790 paper 1:CLS
    paper(screen->cursor, 1);
    clear_screen(screen);
    tab(screen->cursor, 1, 1);
    // 2800 paper 3:ink 0
    paper(screen->cursor, 3);
    ink(screen->cursor, 0);
    // 2810 PRINT C$;LEFT$(B$(W-LEN(C$));
    print_text(screen, C$);
    print_left$_b$(screen, W - strlen(C$));
    // 2820 paper 2:ink 3
    paper(screen->cursor, 2);
    ink(screen->cursor, 3);
    // 2830 FOR I=1 TO 5:PRINT LEFT$(B$,W);:NEXT I
    for (I = 1; I <= 5; I += 1) {
        print_left$_b$(screen, W);
        newline(screen->cursor);
    }
    // 2840 paper 0:ink 1
    paper(screen->cursor, 0);
    ink(screen->cursor, 1);
    // 2850 FOR I=1 TO 15:PRINT tab(1,5+I);LEFT$(B$,15);:NEXT I
    for (I = 1; I <= 15; I += 1) {
        tab(screen->cursor, 1, 5 + I);
        print_left$_b$(screen, 15);
    }
    // 2860 paper 1:ink 3
    paper(screen->cursor, 1);
    ink(screen->cursor, 3);
    // 2870 PRINT tab(16,7);"STR";
    tab(screen->cursor, 16, 7);
    print_text(screen, "STR");
    // 2880 PRINT tab(16,10);"VIT";
    tab(screen->cursor, 16, 10);
    print_text(screen, "VIT");
    // 2890 PRINT tab(16,13);"AUR";
    tab(screen->cursor, 16, 13);
    print_text(screen, "AUR");
    // 2900 PRINT tab(16,16);"FACE";
    tab(screen->cursor, 16, 16);
    print_text(screen, "FACE");
    // 2910 PRINT tab(16,19);"EXP";
    tab(screen->cursor, 16, 19);
    print_text(screen, "EXP");
    // 2920 RETURN
}

int C8;

void lines2930_3200(int *C1, int *C5, int *C6) {
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
    AS = 65;
    C0 = OS + 6;
    // 3180 LET C1=C0+1:LET C2=C0+2:LET C3=C0+3:LET C4=C0+4
    *C1 = C0 + 1;
    C2 = C0 + 2;
    C3 = C0 + 3;
    C4 = C0 + 4;
    // 3190 LET C5=C0+6:LET C6=C0+7:LET C7=C0+8:LET C8=C0+12
    *C5 = C0 + 6;
    *C6 = C0 + 7;
    C7 = C0 + 8;
    C8 = C0 + 12;

    // 3200 RETURN
}
