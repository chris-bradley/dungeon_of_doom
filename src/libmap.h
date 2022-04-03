typedef struct {
    int x;
    int y;
} coord_t;

typedef struct {
    int contents[15][15];
    coord_t entrance_coord;
    int level_num;
} dungeon_t;
