

typedef struct {
    float coord_x;
    float coord_y;
    float next_coord_x;
    float next_coord_y;
    float speed;
    int distance_x;  // A value of 255 indicates no monster active.
    int distance_y;
    int type;
    int strength;
    int char_code;
} monster_t;
