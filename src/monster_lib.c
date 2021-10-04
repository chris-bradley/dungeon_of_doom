#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include "monster_lib.h"

int MONSTER_CODE_BASE = 104;

monster_t * monster_init(int type, int coord_x, int coord_y) {
    monster_t * monster = malloc(sizeof(monster_t));
    monster->char_code = type + MONSTER_CODE_BASE;
    monster->type = type;
    monster->speed = type / 16.0;
    monster->strength = type * 6;
    monster->coord_x = (float) coord_x;
    monster->coord_y = (float) coord_y;

    return monster;
}

monster_list_t * monster_list_init() {
    monster_list_t * monster_list = malloc(sizeof(monster_list_t));
    if (monster_list == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "monster_list is NULL!");
        exit(1);
    }
    *monster_list = (monster_list_t) {
        .first_node = NULL
    };
    return monster_list;
}

void monster_list_add(monster_list_t * monster_list, monster_t * monster) {
    monster_list_node_t * new_node = malloc(sizeof(monster_list_node_t));
    if (new_node == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "new_node is NULL!");
        exit(1);
    }
    *new_node = (monster_list_node_t) {
        .monster = monster,
        .next_node = monster_list->first_node
    };
    monster_list->first_node = new_node;
}

void monster_list_remove(monster_list_t * monster_list, monster_t * monster) {
    monster_list_node_t * node = monster_list->first_node, * prev_node = NULL;
    while (node != NULL) {
        if (node->monster == monster) {
            if (node == monster_list->first_node) {
                monster_list->first_node = node->next_node;
            } else {
                prev_node->next_node = node->next_node;
            }
            free(node);
            return;
        }
        prev_node = node;
        node = node->next_node;
    }
}

monster_t * monster_list_get_nearest(monster_list_t * monster_list,
                                     int coord_x, int coord_y) {
    float distance, closest_distance = 0;
    monster_list_node_t * monster_node = monster_list->first_node;
    monster_t * monster, * nearest_monster = NULL;
    while (monster_node != NULL) {
        monster = monster_node->monster;
        distance = sqrt(
            pow(abs(monster->coord_x - coord_x), 2) -
                pow(abs(monster->coord_y - coord_y), 2)
        );
        if (closest_distance == 0 || distance < closest_distance) {
            nearest_monster = monster;
            closest_distance = distance;
        }
        monster_node = monster_node->next_node;
    }

    return nearest_monster;
}

void monster_list_clear(monster_list_t * monster_list) {
    monster_list_node_t * monster_node = monster_list->first_node, * next_node;
    while (monster_node != NULL) {
        free(monster_node->monster);
        next_node = monster_node->next_node;
        free(monster_node);
        monster_node = next_node;
    }
    monster_list->first_node = NULL;
}
