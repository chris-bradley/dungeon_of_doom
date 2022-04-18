#include "libcoord.h"

int coord_t_is_equal(coord_t coord, coord_t coord2) {
    if (coord.x == coord2.x && coord.y == coord2.y) {
        return 1;
    }
    return 0;
}

int coord_float_t_is_equal(coord_float_t coord, coord_t coord2) {
    if ((int) coord.x == coord2.x && (int) coord.y == coord2.y) {
        return 1;
    }
    return 0;
}

void coord_t_set(coord_t * target, coord_t source) {
    target->x = source.x;
    target->y = source.y;
}

void coord_t_set_from_float(coord_t * target, coord_float_t source) {
    target->x = (int) source.x;
    target->y = (int) source.y;
}

void coord_float_t_set_from_float(coord_float_t * target,
                                  coord_float_t source) {
    target->x = source.x;
    target->y = source.y;
}
