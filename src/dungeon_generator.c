#include <SDL.h>
#include "dungeon_lib.h"

void place_item(int cur_coord_x, int cur_coord_y, int contents[16][16],
                char *pressed_key, int *entrance_coord_x,
                int *entrance_coord_y, int char_code_blank);
void draw_help(screen_t *screen, int screen_cols, const char * help_lines[10]);
void wait_for_user_key_press();
void save_level(screen_t *screen, int screen_cols, int *level_num,
                int char_base, int contents[16][16], int *entrance_coord_x,
                int *entrance_coord_y, int char_code_blank);
void init_vars(int *screen_cols, int *level_num, int *char_base,
               int contents[16][16], int *entrance_coord_x,
               int *entrance_coord_y, int *char_code_blank,
               const char * help_lines[10]);
void lines700_770(int contents[16][16], int *entrance_coord_x,
                  int *entrance_coord_y, int char_code_blank);
void lines790_800(int *char_base, int *screen_cols, int *char_code_blank);
void lines810_840();
void lines5000_5080();

int main(int argc, char *argv[]) {
    int char_code_blank, entrance_coord_x, entrance_coord_y, level_num,
        char_base, screen_cols, cur_coord_x, cur_coord_y;
    int contents[16][16];
    const char * help_lines[10];
    char *pressed_key;
    // 5 GOSUB 5000
    lines5000_5080();
    // GOSUB 610

    init_vars(
        &screen_cols, &level_num, &char_base, contents, &entrance_coord_x,
        &entrance_coord_y, &char_code_blank, help_lines
    );
    // Clear screen; Black background.
    // 20 PRINT CHR$(147): POKE 53280,0:POKE 53281,0
    screen_t *screen = NULL;
    if (init_screen(&screen) < 0) {
        return 1;
    }
    // 30 LET BG=2:LET FG=1:LET T=0:LET L=3:LET LW=W-3:GOSUB 280
    draw_bordered_box(screen, 0, 0, 3, screen_cols - 3, YELLOW, RED);
    // 40 paper 2:ink 0
    paper(screen->cursor, YELLOW);
    ink(screen->cursor, BLACK);
    // 50 PRINT tab(1,1);"LEVEL GENERATOR"
    tab(screen->cursor, 1, 1);
    free(print_text(screen, "LEVEL GENERATOR"));
    // 60 PRINT tab(1,2);"THIS IS LEVEL:";LE;
    tab(screen->cursor, 1, 2);
    char* outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
       fprintf(stdout, "Allocating outstring failed!");
       exit(1);
    }
    snprintf(outstring, 40, "THIS IS LEVEL: %i", level_num);
    free(print_text(screen, outstring));
    free(outstring);
    // 70 PRINT tab(1,3);"PRESS H FOR HELP"
    tab(screen->cursor, 1, 3);
    free(print_text(screen, "PRESS H FOR HELP"));

    // 80 LET BG=3:LET FG=2:LET T=5:LET L=15:LET LW=15:GOSUB 280
    draw_bordered_box(screen, 5, 0, 15, 15, WHITE, YELLOW);
    SDL_RenderPresent(screen->ren);

    // 90 LET X=1:LET Y=1
    cur_coord_x = 1;
    cur_coord_y = 1;

    // 100 LET I$=inkey$
    pressed_key = (char *) malloc(sizeof(char));
    int done = 0;
    while (!done) {
        *pressed_key = inkey$();
        // 110 IF I$="H" THEN GOSUB 360
        // 120 IF I$="A" AND Y>1 THEN LET Y=Y-1
        // 130 IF I$="Z" AND Y<15 THEN LET Y=Y+1
        // 140 IF I$="N" AND X>1 THEN X=X-1
        // 150 IF I$="M" AND X<15 THEN LET X=X+1
        // 160 IF I$>"/" AND I$<":" THEN GOSUB 230

        if (*pressed_key == 'h') {
            draw_help(screen, screen_cols, help_lines);
        } else if (*pressed_key == 'a' && cur_coord_y > 1) {
            cur_coord_y -= 1;
        } else if (*pressed_key == 'z' && cur_coord_y < 15) {
            cur_coord_y += 1;
        } else if (*pressed_key == 'n' && cur_coord_x > 1) {
            cur_coord_x -= 1;
        } else if (*pressed_key == 'm' && cur_coord_x < 15) {
            cur_coord_x += 1;
        } else if (*pressed_key > '/' && *pressed_key < ':') {
            place_item(
                cur_coord_x, cur_coord_y, contents, pressed_key,
                &entrance_coord_x, &entrance_coord_y, char_code_blank
            );
        }
        // 170 paper 3:ink 0
        paper(screen->cursor, WHITE);
        ink(screen->cursor, BLACK);
        // 180 PRINT tab(X,Y+5);CHR$(OS);
        tab(screen->cursor, cur_coord_x, cur_coord_y + 5);
        char os_input[2];
        sprintf(os_input, "%s", (char *) &char_base);
        free(print_text(screen, os_input));
        tab(screen->cursor, cur_coord_x, cur_coord_y + 5);
        // 190 PRINT tab(X,Y+5);CHR$(R(X,Y));
        sprintf(os_input, "%s", (char *) &contents[cur_coord_x][cur_coord_y]);
        free(print_text(screen, os_input));
        SDL_RenderPresent(screen->ren);
        // 200 IF I$="S" AND IX>0 THEN GOSUB 450:GOTO 20
        if (*pressed_key == 's' && entrance_coord_x > 0) {
            save_level(
                screen, screen_cols, &level_num, char_base, contents,
                &entrance_coord_x, &entrance_coord_y, char_code_blank
            );
        }
        // 210 IF I$<>"F" THEN GOTO 100
        if (*pressed_key == 'f') {
            done = SDL_TRUE;
        }
    }
    // 220 STOP
    free(pressed_key);
    destroy_screen(screen);

    return 0;
}

void place_item(int cur_coord_x, int cur_coord_y, int contents[16][16],
                char *pressed_key, int *entrance_coord_x,
                int *entrance_coord_y, int char_code_blank) {
    // 230 LET I=VAL(I$)
    int pressed_key_num = atoi(pressed_key);
    // 240 IF I=9 THEN LET I=8+rnd(3)
    if (pressed_key_num == 9) {
        pressed_key_num = 9 + (rand() % 3);
    }
    // 250 IF I=5 THEN LET IX=X:LET IY=Y
    else if (pressed_key_num == 5) {
       *entrance_coord_x = cur_coord_x;
       *entrance_coord_y = cur_coord_y;
    }
    // 260 LET R(X,Y)=CO+I
    contents[cur_coord_x][cur_coord_y] = char_code_blank + pressed_key_num;
    // 270 RETURN
}


void draw_help(screen_t *screen, int screen_cols,
               const char * help_lines[10]) {

    int index;
    // 360 paper 1:ink 3
    paper(screen->cursor, RED);
    ink(screen->cursor, WHITE);
    // 370 FOR H = 1 TO 10
    for (index = 0; index < 10; index += 1) {
    // 380 PRINT tab(1,4);H$(H);:GOSUB 430
        tab(screen->cursor, 1, 4);
        free(print_text(screen, help_lines[index]));
        SDL_RenderPresent(screen->ren);
        wait_for_user_key_press();
    // 390 PRINT tab(1,4);LEFT$(B$,W-2);
        tab(screen->cursor, 1, 4);
        print_left$_b$(screen, screen_cols - 2);
        SDL_RenderPresent(screen->ren);
    // 400 NEXT H
    }
    // 410 ink 3
    ink(screen->cursor, WHITE);
    // 420 RETURN
}

void wait_for_user_key_press() {
    // 430 LET G$=inkey$:IF G$="" THEN GOTO 430
    // 440 RETURN
    inkey$();
}

void save_level(screen_t *screen, int screen_cols, int *level_num,
                int char_base, int contents[16][16], int *entrance_coord_x,
                int *entrance_coord_y, int char_code_blank) {
    // 450 PRINT tab(1, 4);"ONE MOMENT PLEASE.";
    tab(screen->cursor, 1, 4);
    free(print_text(screen, "ONE MOMENT PLEASE"));
    // 460 LET S$=""
    char save_file_contents[239];
    // 470 FOR J=1 TO 15
    int coord_x, coord_y;
    for (coord_x = 1; coord_x <= 15; coord_x += 1) {
    // 480 FOR K=1 TO 15
        for (coord_y = 1; coord_y <= 15; coord_y += 1) {
    // 490 LET S$=S$+CHR$(R(K,J))
            save_file_contents[(coord_x - 1) * 15 + coord_y - 1] =
                (char) contents[coord_x][coord_y];

    // 500 NEXT K
        }
    // 510 NEXT J
    }
    // 520 LET S$=S$+CHR$(IX+OS):LET S$=S$+CHR(IY+OS)
    save_file_contents[225] = (char) *entrance_coord_x + char_base;
    save_file_contents[226] = (char) *entrance_coord_y + char_base;
    // 530 LET S$=S$+CHR$(LE+OS)
    save_file_contents[227] = (char) *level_num + char_base;
    save_file_contents[228] = 0;
    // 540 PRINT tab(1,4);"ANY KEY TO SAVE   ";GOSUB 430
    tab(screen->cursor, 1, 4);
    free(print_text(screen, "ANY KEY TO SAVE   "));
    SDL_RenderPresent(screen->ren);
    wait_for_user_key_press();
    // 550 LET S=OPENOUT "LEVEL"
    FILE *save_file_handle = fopen("LEVEL", "w");
    // 560 PRINT#S,S$
    int error = fputs(save_file_contents, save_file_handle);
    if (error) {
        fprintf(stderr, "Error %i writing the level!", error);
    }
    // 570 CLOSE#S
    error = fclose(save_file_handle);
    if (error) {
        fprintf(stderr, "Error %i saving the level!", error);
    }
    // 580 PRINT tab(1,4);LEFT(B$,W)
    tab(screen->cursor, 1, 4);
    print_left$_b$(screen, screen_cols);
    SDL_RenderPresent(screen->ren);
    // 590 LET LE=LE+1:GOSUB 700
    *level_num = *level_num + 1;
    lines700_770(
        contents, entrance_coord_x, entrance_coord_y, char_code_blank
    );
    // 600 RETURN
}

void init_vars(int *screen_cols, int *level_num, int *char_base,
               int contents[16][16], int *entrance_coord_x,
               int *entrance_coord_y, int *char_code_blank,
               const char * help_lines[10]) {
    // 610 DIM R(15,15),H$(10)
    // 620 GOSUB 790
    lines790_800(char_base, screen_cols, char_code_blank);
    // 630 DATA "PRESS ANY KEY","TO MOVE A Z N M","1 WALL    2 VASE"
    // 640 DATA "3 CHEST 4 * idol *","5 WAY IN  6 EXIT","7 TRAP", "8 SAFE PLACE"
    // 650 DATA "9 GUARD","0 TO ERASE","S TO SAVE"
    // 660 LET LE = 1
    *level_num = 1;
    // 670 FOR I=1 to 10
    // 680 READ H$(I)
    help_lines[0] = "PRESS ANY KEY     ";
    help_lines[1] = "TO MOVE A Z N M   ";
    help_lines[2] = "1 WALL    2 VASE  ";
    help_lines[3] = "3 CHEST 4 * IDOL *";
    help_lines[4] = "5 WAY IN  6 EXIT  ";
    help_lines[5] = "7 TRAP            ";
    help_lines[6] = "8 SAFE PLACE      ";
    help_lines[7] = "9 GUARD           ";
    help_lines[8] = "0 TO ERASE        ";
    help_lines[9] = "S TO SAVE         ";

    // 690 NEXT I:GOSUB 810
    lines810_840();
    lines700_770(
        contents, entrance_coord_x, entrance_coord_y, *char_code_blank
    );
}

void lines700_770(int contents[16][16], int *entrance_coord_x,
                  int *entrance_coord_y, int char_code_blank) {
    // 700 FOR J=1 to 15
    // 710 FOR K=1 to 15
    // 720 LET R(J,K) = CO
    // 730 NEXT K
    // 740 NEXT J
    for (int coord_x=0; coord_x<16; coord_x++) {
        for (int coord_y=0; coord_y<16; coord_y++) {
            contents[coord_x][coord_y] = char_code_blank;
        }
    }
    // 750 LET IX=0:LET IY=0
    *entrance_coord_x = 0;
    *entrance_coord_y = 0;
    // 760 LET B$="":FOR I = 1 TO W:LET B$=B$+" ":NEXT I
    // dungeon_libs' print_left$_b$() removes the need for B$
    // 770 RETURN
}

void lines790_800(int *char_base, int *screen_cols, int *char_code_blank) {
  // 790 OS=96:CO=OS+6:W=40:GOSUB 4000
  *char_base = 96;
  *char_code_blank = *char_base + 6;
  *screen_cols = 40;
  // 800 RETURN
}

void lines810_840() {
  // 810 REM SET UP THE CHARACTERS
  // 820 FOR I=0 TO 7:READ A:POKE 36352+I, 255-A:NEXT I
  // 830 FOR I=0 TO 95:READ A:POKE 36400+I, 255-A:NEXT I
  // 840 RETURN
}

// Graphic Memory setup stuff

void lines5000_5080() {
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
