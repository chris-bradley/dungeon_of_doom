#pragma once

typedef struct {
    int x;
    int y;
} coord_t;

// A combination of two factors make the next struct necessary:
// 1) Large-block grids
// 2) Monsters can move diagonally
// Once we move to small-block grids, it will become unnecessary.
typedef struct {
    float x;
    float y;
} coord_float_t;

int coord_t_is_equal(coord_t coord, coord_t coord2);
int coord_float_t_is_equal(coord_float_t coord, coord_t coord2);

void coord_t_set(coord_t * target, coord_t source);
void coord_t_set_from_float(coord_t * target, coord_float_t coord);
void coord_float_t_set_from_float(coord_float_t * target,
		                  coord_float_t source);
