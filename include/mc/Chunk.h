#ifndef CHUNK_H
#define CHUNK_H

#include "Voxel.h"
#include "Texture.h"
#include "Mesh.h"
#include "BlockId.h"
#include "Perlin.h"

#include "../cglm/vec3.h"
#include "../cglm/cam.h"
#include "../cglm/mat4.h"
#include "../cglm/affine.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CHUNK_X 16
#define CHUNK_Y 128
#define CHUNK_Z 16

#define TERRAIN_MAX 14

/*
Each chunk in this game is 16x16x64, and world
generation can reach up to 32 blocks.
*/

typedef struct {
    Voxel* m_chunkdata[CHUNK_X][CHUNK_Y][CHUNK_Z];
    unsigned int width;
    unsigned int height;
    unsigned int depth;
    unsigned int mesh;
    unsigned int vbo;
    unsigned int tri_count;

    vec3 pos;

    GlobalTexture* tex;
    Camera* cam;
}Chunk;

Chunk* create_chunk(float x, float y, GlobalTexture* tex, Camera* cam)
{
    Chunk* new_chunk = (Chunk*) malloc(sizeof(Chunk));
    new_chunk->width = CHUNK_X;
    new_chunk->height = CHUNK_Y;
    new_chunk->depth = CHUNK_Z;

    new_chunk->tex = tex;
    new_chunk->pos[0] = x;
    new_chunk->pos[1] = y;

    printf("%f %f\n", new_chunk->pos[0], new_chunk->pos[1]);
    new_chunk->cam = cam;
    new_chunk->tri_count = 0;
    
    return new_chunk;
}

BlockID_t rand_block_id()
{
    switch(rand() % 4) {
        case 0:
            return B_GRASS;
            break;
        case 1:
            return B_STONE;
            break;
        case 2:
            return B_DIRT;
            break;
        default:
            return B_AIR;
            break;
    }
}

int randint(int a, int b)
{
    return (rand() % b) + a;
}

void selective_place(Chunk* ch, int x, int y, int z, BlockID_t material)
{
    if (x >= CHUNK_X || z >= CHUNK_Z || y >= CHUNK_Y) {
        return; // dont do anything.
    }
    update_voxel(ch->m_chunkdata[x][y][z], material);
}

void generate_chunk(Chunk* chunk)
{
    set_seed(time(NULL));
    for (int x = 0; x < chunk->width; x++) {
        for (int y = 0; y < chunk->height; y++) {
            for (int z = 0; z < chunk->depth; z++) {
                chunk->m_chunkdata[x][y][z] = create_voxel(B_AIR,
                                                           (float)x + chunk->pos[0],
                                                           (float)y,
                                                           (float)z + chunk->pos[1], 0, 
                                                           chunk->tex);
            }
        }
    }
    for (int x = 0; x < chunk->width; x++) {
        for (int y = 0; y < chunk->height; y++) {
            for (int z = 0; z < chunk->depth; z++) {
                int y_threshHold = perlin2d((x + chunk->pos[0] * 2) / 12.0f, (z + chunk->pos[1]) / 12.0f, 1.2f, 2) * TERRAIN_MAX;
                y_threshHold += 30;
                BlockID_t id = B_GRASS;
                if (y_threshHold < 38 && y_threshHold > 36) id = B_SAND;
                if (y > y_threshHold) id = B_AIR;
                if (y < y_threshHold) id = B_DIRT;
                if (y < (y_threshHold * 0.8)) id = B_STONE;
                if (y_threshHold < 37 && (y > y_threshHold * 0.91) && y < 37) id = B_WATER;
                if (chunk->m_chunkdata[x][y][z]->id == B_AIR) update_voxel(chunk->m_chunkdata[x][y][z], id);
                if (y == y_threshHold+1 && chunk->m_chunkdata[x][y][z]->id != B_WATER && chunk->m_chunkdata[x][y-1][z]->id != B_WATER
                    && x > 5 && z > 5 && x < (CHUNK_X * 12) - 6 && z < (CHUNK_Z * 12) - 6) {
                    if (x % CHUNK_X > 4 && z % CHUNK_Z > 4 && z % CHUNK_Z < 12 && x % CHUNK_X < 12) {
                        if (randint(0, 100) == 0) {
                            update_voxel(chunk->m_chunkdata[x][y][z], B_WOOD);
                            for (int i = 1; i < 5; i++) {
                                update_voxel(chunk->m_chunkdata[x][y + i][z], B_WOOD);
                            }

                            for (int t = 0; t < 2; t++) {
                                for (int i = -2; i <= 2; i++) {
                                    for (int j = -2; j <= 2; j++) {
                                        if (!(i == 0 && j == 0)) selective_place(chunk, x + i, y + 2 + t, z + j, B_LEAF);
                                    }
                                }
                            }
                            for (int i = -1; i <= 1; i++) {
                                for (int j = -1; j <= 1; j++) {
                                    if (!(i == 0 && j == 0)) selective_place(chunk, x + i, y + 4, z + j, B_LEAF);
                                }
                            }
                            selective_place(chunk, x + 1, y + 5, z, B_LEAF);
                            selective_place(chunk, x, y + 5, z + 1, B_LEAF);
                            selective_place(chunk, x - 1, y + 5, z, B_LEAF);
                            selective_place(chunk, x, y + 5, z - 1, B_LEAF);
                            selective_place(chunk, x, y + 5, z, B_LEAF);
                        }
                    }
                }
            }
        }
    }
    printf("Generated chunk with %d blocks.\n", chunk->width * chunk->height * chunk->depth);
}

void delete_chunk(Chunk* chunk)
{
    for (int x = 0; x < chunk->width; x++) {
        for (int y = 0; y < chunk->height; y++) {
            for (int z = 0; z < chunk->depth; z++) {
                free(chunk->m_chunkdata[x][y][z]);
            }
        }
    }
    free(chunk);
}

// returns wether the block is visible.

int block_near(int x, int y, int z, Voxel* m_chunkdata[CHUNK_X][CHUNK_Y][CHUNK_Z])
{
    if (x < 0 || x >= CHUNK_X || y < 0 || y >= CHUNK_Y || z < 0 || z >= CHUNK_Z) return 1;
    return m_chunkdata[x][y][z]->transparent;
}

void draw_chunk(Chunk* chunk)
{
    mat4 model;
    mat4 model_copy;
    vec3 voxpos;
    vec3 axis = { 0, 1, 0 };
    glm_vec3_copy(chunk->pos, voxpos);
    glm_mat4_identity(model);
    glm_translate(model, voxpos);
    shader_set_mat4(0, "projection", chunk->cam->proj);
    shader_set_mat4(0, "view", chunk->cam->view);
    shader_set_mat4(0, "model", model);

    glUseProgram(shaders[0]);

    glBindVertexArray(chunk->mesh);
    glDrawArrays(GL_TRIANGLES, 0, chunk->tri_count);
}

void slice2(float src[12], float dest[2], int ind)
{
    dest[0] = src[ind - 1];
    dest[1] = src[ind];
}

void build_mesh(Chunk* chunk)
{
    MeshBuilder* mesh = create_mesh();
    for (unsigned int x = 0; x < chunk->width; x++) {
        for (unsigned int y = 0; y < chunk->height; y++) {
            for (unsigned int z = 0; z < chunk->depth; z++) {
                float tex_coords[12];
                float tex_slice[2];
                block_id_uv(chunk->m_chunkdata[x][y][z]->id, tex_coords);
                if (!(chunk->m_chunkdata[x][y][z]->id == B_AIR)) {
                    if (block_near(x, y, z-1, chunk->m_chunkdata)) {
                        slice2(tex_coords, tex_slice, 1);
                        append_face(mesh, tex_slice, CUBE_FACE_0, x + chunk->pos[0], y - chunk->pos[1], z + chunk->pos[1], 0.8f);
                    }
                    if (block_near(x, y, z+1, chunk->m_chunkdata)) {
                        slice2(tex_coords, tex_slice, 3);
                        append_face(mesh, tex_slice, CUBE_FACE_1, x + chunk->pos[0], y - chunk->pos[1], z + chunk->pos[1], 1.0f);
                    }
                    if (block_near(x-1, y, z, chunk->m_chunkdata)) {
                        slice2(tex_coords, tex_slice, 5);
                        append_face(mesh, tex_slice, CUBE_FACE_2, x + chunk->pos[0], y - chunk->pos[1], z + chunk->pos[1], 0.7f);
                    }
                    if (block_near(x+1, y, z, chunk->m_chunkdata)) {
                        slice2(tex_coords, tex_slice, 7);
                        append_face(mesh, tex_slice, CUBE_FACE_3, x + chunk->pos[0], y - chunk->pos[1], z + chunk->pos[1], 0.75f);
                    }
                    if (block_near(x, y-1, z, chunk->m_chunkdata)) {
                        slice2(tex_coords, tex_slice, 9);
                        append_face(mesh, tex_slice, CUBE_FACE_4, x + chunk->pos[0], y - chunk->pos[1], z + chunk->pos[1], 0.8f);
                    }
                    if (block_near(x, y+1, z, chunk->m_chunkdata)) {
                        slice2(tex_coords, tex_slice, 11);
                        append_face(mesh, tex_slice, CUBE_FACE_5, x + chunk->pos[0], y - chunk->pos[1], z + chunk->pos[1], 0.9f);
                    }
                }
            }
        }
    }
    create_vao(mesh);
    chunk->vbo = mesh->VBO;
    chunk->mesh = mesh->VAO;
    chunk->tri_count = mesh->tris;
    free_mesh(mesh);
}

void regen_mesh(Chunk* chunk)
{
    glDeleteBuffers(1, &(chunk->vbo));
    glDeleteVertexArrays(1, &(chunk->mesh));
    build_mesh(chunk);
}

uint32_t float_to_secs(float fl)
{
    union x {
        float i;
        uint32_t out;
    };

    union x ret;
    ret.i = fl;
    return ret.out;
}

void random_chunk(Chunk* chunk)
{
    srand(float_to_secs(glfwGetTime()));
    for (int x = 0; x < chunk->width; x++) {
        for (int y = 0; y < chunk->height; y++) {
            for (int z = 0; z < chunk->depth; z++) {
                update_voxel(chunk->m_chunkdata[x][y][z], rand_block_id());
            }
        } 
    }
    regen_mesh(chunk);
}

#endif