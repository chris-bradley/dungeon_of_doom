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
