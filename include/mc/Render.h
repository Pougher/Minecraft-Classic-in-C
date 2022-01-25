#include "../GL/glew.h"
#include "../GLFW/glfw3.h"
#include <OpenGL/gl.h>

#include <stdio.h>
#include <stdlib.h>

#ifndef RENDER_H
#define RENDER_H

typedef struct {
    const char* name;
    GLFWmonitor* monitor;
    GLFWwindow* share;
    GLFWwindow* window;
    unsigned int w;
    unsigned int h;
}Window;

void set_glfw_hints()
{
    /*
     * Sets GLFW window hints 
    */
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Window* create_window(const char* name, 
                      unsigned int w, 
                      unsigned int h, 
                      GLFWmonitor* mon, 
                      GLFWwindow* share)
{
    /*
     * Create a new GLFW window instance.
    */
    Window* new_window = (Window*) malloc(sizeof(Window));
    new_window->name = name;
    new_window->w = w;
    new_window->h = h;
    new_window->monitor = mon;
    new_window->share = share;
    new_window->window = glfwCreateWindow(w, h, name, mon, share);
    if (new_window->window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(new_window->window);
    glfwSetInputMode(new_window->window, GLFW_STICKY_KEYS, GL_TRUE);

    return new_window;
}

#endif
