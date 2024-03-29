#include <ft2build.h>
#include FT_FREETYPE_H

// TODO: Investigate if the name is valid. The '_t' suffix is common but may
// not adhere to the POSIX standards.
typedef struct {
    int curs_x;
    int curs_y;
    uint8_t foreground_colour[4];
    uint8_t background_colour[4];
} cursor_t;

typedef struct {
    SDL_Renderer * ren;
    SDL_Window * win;
    FT_Library * ft_lib;
    cursor_t * cursor;
    int zoom;
} screen_t;

enum ColourNum {
    BLACK = 0,
    RED = 1,
    YELLOW = 2,
    WHITE = 3
};

enum CharCode {
    CHARACTER_BASE = 65,
    BLANK = 6,
    WALL = 7,
    VASE = 8,
    CHEST = 9,
    IDOL = 10,
    EXIT = 12,
    TRAP = 13,
    SAFE_PLACE = 14
};

SDL_Rect * print_text(screen_t * screen, const char * message);

void ink(cursor_t * cursor, enum ColourNum c_num);

void paper(cursor_t * cursor, enum ColourNum c_num);

void tab(cursor_t * cursor, int x, int y);

void newline(cursor_t * cursor);

screen_t * init_screen();
void destroy_screen(screen_t * screen);
void clear_screen(screen_t * screen);

char inkey$();

void draw_box(screen_t * screen, int top_row, int left_col, int rows, int cols,
              enum ColourNum colour);

void draw_bordered_box(screen_t * screen, int top_row, int left_col, int rows,
                       int cols, enum ColourNum background_colour,
                       enum ColourNum border_colour);

void clear_rect(screen_t * screen, SDL_Rect * rect, enum ColourNum colour);

void render_bitmap(screen_t * screen, int col, int row, int bitmap_num,
                   enum ColourNum foreground_colour,
                   enum ColourNum background_colour);

#ifdef linux
#define PATHSEP '/'
#endif

#ifdef WIN32
#define PATHSEP '\\'
#endif

char * get_character_dir();
char * get_level_dir();

void handle_args(int argc, char * argv[]);
