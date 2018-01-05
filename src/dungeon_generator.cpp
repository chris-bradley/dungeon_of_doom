

// #ifdef HAVE_CONFIG_H
// #include <config.h>
// #endif
//
// #include <iostream>
// #include <cstdlib>
//
// using namespace std;

#include <bits/stdc++.h>
#include <SDL.h>
#include <unistd.h>
#include "dungeon_lib.h"

using namespace std;

void lines230_270();
void lines280_350(screen_t *screen, int BG, int FG, int T, int L, int LW);
void lines360_420(screen_t *screen);
void lines430_440();
void lines450_600(screen_t *screen);
void lines610_690();
void lines700_770();
void lines790_800();
void lines810_840();
void lines5000_5080();

int W;

int LE, X, Y, OS;
int R[16][16];
char *I$;
int IX, IY, CO;

int main(int argc, char *argv[]) {
    // 5 GOSUB 5000
    lines5000_5080();
    // GOSUB 610

    lines610_690();
    // Clear screen; Black background.
    // 20 PRINT CHR$(147): POKE 53280,0:POKE 53281,0
    screen_t *screen = NULL;
    if (init_screen(&screen) < 0) {
        return 1;
    }
    // 30 LET BG=2:LET FG=1:LET T=0:LET L=3:LET LW=W-3:GOSUB 280
    lines280_350(screen, 2, 1, 0, 3, W - 3);
    // 40 paper 2:ink 0
    paper(screen->cursor, 2);
    ink(screen->cursor, 0);
    // 50 PRINT tab(1,1);"LEVEL GENERATOR"
    tab(screen->cursor, 1, 1);
    print_text(screen, "LEVEL GENERATOR");
    // 60 PRINT tab(1,2);"THIS IS LEVEL:";LE;
    tab(screen->cursor, 1, 2);
    char* outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
       fprintf(stdout, "Allocating outstring failed!");
       exit(1);
    }
    snprintf(outstring, 40, "THIS IS LEVEL: %i", LE);
    print_text(screen, outstring);
    free(outstring);
    // 70 PRINT tab(1,3);"PRESS H FOR HELP"
    tab(screen->cursor, 1, 3);
    print_text(screen, "PRESS H FOR HELP");

    // 80 LET BG=3:LET FG=2:LET T=5:LET L=15:LET LW=15:GOSUB 280
    lines280_350(screen, 3, 2, 5, 15, 15);
    SDL_RenderPresent(screen->ren);

    // 90 LET X=1:LET Y=1
    X = 1;
    Y = 1;

    // 100 LET I$=inkey$
    I$ = (char *) malloc(sizeof(char));
    int done = 0;
    while (!done) {
	*I$ = inkey$();
        // 110 IF I$="H" THEN GOSUB 360
        // 120 IF I$="A" AND Y>1 THEN LET Y=Y-1
        // 130 IF I$="Z" AND Y<15 THEN LET Y=Y+1
        // 140 IF I$="N" AND X>1 THEN X=X-1
        // 150 IF I$="M" AND X<15 THEN LET X=X+1
        // 160 IF I$>"/" AND I$<":" THEN GOSUB 230

        if (*I$ == 'h') {
            lines360_420(screen);
        } else if (*I$ == 'a' and Y > 1) {
            Y -= 1;
        } else if (*I$ == 'z' and Y < 15) {
            Y += 1;
        } else if (*I$ == 'n' and X > 1) {
            X -= 1;
        } else if (*I$ == 'm' and X < 15) {
            X += 1;
        } else if (*I$ > '/' and *I$ < ':') {
            lines230_270();
        }
        // 170 paper 3:ink 0
        paper(screen->cursor, 3);
        ink(screen->cursor, 0);
        // 180 PRINT tab(X,Y+5);CHR$(OS);
        tab(screen->cursor, X, Y + 5);
        char os_input[2];
        sprintf(os_input, "%s", (char *) &OS);
        print_text(screen, os_input);
        tab(screen->cursor, X, Y + 5);
        // 190 PRINT tab(X,Y+5);CHR$(R(X,Y));
        sprintf(os_input, "%s", (char *) &R[X][Y]);
        print_text(screen, os_input);
        SDL_RenderPresent(screen->ren);
        // 200 IF I$="S" AND IX>0 THEN GOSUB 450:GOTO 20
        if (*I$ == 's' && IX > 0) {
            lines450_600(screen);
        }
        // 210 IF I$<>"F" THEN GOTO 100
        if (*I$ == 'f') {
            done = SDL_TRUE;
        }
    }
    // 220 STOP
    free(I$);
    destroy_screen(screen);

    return 0;
}

void lines230_270() {
    // 230 LET I=VAL(I$)
    int I = atoi(I$);
    // 240 IF I=9 THEN LET I=8+rnd(3)
    if (I == 9) {
        I = 9 + (rand() % 3);
    }
    // 250 IF I=5 THEN LET IX=X:LET IY=Y
    else if (I == 5) {
       IX = X;
       IY = Y;
    }
    // 260 LET R(X,Y)=CO+I
    R[X][Y] = CO + I;
    // 270 RETURN
}


void lines280_350(screen_t *screen, int BG, int FG, int T, int L, int LW) {
    // 280 PRINT tab(0,T);
    tab(screen->cursor, 0, T);
    // 290 paper FG:PRINT LEFT$(B$,LW+2)
    paper(screen->cursor, FG);
    print_left$_b$(screen, LW + 2);
    newline(screen->cursor);
    // 300 paper BG:ink FG
    paper(screen->cursor, BG);
    ink(screen->cursor, FG);
    // 310 FOR I=1 TO L
    for (int I = 1; I <= L; I +=1) {
    // 320 PRINT BG$(FG);" ";BG$(BG);LEFT$(B$,LW);BG$(FG);" "
        paper(screen->cursor, FG);
        print_text(screen, " ");
        paper(screen->cursor, BG);
        print_left$_b$(screen, LW);
        paper(screen->cursor, FG);
        print_text(screen, " ");
        newline(screen->cursor);
    // 330 NEXT I
    }
    // 340 paper FG:PRINT LEFT$(B$,LW+2);
    paper(screen->cursor, FG);
    print_left$_b$(screen, LW + 2);
    SDL_RenderPresent(screen->ren);
    // 350 RETURN

}

const char * H$[10];

void lines360_420(screen_t *screen) {

    int H;
    // 360 paper 1:ink 3
    paper(screen->cursor, 1);
    ink(screen->cursor, 3);
    // 370 FOR H = 1 TO 10
    for (H = 0; H < 10; H += 1) {
    // 380 PRINT tab(1,4);H$(H);:GOSUB 430
        tab(screen->cursor, 1, 4);
        print_text(screen, H$[H]);
        SDL_RenderPresent(screen->ren);
        lines430_440();
    // 390 PRINT tab(1,4);LEFT$(B$,W-2);
        tab(screen->cursor, 1, 4);
        print_left$_b$(screen, W - 2);
        SDL_RenderPresent(screen->ren);
    // 400 NEXT H
    }
    // 410 ink 3
    ink(screen->cursor, 3);
    // 420 RETURN
}

void lines430_440() {
    // 430 LET G$=inkey$:IF G$="" THEN GOTO 430
    // 440 RETURN
    inkey$();
}

void lines450_600(screen_t *screen) {
    // 450 PRINT tab(1, 4);"ONE MOMENT PLEASE.";
    tab(screen->cursor, 1, 4);
    print_text(screen, "ONE MOMENT PLEASE");
    // 460 LET S$=""
    char S$[239];
    // 470 FOR J=1 TO 15
    int J, K;
    for (J = 1; J <= 15; J += 1) {
    // 480 FOR K=1 TO 15
        for (K = 1; K <= 15; K += 1) {
    // 490 LET S$=S$+CHR$(R(K,J))
            S$[(J - 1) * 15 + K - 1] = (char) R[J][K];

    // 500 NEXT K
        }
    // 510 NEXT J
    }
    // 520 LET S$=S$+CHR$(IX+OS):LET S$=S$+CHR(IY+OS)
    S$[225] = (char) IX + OS;
    S$[226] = (char) IY + OS;
    // 530 LET S$=S$+CHR$(LE+OS)
    S$[227] = (char) LE + OS;
    S$[228] = 0;
    // 540 PRINT tab(1,4);"ANY KEY TO SAVE   ";GOSUB 430
    tab(screen->cursor, 1, 4);
    print_text(screen, "ANY KEY TO SAVE   ");
    SDL_RenderPresent(screen->ren);
    lines430_440();
    // 550 LET S=OPENOUT "LEVEL"
    FILE *S = fopen("LEVEL", "w");
    // 560 PRINT#S,S$
    int error = fputs(S$, S);
    if (error) {
        fprintf(stderr, "Error %i writing the level!", error);
    }
    // 570 CLOSE#S
    error = fclose(S);
    if (error) {
        fprintf(stderr, "Error %i saving the level!", error);
    }
    // 580 PRINT tab(1,4);LEFT(B$,W)
    tab(screen->cursor, 1, 4);
    print_left$_b$(screen, W);
    SDL_RenderPresent(screen->ren);
    // 590 LET LE=LE+1:GOSUB 700
    LE = LE + 1;
    lines700_770();
    // 600 RETURN
}

void lines610_690() {
    // 610 DIM R(15,15),H$(10)
    // 620 GOSUB 790
    lines790_800();
    // 630 DATA "PRESS ANY KEY","TO MOVE A Z N M","1 WALL    2 VASE"
    // 640 DATA "3 CHEST 4 * idol *","5 WAY IN  6 EXIT","7 TRAP", "8 SAFE PLACE"
    // 650 DATA "9 GUARD","0 TO ERASE","S TO SAVE"
    // 660 LET LE = 1
    LE = 1;
    // 670 FOR I=1 to 10
    // 680 READ H$(I)
    H$[0] = "PRESS ANY KEY     ";
    H$[1] = "TO MOVE A Z N M   ";
    H$[2] = "1 WALL    2 VASE  ";
    H$[3] = "3 CHEST 4 * IDOL *";
    H$[4] = "5 WAY IN  6 EXIT  ";
    H$[5] = "7 TRAP            ";
    H$[6] = "8 SAFE PLACE      ";
    H$[7] = "9 GUARD           ";
    H$[8] = "0 TO ERASE        ";
    H$[9] = "S TO SAVE         ";

    // 690 NEXT I:GOSUB 810
    lines810_840();
    lines700_770();
}

void lines700_770() {
    // 700 FOR J=1 to 15
    // 710 FOR K=1 to 15
    // 720 LET R(J,K) = CO
    // 730 NEXT K
    // 740 NEXT J
    for (int J=0; J<16; J++) {
        for (int K=0; K<16; K++) {
            R[J][K] = CO;
        }
    }
    // 750 LET IX=0:LET IY=0
    IX = 0;
    IY = 0;
    // 760 LET B$="":FOR I = 1 TO W:LET B$=B$+" ":NEXT I
    // dungeon_libs' print_left$_b$() removes the need for B$
    // 770 RETURN
}

void lines790_800()
{
  // 790 OS=96:CO=OS+6:W=40:GOSUB 4000
  OS = 96;
  CO = OS + 6;
  W = 40;
  // 800 RETURN
}

void lines810_840()
{
  // 810 REM SET UP THE CHARACTERS
  // 820 FOR I=0 TO 7:READ A:POKE 36352+I, 255-A:NEXT I
  // 830 FOR I=0 TO 95:READ A:POKE 36400+I, 255-A:NEXT I
  // 840 RETURN
}

// Graphic Memory setup stuff

void lines5000_5080()
{
    // Memory remapping; Not needed by us:
    // 5000 POKE 52, 128:POKE 56,128
    // Stop Timer A; Switch the character generator ROM ; Not needed by us?
    // 5010 POKE 56334, PEEK(56334) and 254: POKE 1, PEEK(1) AND 251
    // Copy first 2 KB of the character rom to upper basic area:
    // 5020 FOR I=0 TO 2047:POKE 34816+I, PEEK(53248+I):NEXT I
    // Stop Switch the character generator ROM
    // 5030 POKE 1, PEEK(1) AND 251
    // Start Timer A
    // 5040 POKE 56334.PEEK(56334) OR 1
    // Muck with Data Direction Register Port A?
    // 5050 POKE 56578,PEEK(56578) OR 3
    // Set VIC Chip System Memory Bank Select to 01
    // 5060 POKE 56576,(PEEK (56576) AND 252) OR 1
    // Set Video Matrix Base Address (Inside VIC) to 0000
    // Set Character Dot-Data Base Address (inside VIC) to 001
    // Set top of Screen Memory to 128 (ie. 0800)
    // 5070 POKE 53272,2:POKE 648,128
    // 5080 RETURN
}