#include "../GL/glew.h"
#include "../GLFW/glfw3.h"
#include <OpenGL/gl.h>

#include <stdio.h>
#include <stdlib.h>

#include "../cglm/vec3.h"
#include "../cglm/cam.h"
#include "../cglm/mat4.h"
#include "../cglm/mat3.h"
#include "../cglm/mat2.h"

#ifndef SHADER_H
#define SHADER_H

unsigned int shaders[512];
unsigned int current_shader = 0;

void verify_shader(const char* err, unsigned int shader)
{
    int success;
    char infLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infLog);
        fprintf(stderr, "%s \n\t%s\n", err, infLog);
        exit(-1);
    }
}

char* load_file(const char* p_filename)
{
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen(p_filename, "rb");
    if (!fp) {
        perror(p_filename), exit(1);
    }

    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    /* allocate memory for entire content */
    buffer = calloc(1, lSize + 1);
    if (!buffer) {
        fclose(fp), fputs("memory alloc fails", stderr), exit(1);
    }

    /* copy the file into the buffer */
    if (1 != fread(buffer, lSize, 1, fp)) {
        fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);
    }

    return buffer;
}

void generate_single_shader(char** frag_shader, char** vert_shader)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgram;
    glShaderSource(vertexShader, 1, (const char**)vert_shader, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, (const char**)frag_shader, NULL);
    glCompileShader(fragmentShader);

    verify_shader("Error - vertex shader failed to compile:", vertexShader);
    verify_shader("Error - fragment shader failed to compile:", fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    verify_shader("Error - failed to link shaders:", shaderProgram);
    shaders[current_shader] = shaderProgram;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    current_shader++;
}

void load_shaders(const char* file_list[][2], unsigned int len)
{
    for (unsigned int i = 0; i < len; i++) {
        char* fragshader = load_file(file_list[i][0]);
        char* vertshader = load_file(file_list[i][1]);

        generate_single_shader(&fragshader,  &vertshader);
        free(fragshader);
        free(vertshader);
        printf("Loaded shaders '%s' and '%s'.\n", file_list[i][0], file_list[i][1]);
    }
}

void shader_set_int(int ID, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shaders[ID], name), value);
}
// ------------------------------------------------------------------------
void shader_set_float(int ID, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(shaders[ID], name), value);
}
// ------------------------------------------------------------------------
void shader_set_vec2(int ID, const char* name, vec2 value)
{
    glUniform2fv(glGetUniformLocation(shaders[ID], name), 1, &value[0]);
}
// ------------------------------------------------------------------------
void shader_set_vec3(int ID, const char* name, vec3 value)
{
    glUniform3fv(glGetUniformLocation(shaders[ID], name), 1, &value[0]);
}
// ------------------------------------------------------------------------
void shader_set_vec4(int ID, const char* name, vec4 value)
{
    glUniform4fv(glGetUniformLocation(shaders[ID], name), 1, &value[0]);
}
// ------------------------------------------------------------------------
void shader_set_mat2(int ID, const char* name, mat2 mat)
{
    glUniformMatrix2fv(glGetUniformLocation(shaders[ID], name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void shader_set_mat3(int ID, const char* name, mat3 mat)
{
    glUniformMatrix3fv(glGetUniformLocation(shaders[ID], name), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void shader_set_mat4(int ID, const char* name, mat4 mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shaders[ID], name), 1, GL_FALSE, &mat[0][0]);
}

#endif