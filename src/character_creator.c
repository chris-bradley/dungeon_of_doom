#include <SDL.h>
#include "dungeon_lib.h"

void lines570_600(screen_t *screen, int max_accepted_discount,
                  int character_class_id, int stage, int *gold_coins,
                  int selected_row, int item_num, int attrs_and_prices[5][9],
                  int * inventory, const char * character_class_names[5],
                  const char * point_label, char * message,
                  int item_batch_size[24],
                  const char * item_char_class_avail[25]);
void lines610_670(int max_accepted_discount, int stage, int *gold_coins,
                  int selected_row, int item_num, int offer, int Y,
                  int attrs_and_prices[5][9], int * inventory, char * message,
                  int item_batch_size[24]);
void lines680_710(int character_class_id, int item_num, int *Y,
                  const char * character_class_names[5], char * message,
                  const char * item_char_class_avail[25]);
void lines720_800(screen_t *screen, int interface_num_rows, int *selected_row,
                  int *selected_row_pos, int T, char * pressed_key);
void lines810_850(screen_t *screen, int stage, int num_points, int *T, int W,
                  const char * point_label, char * message,
                  const char * attr_item_and_stage_names[5][10]);
void lines860_890(screen_t *screen, int num_points, const char * point_label,
                  char * message);
void lines900_910(screen_t *screen, int stage, int *T, int W,
                  int attrs_and_prices[5][9],
                  const char * attr_item_and_stage_names[5][10]);
void lines920_970(screen_t *screen, int stage, int T,
                  int attrs_and_prices[5][9],
                  const char * attr_item_and_stage_names[5][10]);
void lines1060_1590(int *char_base, int *interface_num_rows, int *gold_coins,
                    int *attr_points, int *W, int attrs_and_prices[5][9],
                    int ** inventory, const char * character_class_names[5],
                    char ** message, int item_batch_size[24],
                    const char * item_char_class_avail[25],
                    const char * attr_item_and_stage_names[5][10]);
void lines1700_1730(screen_t *screen, int X, int Y, char ** typed_string);

int main(int argc, char *argv[]) {
    int char_base, max_accepted_discount, character_class_id,
        interface_num_rows, gold_coins, index, stage, selected_row,
        attr_points, item_num, num_item_types, offer, selected_row_pos, T, W,
        X, Y;
    int attrs_and_prices[5][9];
    int * inventory;
    const char * character_class_names[5], * point_label,
               * item_char_class_avail[25];
    char * pressed_key, * typed_string = NULL, * message = NULL,
         * character_name;
    int item_batch_size[24];
    const char * attr_item_and_stage_names[5][10];
    // 10 GOSUB 1060
    lines1060_1590(
        &char_base, &interface_num_rows, &gold_coins, &attr_points, &W,
        attrs_and_prices, &inventory, character_class_names, &message,
        item_batch_size, item_char_class_avail, attr_item_and_stage_names
    );
    // 20 paper 0:CLS
    screen_t *screen = NULL;
    if (init_screen(&screen) < 0) {
        return 1;
    }
    paper(screen->cursor, 0);
    // 30 LET J=1:LET H=MP:LET H$="POINTS"
    stage = 1;
    point_label = "POINTS";
    // 40 GOSUB 810:GOSUB900
    lines810_850(
        screen, stage, attr_points, &T, W, point_label, message,
        attr_item_and_stage_names
    );
    lines900_910(
        screen, stage, &T, W, attrs_and_prices, attr_item_and_stage_names
    );
    // 50 LET K=1:LET P=T+1
    selected_row = 1;
    selected_row_pos = T + 1;
    // 60 PRINT tab(1,P);">";
    tab(screen->cursor, 1, selected_row_pos);
    print_text(screen, ">");
    SDL_RenderPresent(screen->ren);
    // 70 GOSUB 720
    pressed_key = (char *) malloc(sizeof(char));
    if (pressed_key == NULL) {
        fprintf(stderr, "pressed_key is NULL!\n");
        exit(1);
    }
    do {
        lines720_800(
            screen, interface_num_rows, &selected_row, &selected_row_pos, T,
            pressed_key
        );
    // 80 IF K=5 THEN GOTO 70
        while (selected_row == 5) {
            lines720_800(
                screen, interface_num_rows, &selected_row, &selected_row_pos,
                T, pressed_key
            );
        }
    // 90 IF I$=";" AND H>0 THEN LET F(J,K)=F(J,K)+1:LET H=H-1:GOSUB 920
        if (*pressed_key == ';' && attr_points > 0) {
            attrs_and_prices[stage][selected_row] += 1;
            attr_points -= 1;
            lines920_970(
                screen, stage, T, attrs_and_prices, attr_item_and_stage_names
            );
        }
    // 100 IF I$="-" AND F(J,K)>1 THEN LET F(J,K)=F(J,K)-1:LET H=H+1:GOSUB 920
        if (*pressed_key == '-' && attrs_and_prices[stage][selected_row] > 1) {
            attrs_and_prices[stage][selected_row] -=1;
            attr_points += 1;
            lines920_970(
                screen, stage, T, attrs_and_prices, attr_item_and_stage_names
            );
        }
    // 110 LET C=1
        character_class_id = 1;
    // 120 IF F(1,4)>6 AND F(1,8)>7 THEN LET C=2
        if (attrs_and_prices[1][4] > 6 && attrs_and_prices[1][8] > 7) {
            character_class_id = 2;
        }
    // 130 IF F(1,4)>8 AND F(1,7)>7 THEN LET C=3
        if (attrs_and_prices[1][4] > 8 && attrs_and_prices[1][7] > 7) {
            character_class_id = 3;
        }
    // 140 IF F(1,1)>7 AND F(1,8)>5 AND F(1,1)+F(1,2)>10 THEN LET C=4
        if (
                attrs_and_prices[1][1] > 7 && attrs_and_prices[1][8] > 5 &&
                attrs_and_prices[1][1] + attrs_and_prices[1][2] > 10
        ) {
            character_class_id = 4;
        }
    // 150 IF F(1,1)>8 AND F(1,2)+F(1,3)>12 and F(1,8)<6 THEN LET C=5
        if (
                attrs_and_prices[1][1] > 8 &&
                attrs_and_prices[1][2] + attrs_and_prices[1][3] > 12 &&
                attrs_and_prices[1][8] < 6
        ) {
            character_class_id = 5;
        }
    // 160 LET M$=C$(C)
        strcpy(message, character_class_names[character_class_id]);
    // 170 GOSUB 860
        lines860_890(screen, attr_points, point_label, message);
        SDL_RenderPresent(screen->ren);
    // 180 IF I$<>" " THEN GOTO 70
    } while (*pressed_key != ' ');
    // 190 LET H=GC:LET H$="GOLD COINS:"
    point_label = "GOLD COINS";
    // 200 FOR J=2 TO 4
    for (stage = 2; stage <= 4; stage += 1) {
    // 210 LET K=1:LET P=T+1
        selected_row = 1;
        selected_row_pos = T + 1;
    // 220 LET M$="CHOOSE WELL SIRE!"
        strcpy(message, "CHOOSE WELL SIRE!");
    // 230 GOSUB 810
        lines810_850(
            screen, stage, gold_coins, &T, W, point_label, message,
            attr_item_and_stage_names
        );
    // 240 GOSUB 900
        lines900_910(
            screen, stage, &T, W, attrs_and_prices, attr_item_and_stage_names
        );
    // 250 PRINT tab(1,P);">";
        tab(screen->cursor, 1, selected_row_pos);
        print_text(screen, ">");
        SDL_RenderPresent(screen->ren);
    // 260 GOSUB 720
        do {
            lines720_800(
                screen, interface_num_rows, &selected_row, &selected_row_pos,
                T, pressed_key
            );
    // 270 LET N=8*(J-2)+K
            item_num = 8 * (stage - 2) + selected_row;
    // 280 LET M$="MAKE YOUR CHOICE"
            strcpy(message, "MAKE YOUR CHOICE");
    // 290 GOSUB 680
            lines680_710(
                character_class_id, item_num, &Y, character_class_names,
                message, item_char_class_avail
            );
    // 300 LET BR=0:LET OF=0
            max_accepted_discount = 0;
            offer = 0;
    // 310 IF I$=";" THEN LET OF=F(J,K):GOSUB 610
            if (*pressed_key == ';') {
                offer = attrs_and_prices[stage][selected_row];
                lines610_670(
                    max_accepted_discount, stage, &gold_coins, selected_row,
                    item_num, offer, Y, attrs_and_prices, inventory, message,
                    item_batch_size
                );
            }
    // 320 IF I$="-" THEN LET BR=rnd(3):GOSUB 570
            if (*pressed_key == '-') {
                max_accepted_discount = rand() % 3;
                lines570_600(
                    screen, max_accepted_discount, character_class_id, stage,
                    &gold_coins, selected_row, item_num, attrs_and_prices,
                    inventory, character_class_names, point_label, message,
                    item_batch_size, item_char_class_avail
                );
            }
    // 330 GOSUB 860
            lines860_890(screen, gold_coins, point_label, message);
    // 340 IF I$<>" " THEN GOTO 260
        } while (*pressed_key != ' ');
    // 350 NEXT J
    }
    character_name = (char *) malloc(sizeof(char) * 40);
    if (character_name == NULL) {
        fprintf(stderr, "character_name is NULL!\n");
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
        lines1700_1730(screen, X, Y, &typed_string);
        strcpy(character_name, typed_string);
        free(typed_string);
    // 390 IF LEN(N$)>10 THEN GOTO 360
    } while (strlen(character_name) > 10);
    // 400 PRINT tab(1,3);"ONE MOMENT PLEASE";
    tab(screen->cursor, 1, 3);
    print_text(screen, "ONE MOMENT PLEASE");
    SDL_RenderPresent(screen->ren);
    // 410 PRINT tab(1,3);
    tab(screen->cursor, 1, 3);
    // 420 LET O=D*3
    num_item_types = interface_num_rows * 3;
    // 430 LET S$=CHR$(O+AS)

    char * save_file_contents = (char *) malloc(
        sizeof(char) * (
            14 + num_item_types + strlen(character_name) + strlen(
                character_class_names[character_class_id]
            )
        )
    );
    if (save_file_contents == NULL) {
        fprintf(stderr, "save_file_contents is NULL!\n");
        exit(1);
    }

    save_file_contents[0] = (char) (num_item_types + char_base);
    // 440 FOR I=1 TO 8
    for (index = 1; index <= 8; index +=1 ) {
    // 450 LET S$=S$+CHR(F(1,I)+AS)
        save_file_contents[index] += (char) (
            attrs_and_prices[1][index] + char_base
        );
    }
    // 460 NEXT I
    // 470 FOR I = 1 TO O
    for (index = 1; index <= num_item_types; index += 1) {
    // 480 LET S$=S$+CHR$(O(I)+AS)
        save_file_contents[8 + index] = (char) (inventory[index] + char_base);
    // 490 NEXT I
    }
    // 500 LET S$=S$+CHR$(H+AS)
    save_file_contents[9 + num_item_types] = (char) (gold_coins + char_base);
    // 510 LET S$=S$+CHR$(AS)
    save_file_contents[10 + num_item_types] = (char) char_base;
    // 520 LET S$=S$+N$+" -"+C$(C)
    strcpy(save_file_contents + 11 + num_item_types, character_name);
    strcpy(
        save_file_contents + 11 + num_item_types + strlen(character_name),
        " -"
    );
    strcpy(
        save_file_contents + 13 + num_item_types + strlen(character_name),
        character_class_names[character_class_id]
    );
    save_file_contents[
        13 + num_item_types + strlen(character_name) + strlen(
            character_class_names[character_class_id]
        )
    ] = 0;
    // 530 LET S=OPENOUT "HERO"
    FILE *save_file_handle = fopen("HERO", "w");
    // 540 PRINT#S,S$
    int error = fputs(save_file_contents, save_file_handle);
    if (error) {
        fprintf(stderr, "Error %i writing the character!", error);
    }

    // 550 CLOSE#S
    error = fclose(save_file_handle);
    if (error) {
        fprintf(stderr, "Error %i saving the character!", error);
    }
    free(save_file_contents);
    // 560 STOP

    free(pressed_key);
    free(message);
    free(character_name);
    free(inventory);

    destroy_screen(screen);

    return 0;
}

void lines570_600(screen_t *screen, int max_accepted_discount,
                  int character_class_id, int stage, int *gold_coins,
                  int selected_row, int item_num, int attrs_and_prices[5][9],
                  int * inventory, const char * character_class_names[5],
                  const char * point_label, char * message,
                  int item_batch_size[24],
                  const char * item_char_class_avail[25]) {
    int offer, X, Y;
    char * typed_string = NULL;
    // 570 LET M$="";GOSUB 860
    strcpy(message, "");
    lines860_890(screen, *gold_coins, point_label, message);
    // 580 PRINT tab(2,2);"YOUR OFFER";
    tab(screen->cursor, 2, 2);
    print_text(screen, "YOUR OFFER");
    SDL_RenderPresent(screen->ren);
    // 590 INPUT OF
    // C64 Version: 590 X = 14:Y=2:GOSUB 1700:OF=VAL(IN$)
    X = 14;
    Y = 2;
    lines1700_1730(screen, X, Y, &typed_string);
    offer = atoi(typed_string);
    free(typed_string);
    // 600 GOSUB 680
    lines680_710(
        character_class_id, item_num, &Y, character_class_names, message,
        item_char_class_avail
    );
    lines610_670(
        max_accepted_discount, stage, gold_coins, selected_row, item_num,
        offer, Y, attrs_and_prices, inventory, message, item_batch_size
    );
}

void lines610_670(int max_accepted_discount, int stage, int *gold_coins,
                  int selected_row, int item_num, int offer, int Y,
                  int attrs_and_prices[5][9], int * inventory, char * message,
                  int item_batch_size[24]) {
    int price;
    // 610 IF O(N)>0 AND N<23 THEN LET M$="YOU HAVE IT SIRE":RETURN
    if (inventory[item_num] > 0 && item_num < 23) {
        strcpy(message, "YOU HAVE IT SIRE");
    } else {
    // 620 LET PR=F(J,K)-BR
        price = attrs_and_prices[stage][selected_row] - max_accepted_discount;
    // 630 IF H<PR THEN LET M$="YOU CANNOT AFFORD":RETURN
        if (*gold_coins < price) {
            strcpy(message, "YOU CANNOT AFFORD");
        } else {
    // 640 IF OF>=PR AND Y=1 THEN LET O(N)=O(N)+P(N):LET H+H-PR:LET M$="TIS YOURS!"
            if (offer >= price && Y == 1) {
                inventory[item_num] += item_batch_size[item_num];
                *gold_coins -= price;
                strcpy(message, "TIS YOURS!");
            }
    // 650 IF OF<PR AND Y=1 THEN LET M$="OFFER REJECTED";
            if (offer < price && Y == 1) {
                strcpy(message, "OFFER REJECTED");
            }
    // 660 IF H<0 THEN LET H=0
            if (*gold_coins < 0) {
                *gold_coins = 0;
            }
        }
    }
    // 670 RETURN
}


void lines680_710(int character_class_id, int item_num, int *Y,
                  const char * character_class_names[5], char * message,
                  const char * item_char_class_avail[25]) {
    // 680 LET Y=0
    *Y = 0;
    // 690 IF MID$(O$(N),C,1)="1" THEN LET Y=1
    if (item_char_class_avail[item_num][character_class_id] == '1') {
        *Y = 1;
    }
    // 700 IF Y=0 THEN LET M$="NOT FOR "+C$(C)
    if (*Y == 0) {
        sprintf(
            message,
            "NOT FOR %s",
            character_class_names[character_class_id]
        );
    }
    // 710 RETURN
}

void lines720_800(screen_t *screen, int interface_num_rows, int *selected_row,
                  int *selected_row_pos, int T, char * pressed_key) {
    // 720 LET I$=inkey$;
    // 730 IF I$="" THEN GOTO 720
    *pressed_key = inkey$();
    // 740 paper 3:ink 1
    paper(screen->cursor, 3);
    ink(screen->cursor, 1);
    // 750 print tab(1,P);" ";
    tab(screen->cursor, 1, *selected_row_pos);
    print_text(screen, " ");
    // 760 IF I$="A" AND K>1 THEN LET K=K-1
    if (*pressed_key == 'a' && *selected_row > 1) {
        *selected_row -= 1;
    }
    // 770 IF I$="Z" AND K<D THEN LET K=K+1
    else if (*pressed_key == 'z' && *selected_row < interface_num_rows) {
        *selected_row += 1;
    }
    // 780 LET P=K*2+T-1
    *selected_row_pos = *selected_row * 2 + T - 1;
    // 790 PRINT tab(1,P);">";
    tab(screen->cursor, 1, *selected_row_pos);
    print_text(screen, ">");
    // 800 RETURN
}

void lines980_1050(screen_t *screen, int W, int background_colour,
                   int border_colour, int T, int rows);

void lines810_850(screen_t *screen, int stage, int num_points, int *T, int W,
                  const char * point_label, char * message,
                  const char * attr_item_and_stage_names[5][10]) {
    int background_colour, border_colour, rows;
    // 810 paper 0:ink 2
    paper(screen->cursor, 0);
    ink(screen->cursor, 2);
    // 820 PRINT tab(0,0);LEFT$(B$,W);
    tab(screen->cursor, 0, 0);
    print_left$_b$(screen, W);
    // 830 PRINT tab(0,0);F$(J,9)
    tab(screen->cursor, 0, 0);
    print_text(screen, attr_item_and_stage_names[stage][9]);
    // 840 LET BG=2:LET FG=3:LET T=1:LET L=2
    background_colour = 2;
    border_colour = 3;
    *T = 1;
    rows = 2;
    // 850 GOSUB 980
    lines980_1050(screen, W, background_colour, border_colour, *T, rows);
    lines860_890(screen, num_points, point_label, message);
}

void lines860_890(screen_t *screen, int num_points, const char * point_label,
                  char * message) {
    // 860 paper 2:ink 0
    paper(screen->cursor, 2);
    ink(screen->cursor, 0);
    // 870 PRINT tab(2,2);LEFT$(B$,17);tab(2,2);M$;
    tab(screen->cursor, 2, 2);
    print_left$_b$(screen, 17);
    tab(screen->cursor, 2, 2);
    print_text(screen, message);
    // C64: 875 PRINT HM$;LEFT$(CU$,3);SPC(15);LEFT$(B$,4);
    tab(screen->cursor, 15, 3);
    print_left$_b$(screen, 4);
    // 880 PRINT tab(2,3);H$;tab(15,3);H;" ";
    tab(screen->cursor, 2, 3);
    print_text(screen, point_label);
    tab(screen->cursor, 15, 3);
    char * outstring = (char *) malloc(sizeof(char) * 40);
    if (outstring == NULL) {
        fprintf(stderr, "outstring is NULL!\n");
        exit(1);
    }
    sprintf(outstring, "%i ", num_points);
    print_text(screen, outstring);
    free(outstring);
    // 890 RETURN
}

void lines900_910(screen_t *screen, int stage, int *T, int W,
                  int attrs_and_prices[5][9],
                  const char * attr_item_and_stage_names[5][10]) {
    int background_colour, border_colour, rows;
    // 900 LET BG=3:LET FG=2:LET T=5:LET L=15
    background_colour = 3;
    border_colour = 2;
    *T = 5;
    rows = 15;
    // 910 GOSUB 980
    lines980_1050(screen, W, background_colour, border_colour, *T, rows);
    lines920_970(
        screen, stage, *T, attrs_and_prices, attr_item_and_stage_names
    );
}

void lines920_970(screen_t *screen, int stage, int T,
                  int attrs_and_prices[5][9],
                  const char * attr_item_and_stage_names[5][10]) {
    int index, Y;
    // 920 paper 3:ink 0
    paper(screen->cursor, 3);
    ink(screen->cursor, 0);
    // 930 FOR I=1 TO 8
    for (index = 1; index <= 8; index += 1) {
    // 940 LET Y=T+(I-1)*2+1
        Y = T + (index - 1) * 2 + 1;
    // C64: 945 PRINT HM$;LEFT(CU$,Y);SPC(15);LEFT(B$,5);
        tab(screen->cursor, 15, Y);
        print_left$_b$(screen, 5);
    // 950 PRINT tab(2,Y);F$(J,I);tab(16,Y);F(J,I);" ";

        tab(screen->cursor, 2, Y);
        print_text(screen, attr_item_and_stage_names[stage][index]);
        tab(screen->cursor, 16, Y);
        char * outstring = (char *) malloc(sizeof(char) * 40);
        if (outstring == NULL) {
            fprintf(stderr, "outstring is NULL!\n");
            exit(1);
        }
        sprintf(outstring, "%i ", attrs_and_prices[stage][index]);
        print_text(screen, outstring);
        free(outstring);
    // 960 NEXT I
    }
    // 970 RETURN
}

void lines980_1050(screen_t *screen, int W, int background_colour,
                   int border_colour, int T, int rows) {
    int index;
    // 980 PRINT tab(0,T);
    tab(screen->cursor, 0, T);
    // 990 paper FG:PRINT LEFT$(B$,W);
    paper(screen->cursor, border_colour);
    print_left$_b$(screen, W);
    newline(screen->cursor);
    // 1000 paper BG:ink FG
    paper(screen->cursor, background_colour);
    ink(screen->cursor, border_colour);
    // 1010 FOR I=1 TO L
    for (index = 1; index <= rows; index += 1) {
    // 1020 PRINT CHR$(B);LEFT$(B$,W-2);CHR$(B);
    // C64: 1020 PRINT BG$(FG);" ";BG$(BG);LEFT$(B$,W-2);BG$(FG);" ";
        paper(screen->cursor, border_colour);
        print_text(screen, " ");
        paper(screen->cursor, background_colour);
        print_left$_b$(screen, W - 2);
        paper(screen->cursor, border_colour);
        print_text(screen, " ");
        // print_text() doesn't support wrapping yet, so we do our own newline:
        newline(screen->cursor);
    // 1030 NEXT I
    }
    // 1040 paper FG:PRINT LEFT$(B$,W);
    paper(screen->cursor, border_colour);
    print_left$_b$(screen, W);
    // 1050 RETURN
}

void lines1600_1650(int *W);

void lines1060_1590(int *char_base, int *interface_num_rows, int *gold_coins,
                    int *attr_points, int *W, int attrs_and_prices[5][9],
                    int ** inventory, const char * character_class_names[5],
                    char ** message, int item_batch_size[24],
                    const char * item_char_class_avail[25],
                    const char * attr_item_and_stage_names[5][10]) {
    int index;
    // 1060 GOSUB 1600
    lines1600_1650(W);
    // 1070 LET D=8
    *interface_num_rows = 8;
    // 1080 DIM F(4,D+1)
    // 1090 DIM F$(4,D+1)
    // 1100 DIM C$(5)
    // 1110 DIM O(D*3)
    *inventory = (int *) malloc(sizeof(int) * (*interface_num_rows) * 3);
    if (*inventory == NULL) {
        fprintf(stderr, "*inventory is NULL!\n");
        exit(1);
    }

    int i;
    for (i = 0; i < *interface_num_rows * 3; i += 1) {
        (*inventory)[i] = 0;
    }

    // 1120 DIM O$(D*3)
    // 1130 DIM P(D*3)
    // 1140 DATA "00001","00011","10011","10011","10011","00011","11111","10011"
    // 1150 DATA "00011","00011","10011","11111","00011","11011","11011","11111"
    // 1160 DATA "11100","00100","11100","10100","11100","11100","11111","11111"
    // 1170 FOR I=1 TO D*3
    // 1180 READ O$(I)
    // 1190 NEXT I
    item_char_class_avail[1] = "00001";
    item_char_class_avail[2] = "00011";
    item_char_class_avail[3] = "10011";
    item_char_class_avail[4] = "10011";
    item_char_class_avail[5] = "10011";
    item_char_class_avail[6] = "00011";
    item_char_class_avail[7] = "11111";
    item_char_class_avail[8] = "10011";
    item_char_class_avail[9] = "00011";
    item_char_class_avail[10] = "00011";
    item_char_class_avail[11] = "10011";
    item_char_class_avail[12] = "11111";
    item_char_class_avail[13] = "00011";
    item_char_class_avail[14] = "11011";
    item_char_class_avail[15] = "11011";
    item_char_class_avail[16] = "11111";
    item_char_class_avail[17] = "11100";
    item_char_class_avail[18] = "00100";
    item_char_class_avail[19] = "11100";
    item_char_class_avail[20] = "10100";
    item_char_class_avail[21] = "11100";
    item_char_class_avail[22] = "11100";
    item_char_class_avail[23] = "11111";
    item_char_class_avail[24] = "11111";
    // 1200 FOR I=1 TO 8
    for (index = 1; index <= 8; index += 1) {
    // 1210 LET F(1,I)=rnd(5)+2
        attrs_and_prices[1][index] = (rand() % 5) + 2;
    // 1220 NEXT I
    }
    // 1230 LET F(1,5)=1
    attrs_and_prices[1][5] = 1;
    // 1240 DATA 20,16,12,15,8,10,8,6
    // 1250 DATA 18,15,9,9,14,8,6,6
    // 1260 DATA 20,15,14,12,10,8,6,6
    // 1270 FOR J=2 TO 4
    // 1280 FOR I=1 TO 8
    // 1290 READ F(J,I)
    // 1300 NEXT I
    // 1310 NEXT J
    attrs_and_prices[2][1] = 20;
    attrs_and_prices[2][2] = 16;
    attrs_and_prices[2][3] = 12;
    attrs_and_prices[2][4] = 15;
    attrs_and_prices[2][5] = 8;
    attrs_and_prices[2][6] = 10;
    attrs_and_prices[2][7] = 8;
    attrs_and_prices[2][8] = 6;
    attrs_and_prices[3][1] = 18;
    attrs_and_prices[3][2] = 15;
    attrs_and_prices[3][3] = 9;
    attrs_and_prices[3][4] = 9;
    attrs_and_prices[3][5] = 14;
    attrs_and_prices[3][6] = 8;
    attrs_and_prices[3][7] = 6;
    attrs_and_prices[3][8] = 6;
    attrs_and_prices[4][1] = 20;
    attrs_and_prices[4][2] = 15;
    attrs_and_prices[4][3] = 14;
    attrs_and_prices[4][4] = 12;
    attrs_and_prices[4][5] = 10;
    attrs_and_prices[4][6] = 8;
    attrs_and_prices[4][7] = 6;
    attrs_and_prices[4][8] = 6;
    // 1320 DATA 5,4,3,3,2,2,1,1
    // 1330 DATA 5,4,3,1,2,1,3,1
    // 1340 DATA 4,3,2,2,3,1,1,1
    // 1350 FOR I=1 TO D*3
    // 1360 READ P(I)
    // 1370 NEXT I
    item_batch_size[1] = 5;
    item_batch_size[2] = 4;
    item_batch_size[3] = 3;
    item_batch_size[4] = 3;
    item_batch_size[5] = 2;
    item_batch_size[6] = 2;
    item_batch_size[7] = 1;
    item_batch_size[8] = 1;
    item_batch_size[9] = 5;
    item_batch_size[10] = 4;
    item_batch_size[11] = 3;
    item_batch_size[12] = 1;
    item_batch_size[13] = 2;
    item_batch_size[14] = 1;
    item_batch_size[15] = 3;
    item_batch_size[16] = 1;
    item_batch_size[17] = 4;
    item_batch_size[18] = 3;
    item_batch_size[19] = 2;
    item_batch_size[20] = 2;
    item_batch_size[21] = 3;
    item_batch_size[22] = 1;
    item_batch_size[23] = 1;
    item_batch_size[24] = 1;

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
    attr_item_and_stage_names[1][1] = "STRENGTH";
    attr_item_and_stage_names[1][2] = "VITALITY";
    attr_item_and_stage_names[1][3] = "AGILITY";
    attr_item_and_stage_names[1][4] = "INTELLIGENCE";
    attr_item_and_stage_names[1][5] = "EXPERIENCE";
    attr_item_and_stage_names[1][6] = "LUCK";
    attr_item_and_stage_names[1][7] = "AURA";
    attr_item_and_stage_names[1][8] = "MORALITY";
    attr_item_and_stage_names[1][9] = "CHARACTER CREATION";
    attr_item_and_stage_names[2][1] = "2 HAND SWORD";
    attr_item_and_stage_names[2][2] = "BROADSWORD";
    attr_item_and_stage_names[2][3] = "SHORTSWORD";
    attr_item_and_stage_names[2][4] = "AXE";
    attr_item_and_stage_names[2][5] = "MACE";
    attr_item_and_stage_names[2][6] = "FLAIL";
    attr_item_and_stage_names[2][7] = "DAGGER";
    attr_item_and_stage_names[2][8] = "GAUNTLET";
    attr_item_and_stage_names[2][9] = "ARMOURY";
    attr_item_and_stage_names[3][1] = "HEAVY ARMOUR";
    attr_item_and_stage_names[3][2] = "CHAIN ARMOUR";
    attr_item_and_stage_names[3][3] = "LEATHER ARMOUR";
    attr_item_and_stage_names[3][4] = "HEAVY ROBE";
    attr_item_and_stage_names[3][5] = "GOLD HELMET";
    attr_item_and_stage_names[3][6] = "HEADPIECE";
    attr_item_and_stage_names[3][7] = "SHIELD";
    attr_item_and_stage_names[3][8] = "TORCH";
    attr_item_and_stage_names[3][9] = "ACCOUTREMENTS";
    attr_item_and_stage_names[4][1] = "NECRONOMICON";
    attr_item_and_stage_names[4][2] = "SCROLLS";
    attr_item_and_stage_names[4][3] = "RING";
    attr_item_and_stage_names[4][4] = "MYSTIC AMULET";
    attr_item_and_stage_names[4][5] = "SASH";
    attr_item_and_stage_names[4][6] = "CLOAK";
    attr_item_and_stage_names[4][7] = "HEALING SALVE";
    attr_item_and_stage_names[4][8] = "POTIONS";
    attr_item_and_stage_names[4][9] = "EMPORIUM";
    // 1510 DATA "WANDERER","CLERIC","MAGE","WARRIOR","BARBARIAN"
    // 1520 FOR I=1 TO 5
    // 1530 READ C$(I)
    // 1540 NEXT I
    character_class_names[1] = "WANDERER";
    character_class_names[2] = "CLERIC";
    character_class_names[3] = "MAGE";
    character_class_names[4] = "WARRIOR";
    character_class_names[5] = "BARBARIAN";
    // 1550 LET MP=3+rnd(5)
    *attr_points = 3 + (rand() % 5);
    // 1560 LET GC=120+rnd(60)
    *gold_coins = 120 + (rand() % 60);
    // 1570 LET M$="":LET AS=65
    *message = (char *) malloc(sizeof(char) * 40);
    if (*message == NULL) {
        fprintf(stderr, "message is NULL!\n");
        exit(1);
    }
    strcpy(*message, "");
    *char_base = 65;
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

void lines1700_1730(screen_t *screen, int X, int Y, char ** typed_string) {
    // 1700 IN$=""
    int ind = 0;
    char * pressed_key = (char *) malloc(sizeof(char));
    *typed_string = (char *) malloc(sizeof(char) * 40);
    if (*typed_string == NULL) {
        fprintf(stderr, "*typed_string is NULL!\n");
        exit(1);
    }
    (*typed_string)[0] = 0;
    // 1710 GET I$:IF I$=CHR$(13) THEN RETURN
    SDL_Event event;
    int done = 0;
    int text_entered;
    while (!done) {
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
                    *pressed_key = event.text.text[0];
                    text_entered = 1;
            }
        }
    // 1720 IF I$>"/" AND I$<"[" THEN LET IN$=IN$+I$:PRINT HM$;LEFT$(CU$,Y);SPC(X);IN$;
        if (
                text_entered && *pressed_key > '/' && *pressed_key < ']' &&
                ind < 39
        ) {
            (*typed_string)[ind] = *pressed_key;
            ind += 1;
            (*typed_string)[ind] = 0;
            tab(screen->cursor, X, Y);
            print_text(screen, *typed_string);
            SDL_RenderPresent(screen->ren);
        }
    // 1730 GOTO 1710
    }
    (*typed_string)[ind] = 0;
    free(pressed_key);
}
