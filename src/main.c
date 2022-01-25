#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/GL/glew.h"
#include "../include/GLFW/glfw3.h"
#include <OpenGL/gl.h>

#include "../include/mc/Shader.h"
#include "../include/mc/Render.h"
#include "../include/mc/Shader.h"
#include "../include/mc/Voxel.h"
#include "../include/mc/Camera.h"
#include "../include/mc/Chunk.h"
#include "../include/mc/Mesh.h"
#include "../include/mc/World.h"
#include "../include/mc/Perlin.h"

BlockID_t CURR_PLACE_ID = B_GRASS;

#include "../include/mc/Ray.h"

#include "../include/cglm/vec3.h"
#include "../include/cglm/cam.h"

#define WIDTH 1280
#define HEIGHT 720

const char* SHADER_FPATH[][2] = {
    { "shaders/fragshader.glsl", "shaders/vertshader.glsl" }
};

Camera* camera;
World* world;
double mousex = 0;
double mousey = 0;
int firstMouse = 1;

void check_leaks();

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        camera->lastX = xpos;
        camera->lastY = ypos;
        firstMouse = 0;
    }

    mousex = xpos;
    mousey = ypos;
  
    float xoffset = xpos - camera->lastX;
    float yoffset = camera->lastY - ypos; 
    camera->lastX = xpos;
    camera->lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera->yaw   += xoffset;
    camera->pitch += yoffset;

    if(camera->pitch > 89.0f)
        camera->pitch = 89.0f;
    if(camera->pitch < -89.0f)
        camera->pitch = -89.0f;

    vec3 direction;
    direction[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    direction[1] = sin(glm_rad(camera->pitch));
    direction[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    glm_normalize(direction);
    glm_vec3_copy(direction, camera->camera_front);
}

void mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        Voxel* voxel;
        Chunk* chunk;
        BlockID_t id = block_intercept(camera, world, &chunk, &voxel);
        if (id != B_AIR) {
            update_voxel(voxel, B_AIR);
            regen_mesh(chunk);
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        Voxel* voxel;
        Chunk* chunk;
        BlockID_t id = block_adjacent_intercept(camera, world, &chunk, &voxel);
        if (id != B_AIR) {
            update_voxel(voxel, id);
            regen_mesh(chunk);
        } 
        printf("%d\n", id);
    }
}

int main(int argc, char** argv)
{
    glewExperimental = GL_TRUE;
    if (!glfwInit()) {
        fprintf(stderr, "Error - GLFW failed to init.\n");
        return -1;
    }

    set_glfw_hints();

    Window* window = create_window("Minecraft", 1280, 720, NULL, NULL);
    GlobalTexture* global_texture = gen_global_tex("assets/atlas.png");
    camera = create_camera(&CURR_PLACE_ID);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW.\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LESS);

    load_shaders(SHADER_FPATH, 1);
    world = create_world(global_texture, camera);
    mesh_world(world);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float prevTime = glfwGetTime();
    int frames = 0;
    //glfwSwapInterval(0);

    glfwSetInputMode(window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetCursorPosCallback(window->window, mouse_callback);
    glfwSetMouseButtonCallback(window->window, mouse_click_callback);

    do{
        glClearColor(0.2f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera_view(camera);
        create_projection(camera);
        render_world(world);
        // process input
        float current_frame = glfwGetTime();
        frames++;
        if (current_frame - prevTime >= 1.0f) {
            printf("FPS: %d\n", frames);
            frames = 0;
            prevTime = glfwGetTime();
        }
        deltaTime = current_frame - lastFrame;
        lastFrame = current_frame;
        process_input(window, camera, deltaTime);

        // Swap buffers
        glfwSwapBuffers(window->window);
        glfwPollEvents();
        glFlush();

    } // Check if the ESC key was pressed or the window was closed
    while(glfwWindowShouldClose(window->window) == 0 );
    free(window);
    delete_world(world);
    return 0;
}