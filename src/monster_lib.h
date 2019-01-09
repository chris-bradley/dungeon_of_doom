typedef struct {
    float coord_x;
    float coord_y;
    float speed;
    int type;
    int strength;
    int char_code;
} monster_t;

struct monster_list_node_s_t;

typedef struct monster_list_node_s_t {
    monster_t * monster;
    struct monster_list_node_s_t * next_node;
} monster_list_node_t;

typedef struct {
    monster_list_node_t * first_node;
} monster_list_t;

monster_t * monster_init(int type, int coord_x, int coord_y);

monster_list_t * monster_list_init();
void monster_list_add(monster_list_t * monster_list, monster_t * monster);
void monster_list_remove(monster_list_t * monster_list, monster_t * monster);
monster_t * monster_list_get_nearest(monster_list_t * monster_list,
                                     int coord_x, int coord_y);
void monster_list_clear(monster_list_t * monster_list);
