#include "libcoord.h"
#include "libmap.h"

int dungeon_t_get_item(dungeon_t * dungeon, coord_t coord) {
    return dungeon->contents[coord.x][coord.y];
}

void dungeon_t_set_item(dungeon_t * dungeon, coord_t coord, int item) {
    dungeon->contents[coord.x][coord.y] = item;
}
