// TODO: Investigate if the name is valid. The '_t' suffix is common but may
// not adhere to the POSIX standards.
typedef struct {
    int curs_x;
    int curs_y;
    uint8_t foreground_colour [4];
    uint8_t background_colour [4];
} cursor_t;

typedef struct {
    SDL_Renderer * ren;
    SDL_Window * win;
    cursor_t * cursor;
    int zoom;
} screen_t;

enum ColourNum {
    BLACK = 0,
    RED = 1,
    YELLOW = 2,
    WHITE = 3
};

SDL_Rect * print_text(screen_t * screen, const char * message);

void ink(cursor_t * cursor, enum ColourNum c_num);

void paper(cursor_t * cursor, enum ColourNum c_num);

void print_left$_b$(screen_t * screen, int width);

void tab(cursor_t * cursor, int x, int y);

void newline(cursor_t * cursor);

screen_t * init_screen();
void destroy_screen(screen_t * screen);
void clear_screen(screen_t * screen);

char inkey$();

void draw_bordered_box(screen_t * screen, int top_row, int left_col, int rows,
                       int cols, enum ColourNum background_colour,
                       enum ColourNum border_colour);

void clear_box(screen_t * screen, SDL_Rect * rect, enum ColourNum colour);

void render_bitmap(screen_t * screen, int col, int row, int bitmap_num,
                   enum ColourNum foreground_colour,
                   enum ColourNum background_colour);
