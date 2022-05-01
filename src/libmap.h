typedef struct {
    int contents[15][15];
    coord_t entrance_coord;
    int level_num;
} dungeon_t;

int dungeon_t_get_item(dungeon_t * dungeon, coord_t coord);
void dungeon_t_set_item(dungeon_t * dungeon, coord_t coord, int item);

char * dungeon_t_serialize(dungeon_t * dungeon, coord_t start_coord);
int dungeon_t_deserialize(dungeon_t * dungeon, char * bytes,
                          coord_t * start_coord);
