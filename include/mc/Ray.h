#ifndef RAY_H
#define RAY_H

#include "Camera.h"
#include "World.h"
#include "Chunk.h"

#include "../cglm/vec3.h"
#include "../cglm/cam.h"

#define RAY_STEP 0.01f
#define RAY_LEN 4.0f

BlockID_t block_intercept(Camera* camera, World* world, Chunk** chunk, Voxel** mod)
{
    vec3 pos;
    vec3 front;
    float ray_len;

    int chunk_x;
    int chunk_z;
    int bx;
    int by;
    int bz;
    BlockID_t id;

    glm_vec3_copy(camera->camera_pos, pos);
    glm_vec3_copy(camera->camera_front, front);

    for (ray_len = 0.0f; ray_len < RAY_LEN; ray_len += RAY_STEP) {
        chunk_x = pos[0] / CHUNK_X;
        chunk_z = pos[2] / CHUNK_Z;
        mul_vec_and_add(pos, front, RAY_STEP);
        bx = (int)pos[0] % CHUNK_X;
        by = (int)pos[1] % CHUNK_Y;
        bz = (int)pos[2] % CHUNK_Z;

        id = world->chunks[chunk_z][chunk_x]->m_chunkdata[bx][by][bz]->id;
        *chunk = world->chunks[chunk_z][chunk_x];
        *mod = world->chunks[chunk_z][chunk_x]->m_chunkdata[bx][by][bz];

        if (id != B_AIR) break;
    }
    return id;
}

BlockID_t block_adjacent_intercept(Camera* camera, World* world, Chunk** chunk, Voxel** mod)
{
    vec3 pos;
    vec3 front;
    float ray_len;

    int chunk_x = camera->camera_pos[0] / CHUNK_X;
    int chunk_z = camera->camera_pos[2] / CHUNK_Z;
    int bx;
    int by;
    int bz;
    BlockID_t id = B_AIR;
    Voxel* adjacents[6];
    int _encountered_block = 0;

    glm_vec3_copy(camera->camera_pos, pos);
    glm_vec3_copy(camera->camera_front, front);

    for (ray_len = 0.0f; ray_len < RAY_LEN; ray_len += RAY_STEP) {
        mul_vec_and_add(pos, front, RAY_STEP);
        chunk_x = pos[0] / CHUNK_X;
        chunk_z = pos[2] / CHUNK_Z;
        bx = (int)pos[0] % CHUNK_X;
        by = (int)pos[1] % CHUNK_Y;
        bz = (int)pos[2] % CHUNK_Z;

        id = world->chunks[chunk_z][chunk_x]->m_chunkdata[bx][by][bz]->id;
        if (id != B_AIR) { _encountered_block = 1; break; }
    }
    mul_vec_and_add(pos, front, -RAY_STEP);
    chunk_x = pos[0] / CHUNK_X;
    chunk_z = pos[2] / CHUNK_Z;
    bx = (int)pos[0] % CHUNK_X;
    by = (int)pos[1] % CHUNK_Y;
    bz = (int)pos[2] % CHUNK_Z;

    if (_encountered_block) id = CURR_PLACE_ID;
    else { id = B_AIR; }
    *chunk = world->chunks[chunk_z][chunk_x];
    *mod = world->chunks[chunk_z][chunk_x]->m_chunkdata[bx][by][bz];
    return id;
}


#endif