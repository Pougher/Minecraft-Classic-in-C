#ifndef BLOCK_ID_H
#define BLOCK_ID_H

#include "../cglm/vec2.h"

#define PIXEL 1.0f / 480.0f

#define BLOCK_GRASS_TC (float[12]) { PIXEL * 140, 0.0f,\
                                     PIXEL * 140, 0.0f,\
                                     PIXEL * 168, 0.0f,\
                                     PIXEL * 168, 0.0f,\
                                     PIXEL * 56 , 0.0f,\
                                     0.0f, 0.0f}
#define BLOCK_STONE_TC (float[12]) { PIXEL * 28, 0.0f,\
                                     PIXEL * 28, 0.0f,\
                                     PIXEL * 28, 0.0f,\
                                     PIXEL * 28, 0.0f,\
                                     PIXEL * 28, 0.0f,\
                                     PIXEL * 28, 0.0f }
#define BLOCK_DIRT_TC  (float[12]) { PIXEL * 56, 0.0f,\
                                     PIXEL * 56, 0.0f,\
                                     PIXEL * 56, 0.0f,\
                                     PIXEL * 56, 0.0f,\
                                     PIXEL * 56, 0.0f,\
                                     PIXEL * 56, 0.0f,}
#define BLOCK_WATER_TC (float[12]) { PIXEL * 84, 0.0f,\
                                     PIXEL * 84, 0.0f,\
                                     PIXEL * 84, 0.0f,\
                                     PIXEL * 84, 0.0f,\
                                     PIXEL * 84, 0.0f,\
                                     PIXEL * 84, 0.0f,}
#define BLOCK_SAND_TC  (float[12]) { PIXEL * 112, 0.0f,\
                                     PIXEL * 112, 0.0f,\
                                     PIXEL * 112, 0.0f,\
                                     PIXEL * 112, 0.0f,\
                                     PIXEL * 112, 0.0f,\
                                     PIXEL * 112, 0.0f,}
#define BLOCK_WOOD_TC  (float[12]) { PIXEL * 196, 0.0f,\
                                     PIXEL * 196, 0.0f,\
                                     PIXEL * 196, 0.0f,\
                                     PIXEL * 196, 0.0f,\
                                     PIXEL * 224 , 0.0f,\
                                     PIXEL * 224, 0.0f}
#define BLOCK_LEAF_TC  (float[12]) { PIXEL * 252, 0.0f,\
                                     PIXEL * 252, 0.0f,\
                                     PIXEL * 252, 0.0f,\
                                     PIXEL * 252, 0.0f,\
                                     PIXEL * 252 , 0.0f,\
                                     PIXEL * 252, 0.0f}
#define BLOCK_PLANK_TC (float[12]) { PIXEL * 280, 0.0f,\
                                     PIXEL * 280, 0.0f,\
                                     PIXEL * 280, 0.0f,\
                                     PIXEL * 280, 0.0f,\
                                     PIXEL * 280 , 0.0f,\
                                     PIXEL * 280, 0.0f}

typedef enum {
    B_GRASS = 0,
    B_STONE = 1,
    B_DIRT  = 2,
    B_WATER = 3,
    B_SAND  = 4,
    B_WOOD  = 5,
    B_LEAF  = 6,
    B_PLANK = 7,
    B_AIR   = 999,
}BlockID_t;

void copy_vec12(float source[12], float dest[12])
{
    for (unsigned int i = 0; i < 12; i++) dest[i] = source[i];
}

void block_id_uv(BlockID_t id, float vec[12])
{
    switch(id) {
        case B_GRASS:
            copy_vec12(BLOCK_GRASS_TC, vec);
            break;
        case B_STONE:
            copy_vec12(BLOCK_STONE_TC, vec);
            break;
        case B_DIRT:
            copy_vec12(BLOCK_DIRT_TC, vec);
            break;
        case B_WATER:
            copy_vec12(BLOCK_WATER_TC, vec);
            break;
        case B_SAND:
            copy_vec12(BLOCK_SAND_TC, vec);
            break;
        case B_WOOD:
            copy_vec12(BLOCK_WOOD_TC, vec);
            break;
        case B_LEAF:
            copy_vec12(BLOCK_LEAF_TC, vec);
            break;
        case B_PLANK:
            copy_vec12(BLOCK_PLANK_TC, vec);
            break;
        default:
            break;
    }
}

#endif