#include <SDL.h>
#include <unistd.h>
#include <stdio.h>
#include "dungeon_lib.h"

void lines570_600(screen_t *screen, int BR, int C, int J, int *H, int K,
                  int N, int F[5][9], int * O, const char * C$[5],
                  const char * H$, char * M$, int P[24], const char * O$[25]);
void lines610_670(int BR, int J, int *H, int K, int N, int OF, int Y,
                  int F[5][9], int * O, char * M$, int P[24]);
void lines680_710(int C, int N, int *Y, const char * C$[5], char * M$,
                  const char * O$[25]);
void lines720_800(screen_t *screen, int D, int *K, int *P_, int T, char * I$);
void lines810_850(screen_t *screen, int J, int H, int *T, int W,
                  const char * H$, char * M$);
void lines860_890(screen_t *screen, int H, const char * H$, char * M$);
void lines900_910(screen_t *screen, int J, int *T, int W, int F[5][9]);
void lines920_970(screen_t *screen, int J, int T, int F[5][9]);
void lines1060_1590(int *AS, int *D, int *GC, int *MP, int *W, int F[5][9],
                    int ** O, const char * C$[5], char * M$, int P[24],
                    const char * O$[25]);
void lines1700_1730(screen_t *screen, int X, int Y, char * IN$);

int main(int argc, char *argv[]) {
    int AS, BR, C, D, GC, I, J, H, K, MP, N, O_, OF, P_, T, W, X, Y;
    int F[5][9];
    int * O;
    const char * C$[5], * H$, * O$[25];
    char * I$, * IN$ = NULL, * M$ = NULL, * N$;
    int P[24];
    // 10 GOSUB 1060
    lines1060_1590(&AS, &D, &GC, &MP, &W, F, &O, C$, M$, P, O$);
    // 20 paper 0:CLS
    screen_t *screen = NULL;
    if (init_screen(&screen) < 0) {
        return 1;
    }
    paper(screen->cursor, 0);
    // 30 LET J=1:LET H=MP:LET H$="POINTS"
    J = 1;
    H = MP;
    H$ = "POINTS";
    // 40 GOSUB 810:GOSUB900
    lines810_850(screen, J, H, &T, W, H$, M$);
    lines900_910(screen, J, &T, W, F);
    // 50 LET K=1:LET P=T+1
    K = 1;
    P_ = T + 1;
    // 60 PRINT tab(1,P);">";
    tab(screen->cursor, 1, P_);
    print_text(screen, ">");
    SDL_RenderPresent(screen->ren);
    // 70 GOSUB 720
    I$ = (char *) malloc(sizeof(char));
    if (I$ == NULL) {
        fprintf(stderr, "I$ is NULL!\n");
        exit(1);
    }
    do {
        lines720_800(screen, D, &K, &P_, T, I$);
    // 80 IF K=5 THEN GOTO 70
        while (K == 5) {
            lines720_800(screen, D, &K, &P_, T, I$);
        }
    // 90 IF I$=";" AND H>0 THEN LET F(J,K)=F(J,K)+1:LET H=H-1:GOSUB 920
        if (*I$ == ';' && H > 0) {
            F[J][K] += 1;
            H -= 1;
            lines920_970(screen, J, T, F);
        }
    // 100 IF I$="-" AND F(J,K)>1 THEN LET F(J,K)=F(J,K)-1:LET H=H+1:GOSUB 920
        if (*I$ == '-' && F[J][K] > 1) {
            F[J][K] -=1;
            H += 1;
            lines920_970(screen, J, T, F);
        }
    // 110 LET C=1
        C = 1;
    // 120 IF F(1,4)>6 AND F(1,8)>7 THEN LET C=2
        if (F[1][4] > 6 && F[1][8] > 7) {
            C = 2;
        }
    // 130 IF F(1,4)>8 AND F(1,7)>7 THEN LET C=3
        if (F[1][4] > 8 && F[1][7] > 7) {
            C = 3;
        }
    // 140 IF F(1,1)>7 AND F(1,8)>5 AND F(1,1)+F(1,2)>10 THEN LET C=4
        if (F[1][1] > 7 && F[1][8] > 5 && F[1][1] + F[1][2] > 10) {
            C = 4;
        }
    // 150 IF F(1,1)>8 AND F(1,2)+F(1,3)>12 and F(1,8)<6 THEN LET C=5
        if (F[1][1] > 8 && F[1][2] + F[1][3] > 12 && F[1][8]) {
            C = 5;
        }
    // 160 LET M$=C$(C)
        strcpy(M$, C$[C]);
    // 170 GOSUB 860
        lines860_890(screen, H, H$, M$);
        SDL_RenderPresent(screen->ren);
    // 180 IF I$<>" " THEN GOTO 70
    } while (*I$ != ' ');
    // 190 LET H=GC:LET H$="GOLD COINS:"
    H = GC;
    H$ = "GOLD COINS";
    // 200 FOR J=2 TO 4
    for (J = 2; J <= 4; J += 1) {
    // 210 LET K=1:LET P=T+1
        K = 1;
        P_ = T + 1;
    // 220 LET M$="CHOOSE WELL SIRE!"
        strcpy(M$, "CHOOSE WELL SIRE!");
    // 230 GOSUB 810
        lines810_850(screen, J, H, &T, W, H$, M$);
    // 240 GOSUB 900
        lines900_910(screen, J, &T, W, F);
    // 250 PRINT tab(1,P);">";
        tab(screen->cursor, 1, P_);
        print_text(screen, ">");
        SDL_RenderPresent(screen->ren);
    // 260 GOSUB 720
        do {
            lines720_800(screen, D, &K, &P_, T, I$);
    // 270 LET N=8*(J-2)+K
            N = 8 * (J - 2) + K;
    // 280 LET M$="MAKE YOUR CHOICE"
            strcpy(M$, "MAKE YOUR CHOICE");
    // 290 GOSUB 680
            lines680_710(C, N, &Y, C$, M$, O$);
    // 300 LET BR=0:LET OF=0
            BR = 0;
            OF = 0;
    // 310 IF I$=";" THEN LET OF=F(J,K):GOSUB 610
            if (*I$ == ';') {
                OF = F[J][K];
                lines610_670(BR, J, &H, K, N, OF, Y, F, O, M$, P);
            }
    // 320 IF I$="-" THEN LET BR=rnd(3):GOSUB 570
            if (*I$ == '-') {
                BR = rand() % 3;
                lines570_600(
                    screen, BR, C, J, &H, K, N, F, O, C$, H$, M$, P, O$
                );
            }
    // 330 GOSUB 860
            lines860_890(screen, H, H$, M$);
    // 340 IF I$<>" " THEN GOTO 260
        } while (*I$ != ' '); 
    // 350 NEXT J
    }
    N$ = (char *) malloc(sizeof(char) * 40);
    if (N$ == NULL) {
        fprintf(stderr, "N$ is NULL!\n");
        exit(1);
    }
    do {
    // 360 PRINT tab(1,2);"NAME THY CHARACTER";
        tab(screen->cursor, 1, 2);
        print_text(screen, "NAME THY CHARACTER");
    // 370 PRINT tab(1,3);LEFT$(B$,W-2);:PRINT tab(1,3);
        tab(screen->cursor, 1, 3);
        print_left$_b$(screen, W - 2);
        SDL_RenderPresent(screen->ren);
        tab(screen->cursor, 1, 3);
    // 380 INPUT N$

    // C64 VERSION: 380 X=1:Y=3:GOSUB 1700:N$=IN$
        X = 1;
        Y = 3;
        lines1700_1730(screen, X, Y, IN$);
        strcpy(N$, IN$);
        free(IN$);
    // 390 IF LEN(N$)>10 THEN GOTO 360
    } while (strlen(N$) > 10);
    // 400 PRINT tab(1,3);"ONE MOMENT PLEASE";
    tab(screen->cursor, 1, 3);
    print_text(screen, "ONE MOMENT PLEASE");
    SDL_RenderPresent(screen->ren);
    // 410 PRINT tab(1,3);
    tab(screen->cursor, 1, 3);
    // 420 LET O=D*3
    O_ = D * 3;
    // 430 LET S$=CHR$(O+AS)

    char * S$ = (char *) malloc(
        sizeof(char) * (14 + O_ + strlen(N$) + strlen(C$[C]))
    );
    if (S$ == NULL) {
        fprintf(stderr, "S$ is NULL!\n");
        exit(1);
    }

    S$[0] = char (O_ + AS);
    // 440 FOR I=1 TO 8
    for (I = 1; I <= 8; I +=1 ) {
    // 450 LET S$=S$+CHR(F(1,I)+AS)
        S$[I] += char (F[1][I] + AS);
    }
    // 460 NEXT I
    // 470 FOR I = 1 TO O
    for (I = 1; I <= O_; I += 1) {
    // 480 LET S$=S$+CHR$(O(I)+AS)
        S$[8 + I] = char (O[I] + AS);
    // 490 NEXT I
    }
    // 500 LET S$=S$+CHR$(H+AS)
    S$[9 + O_] = char (H + AS);
    // 510 LET S$=S$+CHR$(AS)
    S$[10 + O_] = (char) AS;
    // 520 LET S$=S$+N$+" -"+C$(C)
    strcpy(S$ + 11 + O_, N$);
    strcpy(S$ + 11 + O_ + strlen(N$), " -");
    strcpy(S$ + 13 + O_ + strlen(N$), C$[C]);
    S$[13 + O_ + strlen(N$) + strlen(C$[C])] = 0;
    // 530 LET S=OPENOUT "HERO"
    FILE *S = fopen("HERO", "w");
    // 540 PRINT#S,S$
    int error = fputs(S$, S);
    if (error) {
        fprintf(stderr, "Error %i writing the character!", error);
    }

    // 550 CLOSE#S
    error = fclose(S);
    if (error) {
        fprintf(stderr, "Error %i saving the character!", error);
    }
    free(S$);
    // 560 STOP

    free(I$);
    free(M$);
    free(N$);
    free(O);

    destroy_screen(screen);

    return 0;
}

void lines570_600(screen_t *screen, int BR, int C, int J, int *H, int K,
                  int N, int F[5][9], int * O, const char * C$[5],
                  const char * H$, char * M$, int P[24], const char * O$[25]) {
    int OF, X, Y;
    char * IN$ = NULL;
    // 570 LET M$="";GOSUB 860
    strcpy(M$, "");
    lines860_890(screen, *H, H$, M$);
    // 580 PRINT tab(2,2);"YOUR OFFER";
    tab(screen->cursor, 2, 2);
    print_text(screen, "YOUR OFFER");
    SDL_RenderPresent(screen->ren);
    // 590 INPUT OF
    // C64 Version: 590 X = 14:Y=2:GOSUB 1700:OF=VAL(IN$)
    X = 14;
    Y = 2;
    lines1700_1730(screen, X, Y, IN$);
    OF = atoi(IN$);
    free(IN$);
    // 600 GOSUB 680
    lines680_710(C, N, &Y, C$, M$, O$);
    lines610_670(BR, J, H, K, N, OF, Y, F, O, M$, P);
}

void lines610_670(int BR, int J, int *H, int K, int N, int OF, int Y,
                  int F[5][9], int * O, char * M$, int P[24]) {
    int PR;
    // 610 IF O(N)>0 AND N<23 THEN LET M$="YOU HAVE IT SIRE":RETURN
    if (O[N] > 0 && N < 23) {
        strcpy(M$, "YOU HAVE IT SIRE");
    } else {
    // 620 LET PR=F(J,K)-BR
        PR = F[J][K] - BR;
    // 630 IF H<PR THEN LET M$="YOU CANNOT AFFORD":RETURN
        if (*H < PR) {
            strcpy(M$, "YOU CANNOT AFFORD");
        } else {
    // 640 IF OF>=PR AND Y=1 THEN LET O(N)=O(N)+P(N):LET H+H-PR:LET M$="TIS YOURS!"
            if (OF >= PR && Y == 1) {
                O[N] += P[N];
                *H -= PR;
                strcpy(M$, "TIS YOURS!");
            }
    // 650 IF OF<PR AND Y=1 THEN LET M$="OFFER REJECTED";
            if (OF < PR && Y == 1) {
                strcpy(M$, "OFFER REJECTED");
            }
    // 660 IF H<0 THEN LET H=0
            if (*H < 0) {
                *H = 0;
            }
        }
    }
    // 670 RETURN
}


void lines680_710(int C, int N, int *Y, const char * C$[5], char * M$,
                  const char * O$[25]) {
    // 680 LET Y=0
    *Y = 0;
    // 690 IF MID$(O$(N),C,1)="1" THEN LET Y=1
    if (O$[N][C] == '1') {
        *Y = 1;
    }
    // 700 IF Y=0 THEN LET M$="NOT FOR "+C$(C)
    if (*Y == 0) {
        sprintf(M$, "NOT FOR %s", C$[C]);
    }
    // 710 RETURN
}

void lines720_800(screen_t *screen, int D, int *K, int *P_, int T, char * I$) {
    // 720 LET I$=inkey$;
    // 730 IF I$="" THEN GOTO 720
    *I$ = inkey$();
    // 740 paper 3:ink 1
    paper(screen->cursor, 3);
    ink(screen->cursor, 1);
    // 750 print tab(1,P);" ";
    tab(screen->cursor, 1, *P_);
    print_text(screen, " ");
    // 760 IF I$="A" AND K>1 THEN LET K=K-1
    if (*I$ == 'a' && *K > 1) {
        *K -= 1;
    }
    // 770 IF I$="Z" AND K<D THEN LET K=K+1
    else if (*I$ == 'z' && *K < D) {
        *K += 1;
    }
    // 780 LET P=K*2+T-1
    *P_ = *K * 2 + T - 1;
    // 790 PRINT tab(1,P);">";
    tab(screen->cursor, 1, *P_);
    print_text(screen, ">");
    // 800 RETURN
}

void lines980_1050(screen_t *screen, int T, int W, int BG);

int FG, L;
const char * F$[5][10];
void lines810_850(screen_t *screen, int J, int H, int *T, int W,
                  const char * H$, char * M$) {
    int BG;
    // 810 paper 0:ink 2
    paper(screen->cursor, 0);
    ink(screen->cursor, 2);
    // 820 PRINT tab(0,0);LEFT$(B$,W);
    tab(screen->cursor, 0, 0);
    print_left$_b$(screen, W);
    // 830 PRINT tab(0,0);F$(J,9)
    tab(screen->cursor, 0, 0);
    print_text(screen, F$[J][9]);
    // 840 LET BG=2:LET FG=3:LET T=1:LET L=2
    BG = 2;
    FG = 3;
    *T = 1;
    L = 2;
    // 850 GOSUB 980
    lines980_1050(screen, *T, W, BG);
    lines860_890(screen, H, H$, M$);
}

void lines860_890(screen_t *screen, int H, const char * H$, char * M$) {
    // 860 paper 2:ink 0
    paper(screen->cursor, 2);
    ink(screen->cursor, 0);
    // 870 PRINT tab(2,2);LEFT$(B$,17);tab(2,2);M$;
    tab(screen->cursor, 2, 2);
    print_left$_b$(screen, 17);
    tab(screen->cursor, 2, 2);
    print_text(screen, M$);
    // C64: 875 PRINT HM$;LEFT$(CU$,3);SPC(15);LEFT$(B$,4);
    tab(screen->cursor, 15, 3);
    print_left$_b$(screen, 4);
    // 880 PRINT tab(2,3);H$;tab(15,3);H;" ";
    tab(screen->cursor, 2, 3);
    print_text(screen, H$);
    tab(screen->cursor, 15, 3);
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    sprintf(outstring, "%i ", H);
    print_text(screen, outstring);
    free(outstring);
    // 890 RETURN
}

void lines900_910(screen_t *screen, int J, int *T, int W, int F[5][9]) {
    int BG;
    // 900 LET BG=3:LET FG=2:LET T=5:LET L=15
    BG = 3;
    FG = 2;
    *T = 5;
    L = 15;
    // 910 GOSUB 980
    lines980_1050(screen, *T, W, BG);
    lines920_970(screen, J, *T, F);
}

void lines920_970(screen_t *screen, int J, int T, int F[5][9]) {
    int I, Y;
    // 920 paper 3:ink 0
    paper(screen->cursor, 3);
    ink(screen->cursor, 0);
    // 930 FOR I=1 TO 8
    for (I = 1; I <= 8; I += 1) {
    // 940 LET Y=T+(I-1)*2+1
        Y = T + (I - 1) * 2 + 1;
    // C64: 945 PRINT HM$;LEFT(CU$,Y);SPC(15);LEFT(B$,5);
        tab(screen->cursor, 15, Y);
        print_left$_b$(screen, 5);
    // 950 PRINT tab(2,Y);F$(J,I);tab(16,Y);F(J,I);" ";

        tab(screen->cursor, 2, Y);
        print_text(screen, F$[J][I]);
        tab(screen->cursor, 16, Y);
        char * outstring = (char *) malloc(sizeof(char) * 40);
        if (outstring == NULL) {
            fprintf(stderr, "outstring is NULL!\n");
            exit(1);
        }
        sprintf(outstring, "%i ", F[J][I]);
        print_text(screen, outstring);
        free(outstring);
    // 960 NEXT I
    }
    // 970 RETURN
}

void lines980_1050(screen_t *screen, int T, int W, int BG) {
    int I;
    // 980 PRINT tab(0,T);
    tab(screen->cursor, 0, T);
    // 990 paper FG:PRINT LEFT$(B$,W);
    paper(screen->cursor, FG);
    print_left$_b$(screen, W);
    newline(screen->cursor);
    // 1000 paper BG:ink FG
    paper(screen->cursor, BG);
    ink(screen->cursor, FG);
    // 1010 FOR I=1 TO L
    for (I = 1; I <= L; I += 1) {
    // 1020 PRINT CHR$(B);LEFT$(B$,W-2);CHR$(B);
    // C64: 1020 PRINT BG$(FG);" ";BG$(BG);LEFT$(B$,W-2);BG$(FG);" ";
        paper(screen->cursor, FG);
        print_text(screen, " ");
        paper(screen->cursor, BG);
        print_left$_b$(screen, W - 2);
        paper(screen->cursor, FG);
        print_text(screen, " ");
        // print_text() doesn't support wrapping yet, so we do our own newline:
        newline(screen->cursor);
    // 1030 NEXT I
    }
    // 1040 paper FG:PRINT LEFT$(B$,W);
    paper(screen->cursor, FG);
    print_left$_b$(screen, W);
    // 1050 RETURN
}

void lines1600_1650(int *W);

void lines1060_1590(int *AS, int *D, int *GC, int *MP, int *W, int F[5][9],
                    int ** O, const char * C$[5], char * M$, int P[24],
                    const char * O$[25]) {
    int I;
    // 1060 GOSUB 1600
    lines1600_1650(W);
    // 1070 LET D=8
    *D = 8;
    // 1080 DIM F(4,D+1)
    // 1090 DIM F$(4,D+1)
    // 1100 DIM C$(5)
    // 1110 DIM O(D*3)
    *O = (int *) malloc(sizeof(int) * (*D) * 3);
    if (*O == NULL) {
        fprintf(stderr, "*O is NULL!\n");
        exit(1);
    }
   
    int i;
    for (i = 0; i < *D * 3; i += 1) {
        O[i] = 0;
    }

    // 1120 DIM O$(D*3)
    // 1130 DIM P(D*3)
    // 1140 DATA "00001","00011","10011","10011","10011","00011","11111","10011"
    // 1150 DATA "00011","00011","10011","11111","00011","11011","11011","11111"
    // 1160 DATA "11100","00100","11100","10100","11100","11100","11111","11111"
    // 1170 FOR I=1 TO D*3
    // 1180 READ O$(I)
    // 1190 NEXT I
    O$[1] = "00001";
    O$[2] = "00011";
    O$[3] = "10011";
    O$[4] = "10011";
    O$[5] = "10011";
    O$[6] = "00011";
    O$[7] = "11111";
    O$[8] = "10011";
    O$[9] = "00011";
    O$[10] = "00011";
    O$[11] = "10011";
    O$[12] = "11111";
    O$[13] = "00011";
    O$[14] = "11011";
    O$[15] = "11011";
    O$[16] = "11111";
    O$[17] = "11100";
    O$[18] = "00100";
    O$[19] = "11100";
    O$[20] = "10100";
    O$[21] = "11100";
    O$[22] = "11100";
    O$[23] = "11111";
    O$[24] = "11111";
    // 1200 FOR I=1 TO 8
    for (I = 1; I <= 8; I += 1) {
    // 1210 LET F(1,I)=rnd(5)+2
        F[1][I] = (rand() % 5) + 2;
    // 1220 NEXT I
    }
    // 1230 LET F(1,5)=1
    F[1][5] = 1;
    // 1240 DATA 20,16,12,15,8,10,8,6
    // 1250 DATA 18,15,9,9,14,8,6,6
    // 1260 DATA 20,15,14,12,10,8,6,6
    // 1270 FOR J=2 TO 4
    // 1280 FOR I=1 TO 8
    // 1290 READ F(J,I)
    // 1300 NEXT I
    // 1310 NEXT J
    F[2][1] = 20;
    F[2][2] = 16;
    F[2][3] = 12;
    F[2][4] = 15;
    F[2][5] = 8;
    F[2][6] = 10;
    F[2][7] = 8;
    F[2][8] = 6;
    F[3][1] = 18;
    F[3][2] = 15;
    F[3][3] = 9;
    F[3][4] = 9;
    F[3][5] = 14;
    F[3][6] = 8;
    F[3][7] = 6;
    F[3][8] = 6;
    F[4][1] = 20;
    F[4][2] = 15;
    F[4][3] = 14;
    F[4][4] = 12;
    F[4][5] = 10;
    F[4][6] = 8;
    F[4][7] = 6;
    F[4][8] = 6;
    // 1320 DATA 5,4,3,3,2,2,1,1
    // 1330 DATA 5,4,3,1,2,1,3,1
    // 1340 DATA 4,3,2,2,3,1,1,1
    // 1350 FOR I=1 TO D*3
    // 1360 READ P(I)
    // 1370 NEXT I
    P[1] = 5;
    P[2] = 4;
    P[3] = 3;
    P[4] = 3;
    P[5] = 2;
    P[6] = 2;
    P[7] = 1;
    P[8] = 1;
    P[9] = 5;
    P[10] = 4;
    P[11] = 3;
    P[12] = 1;
    P[13] = 2;
    P[14] = 1;
    P[15] = 3;
    P[16] = 1;
    P[17] = 4;
    P[18] = 3;
    P[19] = 2;
    P[20] = 2;
    P[21] = 3;
    P[22] = 1;
    P[23] = 1;
    P[24] = 1;

    // 1380 DATA "STRENGTH","VITALITY","AGILITY","INTELLIGENCE"
    // 1390 DATA "EXPERIENCE","LUCK","AURA","MORALITY","CHARACTER CREATION"
    // 1400 DATA "2 HAND SWORD","BROADSWORD","SHORTSWORD"
    // 1410 DATA "AXE","MACE","FLAIL","DAGGER","GAUNTLET","ARMOURY"
    // 1420 DATA "HEAVY ARMOUR","CHAIN ARMOUR","LEATHER ARMOUR","HEAVY ROBE"
    // 1430 DATA "GOLD HELMET","HEADPIECE","SHIELD","TORCH","ACCOUTREMENTS"
    // 1440 DATA "NECRONOMICON","SCROLLS","RING","MYSTIC AMULET","SASH","CLOAK"
    // 1450 DATA "HEALING SALVE","POTIONS","EMPORIUM"
    // 1460 FOR J=1 TO 4
    // 1470 FOR I=1 TO 9
    // 1480 READ F$(J,I)
    // 1490 NEXT I
    // 1500 NEXT J
    F$[1][1] = "STRENGTH";
    F$[1][2] = "VITALITY";
    F$[1][3] = "AGILITY";
    F$[1][4] = "INTELLIGENCE";
    F$[1][5] = "EXPERIENCE";
    F$[1][6] = "LUCK";
    F$[1][7] = "AURA";
    F$[1][8] = "MORALITY";
    F$[1][9] = "CHARACTER CREATION";
    F$[2][1] = "2 HAND SWORD";
    F$[2][2] = "BROADSWORD";
    F$[2][3] = "SHORTSWORD";
    F$[2][4] = "AXE";
    F$[2][5] = "MACE";
    F$[2][6] = "FLAIL";
    F$[2][7] = "DAGGER";
    F$[2][8] = "GAUNTLET";
    F$[2][9] = "ARMOURY";
    F$[3][1] = "HEAVY ARMOUR";
    F$[3][2] = "CHAIN ARMOUR";
    F$[3][3] = "LEATHER ARMOUR";
    F$[3][4] = "HEAVY ROBE";
    F$[3][5] = "GOLD HELMET";
    F$[3][6] = "HEADPIECE";
    F$[3][7] = "SHIELD";
    F$[3][8] = "TORCH";
    F$[3][9] = "ACCOUTREMENTS";
    F$[4][1] = "NECRONOMICON";
    F$[4][2] = "SCROLLS";
    F$[4][3] = "RING";
    F$[4][4] = "MYSTIC AMULET";
    F$[4][5] = "SASH";
    F$[4][6] = "CLOAK";
    F$[4][7] = "HEALING SALVE";
    F$[4][8] = "POTIONS";
    F$[4][9] = "EMPORIUM";
    // 1510 DATA "WANDERER","CLERIC","MAGE","WARRIOR","BARBARIAN"
    // 1520 FOR I=1 TO 5
    // 1530 READ C$(I)
    // 1540 NEXT I
    C$[1] = "WANDERER";
    C$[2] = "CLERIC";
    C$[3] = "MAGE";
    C$[4] = "WARRIOR";
    C$[5] = "BARBARIAN";
    // 1550 LET MP=3+rnd(5)
    *MP = 3 + (rand() % 5);
    // 1560 LET GC=120+rnd(60)
    *GC = 120 + (rand() % 60);
    // 1570 LET M$="":LET AS=65
    M$ = (char *) malloc(sizeof(char) * 40);
    if (M$ == NULL) {
        fprintf(stderr, "M$ is NULL!\n");
        exit(1);
    }
    strcpy(M$, "");
    *AS = 65;
    // 1580 LET B$="":FOR I=1 TO W:LET B$=B$+" ":NEXT I
    // dungeon_libs' print_left$_b$() removes the need for B$
    // 1590 RETURN
}

void lines1600_1650(int *W) {
    // 1600 LET W=40
    *W = 40;
    // 1610 GOSUB 4000
    // not needed due to dungeon_lib
    // 1650 RETURN
}

void lines1700_1730(screen_t *screen, int X, int Y, char * IN$) {
    // 1700 IN$=""
    int ind = 0;
    char * I$ = (char *) malloc(sizeof(char));
    IN$ = (char *) malloc(sizeof(char) * 40);
    if (IN$ == NULL) {
        fprintf(stderr, "IN$ is NULL!\n");
        exit(1);
    }
    IN$[0] = 0;
    // 1710 GET I$:IF I$=CHR$(13) THEN RETURN
    SDL_Event event;
    int done = 0;
    int text_entered;
    while (not done) {
        text_entered = 0;
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_RETURN ||
                        event.key.keysym.sym == SDLK_RETURN2) {
                        done = 1 ;
                    }
                    break;
                case SDL_TEXTINPUT:
                    *I$ = event.text.text[0];
                    text_entered = 1;
            }
        }
    // 1720 IF I$>"/" AND I$<"[" THEN LET IN$=IN$+I$:PRINT HM$;LEFT$(CU$,Y);SPC(X);IN$;
        if (text_entered && *I$ > '/' && *I$ < ']' && ind < 39) {
            IN$[ind] = *I$;
            ind += 1;
            IN$[ind] = 0;
            tab(screen->cursor, X, Y);
            print_text(screen, IN$);
            SDL_RenderPresent(screen->ren);
        }
    // 1730 GOTO 1710
    }
    IN$[ind] = 0;
    free(I$);
}
