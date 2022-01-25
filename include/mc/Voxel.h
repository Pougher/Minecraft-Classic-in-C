#include <stdio.h>
#include <stdlib.h>

#include "../GL/glew.h"
#include "../GLFW/glfw3.h"
#include <OpenGL/gl.h>

#include "Camera.h"
#include "../cglm/vec3.h"
#include "../cglm/cam.h"
#include "../cglm/mat4.h"
#include "../cglm/affine.h"

#include "Texture.h"
#include "Mesh.h"
#include "BlockId.h"

#ifndef VOXEL_H
#define VOXEL_H

typedef struct {
    vec3 pos;
    unsigned int shader_num;
    GlobalTexture* texture;
    BlockID_t id;
    int transparent;
} Voxel;

inline void assign_vert_bufferf(float* buffer, float* vals, unsigned int len)
{
    for (int x = 0; x < len; x++) buffer[x] = vals[x];
}

inline void assign_vert_bufferi(unsigned int* buffer, unsigned int* vals, unsigned int len)
{
    for (int x = 0; x < len; x++) buffer[x] = vals[x];
}

Voxel* create_voxel(BlockID_t id, float x, float y, float z, unsigned int shader_n, GlobalTexture* tex)
{
    Voxel* new_voxel = (Voxel*) malloc(sizeof(Voxel));
    new_voxel->pos[0] = x;
    new_voxel->pos[1] = y;
    new_voxel->pos[2] = z;
    new_voxel->shader_num = shader_n;
    new_voxel->texture = tex;
    new_voxel->id = id;
    new_voxel->transparent = 0;
    if (id == B_AIR) new_voxel->transparent = 1;
    
    return new_voxel;
}

void gen_model(mat4 model, vec3 transform, vec3 axis, float rad, mat4 model_copy)
{
    glm_mat4_copy(model, model_copy);
    glm_translate(model_copy, transform);
    glm_rotate(model_copy, rad, axis);
}

void set_mat_and_draw(mat4 model, Voxel* voxel)
{
    shader_set_mat4(voxel->shader_num, "model", model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void update_voxel(Voxel* voxel, BlockID_t new_id)
{
    voxel->id = new_id;
    if (new_id == B_AIR) voxel->transparent = 1;
    if (new_id != B_AIR) voxel->transparent = 0;
}

#endif