#include "libcoord.h"

enum AttrNum {
    STRENGTH = 0,
    VITALITY = 1,
    AGILITY = 2,
    INTELLIGENCE = 3,
    EXPERIENCE = 4,
    LUCK = 5,
    AURA = 6,
    MORALITY = 7
};

enum InventoryCode {
    TWO_HAND_SWORD = 0,
    BROADSWORD = 1,
    SHORTSWORD = 2,
    AXE = 3,
    MACE = 4,
    FLAIL = 5,
    DAGGER = 6,
    GAUNTLET = 7,
    HEAVY_ARMOUR = 8,
    CHAIN_ARMOUR = 9,
    LEATHER_ARMOUR = 10,
    HEAVY_ROBE = 11,
    GOLD_HELMET = 12,
    HEADPIECE = 13,
    SHIELD = 14,
    TORCH = 15,
    NECRONOMICON = 16,
    SCROLLS = 17,
    RING = 18,
    MYSTIC_AMULET = 19,
    SASH = 20,
    CLOAK = 21,
    HEALING_SALVE = 22,
    POTIONS = 23
};

struct character_s_t;

typedef struct {
    int id;
    const char * name;
    int (*elig_func)(struct character_s_t *);
} character_class_t;

typedef struct character_s_t {
    coord_t coord;
    int facing;  // Facing. NESW
    double * attrs;
    double initial_strength;
    double initial_vitality;
    double initial_experience;
    character_class_t * class;
    char * name;
    int * inventory;
    int * spells_remaining;
    int treasure;
    int gold;
    int torches;
} character_t;
