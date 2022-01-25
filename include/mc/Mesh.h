#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>
#include "BlockId.h"

const int CUBE_FACE_0[18] = {
    0, 0, 0,
    1, 0, 0,
    1, 1, 0,
    1, 1, 0,
    0, 1, 0,
    0, 0, 0,
};

const int CUBE_FACE_1[18] = {
    0, 0, 1,
    1, 0, 1,
    1, 1, 1,
    1, 1, 1,
    0, 1, 1,
    0, 0, 1,
};

const int CUBE_FACE_2[18] = {
    0, 1, 1,
    0, 1, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 1,
    0, 1, 1,
};

const int CUBE_FACE_3[18] = {
    1, 1, 1,
    1, 1, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 1,
    1, 1, 1,
};

const int CUBE_FACE_4[18] = {
    0, 0, 0,
    1, 0, 0,
    1, 0, 1,
    1, 0, 1,
    0, 0, 1,
    0, 0, 0,
};

const int CUBE_FACE_5[18] = {
    0, 1, 0,
    1, 1, 0,
    1, 1, 1,
    1, 1, 1,
    0, 1, 1,
    0, 1, 0,
};

typedef struct {
    float* data;
    unsigned int len;
    unsigned int tris;

    unsigned int VAO;
    unsigned int VBO;
} MeshBuilder;

MeshBuilder* create_mesh()
{
    MeshBuilder* mesh = (MeshBuilder*)malloc(sizeof(MeshBuilder));
    mesh->len = 0;
    mesh->tris = 0;
    return mesh; // enough space for one vertex and some texture information.
}

void append_float_mesh(MeshBuilder* mesh, float d)
{
    unsigned int old_sz = mesh->len - 1;
    mesh->data = (float*)realloc(mesh->data, sizeof(float) * (mesh->len + 5));
    mesh->data[old_sz] = d;
}

void append_mesh(MeshBuilder* vertex_data, float info[5])
{
    unsigned int old_sz = vertex_data->len - 1;
    vertex_data->data = (float*)realloc(vertex_data->data, sizeof(float) * (vertex_data->len + 5));
    for (unsigned int i = 0; i < 5; i++) vertex_data->data[old_sz + i] = info[i];
}

void append_side(MeshBuilder* vertex_data, float info[36])
{
    unsigned int old_sz = vertex_data->len;
    if (!old_sz) {
        vertex_data->data = (float*)malloc(sizeof(float) * 36);
        for (unsigned int i = 0; i < 36; i++) vertex_data->data[old_sz + i] = info[i];
    } else {
        vertex_data->data = (float*)realloc(vertex_data->data, sizeof(float) * (vertex_data->len + 36));
        for (unsigned int i = 0; i < 36; i++) vertex_data->data[old_sz + i] = info[i];
    }
    vertex_data->len = old_sz + 36;
}

void append_cube_vertices(MeshBuilder* mesh, float cube_data[180])
{
    unsigned int old_sz = mesh->len;
    if (!old_sz) {
        mesh->data = (float*)malloc(sizeof(float) * 180);
        for (unsigned int i = 0; i < 180; i++) mesh->data[old_sz + i] = cube_data[i];
        mesh->len = old_sz + 180;
        return;
    } else {
        mesh->data = (float*)realloc(mesh->data, sizeof(float) * (mesh->len + 180));
        for (unsigned int i = 0; i < 180; i++) mesh->data[old_sz + i] = cube_data[i];
        mesh->len = old_sz + 180;
    }
}

void free_mesh(MeshBuilder* mesh)
{
    free(mesh->data);
    free(mesh);
}

void append_face(MeshBuilder* mesh, float uv_coords[2], const int face[18], int x, int y, int z, float brightness)
{
    append_side(mesh, (float[36]) {
        face[0] + x, face[1] + y, face[2] + z, uv_coords[0], uv_coords[1], brightness,
        face[3] + x, face[4] + y, face[5] + z, uv_coords[0] + (PIXEL * 16), uv_coords[1], brightness,
        face[6] + x, face[7] + y, face[8] + z, uv_coords[0] + (PIXEL * 16), uv_coords[1] + (PIXEL * 16), brightness,
        face[9] + x, face[10] + y, face[11] + z, uv_coords[0] + (PIXEL * 16), uv_coords[1] + (PIXEL * 16), brightness,
        face[12] + x, face[13] + y, face[14] + z, uv_coords[0], uv_coords[1] + (PIXEL * 16), brightness,
        face[15] + x, face[16] + y, face[17] + z, uv_coords[0], uv_coords[1], brightness
    });
    
    mesh->tris += 6;
}

void print_mesh(MeshBuilder* mesh)
{
    for (int i = 0; i < mesh->len; i++) {
        printf("%f ", mesh->data[i]);
        if (i % 5 == 0 && i != 0) printf("\n");
    }
    printf("\n");
}

void create_vao(MeshBuilder* mesh)
{
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->len * sizeof(float), mesh->data, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    mesh->VBO = VBO;
    mesh->VAO = VAO;
}

#endif