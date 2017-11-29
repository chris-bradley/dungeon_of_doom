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

void print_text(screen_t *screen, cursor_t *cursor, const char *message);

void ink(cursor_t *cursor, int c_num);

void paper(cursor_t *cursor, int c_num);
