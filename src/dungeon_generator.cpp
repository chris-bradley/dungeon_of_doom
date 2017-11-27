

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
#include <SDL_ttf.h>
#include <unistd.h>

using namespace std;

// TODO: Investigate if the name is valid. The '_t' suffix is common but may
// not adhere to the POSIX standards.
typedef struct {
    int curs_x;
    int curs_y;
    Uint8 foreground_colour [4];
    Uint8 background_colour [4];
} cursor_t;

typedef struct {
    SDL_Renderer *ren;
    int zoom;
} screen_t;

void lines230_270();
void lines280_350(screen_t *screen, cursor_t *cursor);
void lines360_420(screen_t *screen, cursor_t *cursor);
void lines430_440();
void lines450_600(screen_t *screen, cursor_t *cursor);
void lines610_690();
void lines700_770();
void lines790_800();
void lines810_840();
void ink(cursor_t *cursor, int c_num);
void paper(cursor_t *cursor, int c_num);
void lines4000_4030();
void lines5000_5080();

SDL_Window *win;

int BG, FG, T, L, LW;
int W;

void print_text(screen_t *screen, cursor_t *cursor, const char *message) {
    TTF_Font *c64_font = TTF_OpenFont(
        "fonts/dungeon_of_doom.ttf",
        8 * screen->zoom
    );
    if (!c64_font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // Do some proper error handling.
        return;
    }
    int message_length = (int) strlen(message);
    SDL_Rect text_pos = {
        .x = cursor->curs_x * 8 * screen->zoom,
        .y = cursor->curs_y * 8 * screen->zoom,
        .w = message_length * 8 * screen->zoom,
        .h = 8 * screen->zoom
    };
    cursor->curs_x += message_length;

    Uint8 r, g, b;
    // Background
    int error = SDL_SetRenderDrawColor(
        screen->ren,
        cursor->background_colour[0],
        cursor->background_colour[1],
        cursor->background_colour[2],
        cursor->background_colour[3]
    );
    if (error) {
        printf("SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
    }
    error = SDL_RenderFillRect(screen->ren, &text_pos);
    if (error) {
        printf("SDL_RenderFillRect error: %s\n", SDL_GetError());
    }
    // Foreground
    r = cursor->foreground_colour[0];
    g = cursor->foreground_colour[1];
    b = cursor->foreground_colour[2];

    SDL_Color text_color = {
        .r = r,
        .g = g,
        .b = b
    };

    SDL_Surface *text_surface = TTF_RenderText_Solid(
        c64_font,
        message,
        text_color
    );

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(
        screen->ren,
        text_surface
    );

    text_pos.y += 1;
    text_pos.h += 1;

    error = SDL_RenderCopy(
        screen->ren,
        text_texture,
        NULL,
        &text_pos
    );
    if (error) {
        printf("SDL_RenderCopy error: %s\n", SDL_GetError());
        fflush(stdout);
    }
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    TTF_CloseFont(c64_font);
}

void tab(cursor_t *cursor, int x, int y) {
    cursor->curs_x = x;
    cursor->curs_y = y;
}

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

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL_Init error:" << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    TTF_Init();
    screen_t *screen = new screen_t;
    screen->zoom = 4;
    win = SDL_CreateWindow(
        "Dungeon of Doom",
        100 * screen->zoom,
        100 * screen->zoom,
        320 * screen->zoom,
        176 * screen->zoom,
        SDL_WINDOW_SHOWN
    );
    screen->ren = SDL_CreateRenderer(
        win,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (screen->ren == NULL) {
        SDL_DestroyWindow(win);
        cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    SDL_RenderClear(screen->ren);
    SDL_RenderPresent(screen->ren);

    cursor_t *cursor = new cursor_t;
    cursor->curs_x = 0;
    cursor->curs_y = 0;
    // 30 LET BG=2:LET FG=1:LET T=0:LET L=3:LET LW=W-3:GOSUB 280
    BG = 2;
    FG = 1;
    T = 0;
    L = 3;
    LW = W - 3;
    lines280_350(screen, cursor);
    // 40 paper 2:ink 0
    paper(cursor, 2);
    ink(cursor, 0);
    // 50 PRINT tab(1,1);"LEVEL GENERATOR"
    tab(cursor, 1, 1);
    print_text(screen, cursor, "LEVEL GENERATOR");
    // 60 PRINT tab(1,2);"THIS IS LEVEL:";LE;
    tab(cursor, 1, 2);
    char* outstring = new char[40];
    snprintf(outstring, 40, "THIS IS LEVEL: %i", LE);
    print_text(screen, cursor, outstring);
    delete outstring;
    // 70 PRINT tab(1,3);"PRESS H FOR HELP"
    tab(cursor, 1, 3);
    print_text(screen, cursor, "PRESS H FOR HELP");

    // 80 LET BG=3:LET FG=2:LET T=5:LET L=15:LET LW=15:GOSUB 280
    BG = 3;
    FG = 2;
    T = 5;
    L = 15;
    LW = 15;
    lines280_350(screen, cursor);
    SDL_RenderPresent(screen->ren);

    // 90 LET X=1:LET Y=1
    X = 1;
    Y = 1;

    // 100 LET I$=inkey$
    SDL_Event event;
    int done = 0;
    int text_entered;
    while (!done) {
        text_entered = 0;
        while (!done && !text_entered) {
            if (SDL_PollEvent(&event)) {
                switch (event.type) {
                   case SDL_QUIT:
                      done = SDL_TRUE;
                      break;
                   case SDL_TEXTINPUT:
                       I$ = event.text.text;
                       text_entered = 1;
                }
            }
        }
        if (text_entered) {
            // 110 IF I$="H" THEN GOSUB 360
            // 120 IF I$="A" AND Y>1 THEN LET Y=Y-1
            // 130 IF I$="Z" AND Y<15 THEN LET Y=Y+1
            // 140 IF I$="N" AND X>1 THEN X=X-1
            // 150 IF I$="M" AND X<15 THEN LET X=X+1
            // 160 IF I$>"/" AND I$<":" THEN GOSUB 230

            if (*I$ == 'h') {
                lines360_420(screen, cursor);
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
            paper(cursor, 3);
            ink(cursor, 0);
            // 180 PRINT tab(X,Y+5);CHR$(OS);
            tab(cursor, X, Y + 5);
            char os_input[2];
            sprintf(os_input, "%s", (char *) &OS);
            print_text(screen, cursor, os_input);
            tab(cursor, X, Y + 5);
            // 190 PRINT tab(cursor, X,Y+5);CHR$(R(X,Y));
            sprintf(os_input, "%s", (char *) &R[X][Y]);
            print_text(screen, cursor, os_input);
            SDL_RenderPresent(screen->ren);
            // 200 IF I$="S" AND IX>0 THEN GOSUB 450:GOTO 20
            if (*I$ == 's' && IX > 0) {
                lines450_600(screen, cursor);
            }
            // 210 IF I$<>"F" THEN GOTO 100
            if (*I$ == 'f') {
                done = SDL_TRUE;
            }
        }
    }
    // 220 STOP

    delete cursor;

    TTF_Quit();
    SDL_DestroyRenderer(screen->ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    // delete [] B$;
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

void print_left$_b$(screen_t *screen, cursor_t *cursor, int width);
void newline(cursor_t *cursor);

void lines280_350(screen_t *screen, cursor_t *cursor) {
    // 280 PRINT tab(0,T);
    tab(cursor, 0, T);
    // 290 paper FG:PRINT LEFT$(B$,LW+2)
    paper(cursor, FG);
    print_left$_b$(screen, cursor, LW + 2);
    newline(cursor);
    // 300 paper BG:ink FG
    paper(cursor, BG);
    ink(cursor, FG);
    // 310 FOR I=1 TO L
    for (int I = 1; I <= L; I +=1) {
    // 320 PRINT BG$(FG);" ";BG$(BG);LEFT$(B$,LW);BG$(FG);" "
        paper(cursor, FG);
        print_text(screen, cursor, " ");
        paper(cursor, BG);
        print_left$_b$(screen, cursor, LW);
        paper(cursor, FG);
        print_text(screen, cursor, " ");
        newline(cursor);
    // 330 NEXT I
    }
    // 340 paper FG:PRINT LEFT$(B$,LW+2);
    paper(cursor, FG);
    print_left$_b$(screen, cursor, LW + 2);
    SDL_RenderPresent(screen->ren);
    // 350 RETURN

}

const char * H$[10];
char *B$;

void lines360_420(screen_t *screen, cursor_t *cursor) {

    int H;
    // 360 paper 1:ink 3
    paper(cursor, 1);
    ink(cursor, 3);
    // 370 FOR H = 1 TO 10
    for (H = 0; H < 10; H += 1) {
    // 380 PRINT tab(1,4);H$(H);:GOSUB 430
        tab(cursor, 1, 4);
        print_text(screen, cursor, H$[H]);
        SDL_RenderPresent(screen->ren);
        lines430_440();
    // 390 PRINT tab(1,4);LEFT$(B$,W-2);
        tab(cursor, 1, 4);
        print_left$_b$(screen, cursor, W - 2);
        SDL_RenderPresent(screen->ren);
    // 400 NEXT H
    }
    // 410 ink 3
    ink(cursor, 3);
    // 420 RETURN
}

void lines430_440() {
    // 430 LET G$=inkey$:IF G$="" THEN GOTO 430
    // 440 RETURN

    int done = 0;
    SDL_Event event;
    while (!done) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
               case SDL_QUIT:
               case SDL_TEXTINPUT:
                  done = SDL_TRUE;
                  break;
            }
        }
    }
}

void lines450_600(screen_t *screen, cursor_t *cursor) {
    // 450 PRINT tab(1, 4);"ONE MOMENT PLEASE.";
    tab(cursor, 1, 4);
    print_text(screen, cursor, "ONE MOMENT PLEASE");
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
    tab(cursor, 1, 4);
    print_text(screen, cursor, "ANY KEY TO SAVE   ");
    SDL_RenderPresent(screen->ren);
    lines430_440();
    // 550 LET S=OPENOUT "LEVEL"
    FILE *S = fopen("LEVEL", "w");
    // 560 PRINT#S,S$
    int error = fputs(S$, S);
    if (error) {
        printf("Error %i writing the level!", error);
    }
    // 570 CLOSE#S
    error = fclose(S);
    if (error) {
        printf("Error %i saving the level!", error);
    }
    // 580 PRINT tab(1,4);LEFT(B$,W)
    tab(cursor, 1, 4);
    print_left$_b$(screen, cursor, W);
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
    B$ = new char[W + 1];
    for (int I=0; I<W; I++) {
        B$[I] = ' ';
    }
    B$[W] = 0;
    // 770 RETURN
}

void print_left$_b$(screen_t *screen, cursor_t *cursor, int width) {
    int error = SDL_SetRenderDrawColor(
        screen->ren,
        cursor->background_colour[0],
        cursor->background_colour[1],
        cursor->background_colour[2],
        cursor->background_colour[3]
    );
    if (error) {
        printf("SDL_SetRenderDrawColor error: %s\n", SDL_GetError());
    }
    const SDL_Rect rect = {
        .x = cursor->curs_x * 8 * screen->zoom,
        .y = cursor->curs_y * 8 * screen->zoom,
        .w = width * 8 * screen->zoom,
        .h = 8 * screen->zoom
    };
    cursor->curs_x += width;
    error = SDL_RenderFillRect(screen->ren, &rect);
    if (error) {
        printf("SDL_RenderFillRect!: %s\n", SDL_GetError());
    }
}

void newline(cursor_t *cursor) {
    cursor->curs_x = 0;
    cursor->curs_y += 1;
}

void lines790_800()
{
  // 790 OS=96:CO=OS+6:W=40:GOSUB 4000
  OS = 96;
  CO = OS + 6;
  W = 40;
  lines4000_4030();
  // 800 RETURN
}

void lines810_840()
{
  // 810 REM SET UP THE CHARACTERS
  // 820 FOR I=0 TO 7:READ A:POKE 36352+I, 255-A:NEXT I
  // 830 FOR I=0 TO 95:READ A:POKE 36400+I, 255-A:NEXT I
  // 840 RETURN
}

Uint8 BG$ [4][2][4];


void ink(cursor_t *cursor, int c_num) {
    cursor->foreground_colour[0] = BG$[c_num][1][0];
    cursor->foreground_colour[1] = BG$[c_num][1][1];
    cursor->foreground_colour[2] = BG$[c_num][1][2];
    cursor->foreground_colour[3] = BG$[c_num][1][3];
}

void paper(cursor_t *cursor, int c_num) {
    cursor->background_colour[0] = BG$[c_num][1][0];
    cursor->background_colour[1] = BG$[c_num][1][1];
    cursor->background_colour[2] = BG$[c_num][1][2];
    cursor->background_colour[3] = BG$[c_num][1][3];
}



// Sets up the print formatting. Will be replaced by ncurses commands.
void lines4000_4030()
{
    // 4000 BG$(0)=chr$(146):BG$(1)=CHR$(18)+CHR(28)
    // BG$(0) stops highlighting; Use attroff(A_REVERSE);
    // BG$(1) sets the type to highlighted red;
    BG$[1][0][0] = 0;
    BG$[1][0][1] = 0;
    BG$[1][0][2] = 0;
    BG$[1][0][3] = 0;
    BG$[1][1][0] = 0x88;
    BG$[1][1][1] = 0x20;
    BG$[1][1][2] = 0x00;
    BG$[1][1][3] = 0;
    // 4010 BG$(2)=CHR$(18)+CHR$(158):BG$(3)=CHR$(18)+CHR$(5)
    // BG$(2) sets the type to highlighted yellow
    BG$[2][0][0] = 0;
    BG$[2][0][1] = 0;
    BG$[2][0][2] = 0;
    BG$[2][0][3] = 0;
    BG$[2][1][0] = 0xf0;
    BG$[2][1][1] = 0xe8;
    BG$[2][1][2] = 0x58;
    BG$[2][1][3] = 0;

    // BG$(3) sets the type to highlighted white; Use attron(COLOR_PAIR(3));
    BG$[3][0][0] = 0;
    BG$[3][0][1] = 0;
    BG$[3][0][2] = 0;
    BG$[3][0][3] = 0;
    BG$[3][1][0] = 0xff;
    BG$[3][1][1] = 0xff;
    BG$[3][1][2] = 0xff;
    BG$[3][1][3] = 0;
    // 4020 HM$=CHR(19):CU$="":FOR I=1 TO W:LET CU$=CU$+CHR(17):NEXT I
    // HM$ returns to the top of the screen; Use move(0, 0);
    // CU$ is an array of up arrows; use getyx() and move(y - index, x);
    // 4030 POKE 650,255:RETURN
    // Repeat key turned on; This is the default
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
