#include <SDL.h>
#include "libcoord.h"
#include "libmap.h"

int dungeon_t_get_item(dungeon_t * dungeon, coord_t coord) {
    return dungeon->contents[coord.x][coord.y];
}

void dungeon_t_set_item(dungeon_t * dungeon, coord_t coord, int item) {
    dungeon->contents[coord.x][coord.y] = item;
}

int DUNGEON_BASE = 96;

char * dungeon_t_serialize(dungeon_t * dungeon, coord_t start_coord) {
    coord_t coord;
    char * bytes = (char *) malloc(sizeof(char) * 229);
    if (bytes == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "bytes is NULL!");
        exit(1);
    }
    int index = 0;
    for (coord.y = 0; coord.y < 15; coord.y += 1) {
        for (coord.x = 0; coord.x < 15; coord.x += 1) {
            bytes[index] = (char) (
                DUNGEON_BASE + dungeon_t_get_item(dungeon, coord)
            );
            index += 1;
        }
    }
    bytes[index] = (char) (DUNGEON_BASE + start_coord.x + 1);
    index += 1;
    bytes[index] = (char) (DUNGEON_BASE + start_coord.y + 1);
    index += 1;
    bytes[index] = (char) (DUNGEON_BASE + dungeon->level_num);
    index += 1;
    bytes[index] = 0;
    return bytes;
}

int dungeon_t_deserialize(dungeon_t * dungeon, char * bytes,
                          coord_t * start_coord) {
    int index;
    coord_t coord;
    index = 0;
    for (coord.y = 0; coord.y < 15; coord.y += 1) {
        for (coord.x = 0; coord.x < 15; coord.x += 1) {
            dungeon_t_set_item(
                dungeon, coord, (int) bytes[index] - DUNGEON_BASE
            );
            index += 1;
        }
    }
    start_coord->x = (int) bytes[index] - DUNGEON_BASE - 1;
    start_coord->y = (int) bytes[index + 1] - DUNGEON_BASE - 1;
    dungeon->level_num = (int) bytes[index + 2] - DUNGEON_BASE;
    return 0;
}
