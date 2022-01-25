#include "../GL/glew.h"
#include "../GLFW/glfw3.h"
#include <OpenGL/gl.h>

#include <stdio.h>
#include <stdlib.h>

#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct {
    unsigned int global_texture;
    unsigned int width;
    unsigned int height;
} GlobalTexture;

void load_texture(GlobalTexture* tex, const char* texture_fpath)
{
    glGenTextures(1, &(tex->global_texture));
    glBindTexture(GL_TEXTURE_2D, tex->global_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, n_chan;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texture_fpath, &width, &height, &n_chan, 0);
    tex->width = width;
    tex->height = height;

    if (!data) {
        fprintf(stderr, "Error - failed to load global texture %s.\n", texture_fpath);
        exit(-1);
    } else {
        printf("Correctly loaded texture %s!\n", texture_fpath);
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    printf("Finished texture loading.\n");
}

void apply_texture(GlobalTexture* tex)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->global_texture);
}

GlobalTexture* gen_global_tex(const char* texture_fpath)
{
    GlobalTexture* tex = (GlobalTexture*) malloc(sizeof(GlobalTexture));
    load_texture(tex, texture_fpath);
    return tex;
}

#endif