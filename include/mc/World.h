#ifndef WORLD_H
#define WORLD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "Chunk.h"
#include "Texture.h"
#include "Camera.h"

#define WORLD_W 8
#define WORLD_L 8

typedef struct
{
    uint32_t seed;
    Chunk* chunks[WORLD_L][WORLD_W];
} World;

World* create_world(GlobalTexture* tex, Camera* cam)
{
    World* new_world = (World*) malloc(sizeof(World));
    new_world->seed = (uint32_t)time(NULL);
    for (int y = 0; y < WORLD_L; y++) {
        for (int x = 0; x < WORLD_W; x++) {
            new_world->chunks[y][x] = create_chunk((float)x * 8, (float)y * 16, tex, cam);
            generate_chunk(new_world->chunks[y][x]);
        }
    }
    return new_world;
}

Chunk* get_nearby_chunk(World* world, int x, int y)
{
    if (x < 0 || y < 0 || y >= WORLD_L || x >= WORLD_W) return NULL;
    else {
        return world->chunks[x][y];
    }
}

void mesh_world(World* world)
{
    for (int y = 0; y < WORLD_L; y++) {
        for (int x = 0; x < WORLD_W; x++) {
            build_mesh(world->chunks[y][x]);
        }
    }
}

void render_world(World* world)
{
    for (int y = 0; y < WORLD_L; y++) {
        for (int x = 0; x < WORLD_W; x++) {
            draw_chunk(world->chunks[y][x]);
        }
    }
}

void delete_world(World* world)
{
    for (int y = 0; y < WORLD_L; y++) {
        for (int x = 0; x < WORLD_W; x++) {
            delete_chunk(world->chunks[y][x]);
        }
    }
    free(world);
}

#endif