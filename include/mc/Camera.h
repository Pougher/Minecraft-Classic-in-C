#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>

#include "../cglm/vec3.h"
#include "../cglm/cam.h"
#include "../cglm/mat4.h"
#include "Render.h"
#include "BlockId.h"

typedef struct {
    vec3 camera_pos;
    vec3 camera_front;
    vec3 camera_up;
    vec3 direction;
    mat4 view;
    mat4 proj;

    float pitch;
    float yaw;
    float lastX;
    float lastY;
    float fov;
    
    float dfromcam;
    uint8_t zoom;
    uint8_t cnorm;
    uint8_t polygon;

    BlockID_t* id;
}Camera;

Camera* create_camera(BlockID_t* id)
{
    Camera* cam = (Camera*) malloc(sizeof(Camera));
    glm_vec3_zero(cam->camera_pos);
    glm_vec3_zero(cam->camera_front);
    glm_vec3_zero(cam->camera_up);
    cam->camera_pos[1] = 10.0f;
    cam->camera_front[2] = -1.0f;
    cam->camera_up[1] = 1.0f;

    cam->fov = 70.0f;
    cam->pitch = 0.0f;
    cam->yaw = -90.0f;
    cam->lastX = 1280.0 / 2.0;
    cam->lastY = 720 / 2.0;
    cam->zoom = 0;
    cam->cnorm = 0;
    cam->polygon = 0;
    cam->id = id;
    return cam;
}

void camera_view(Camera* cam)
{
    vec3 vec_temp;
    glm_vec3_add(cam->camera_pos, cam->camera_front, vec_temp);
    glm_lookat(cam->camera_pos, vec_temp, cam->camera_up, cam->view);
}

void create_projection(Camera* cam)
{
    glm_perspective(glm_rad(cam->fov), 1.77777777f, 0.1f, 500.0f, cam->proj);
}

inline void mul_vec_and_add(vec3 vec1, vec3 vec2, const float fl)
{
    vec3 cvec;
    vec3 rvec;
    cvec[0] = fl; cvec[1] = fl; cvec[2] = fl;
    glm_vec3_mul(vec2, cvec, rvec);
    glm_vec3_add(vec1, rvec, vec1);
}

inline void mul_vec_and_sub(vec3 vec_1, vec3 vec_2, const float fl)
{
    vec3 cvec;
    vec3 rvec;
    cvec[0] = fl; cvec[1] = fl; cvec[2] = fl;
    glm_vec3_mul(vec_2, cvec, rvec);
    glm_vec3_sub(vec_1, rvec, vec_1);
}

inline void add_norm_cross_mul(vec3 vec_1, vec3 vec_2, vec3 vec_3, const float fl)
{
    
    vec3 cvec;
    vec3 rvec;
    cvec[0] = fl; cvec[1] = fl; cvec[2] = fl;
    glm_vec3_cross(vec_1, vec_2, rvec);
    glm_normalize(rvec);
    glm_vec3_mul(rvec, cvec, rvec);
    glm_vec3_add(rvec, vec_3, vec_3);
}

inline void sub_norm_cross_mul(vec3 vec_1, vec3 vec_2, vec3 vec_3, const float fl)
{   
    vec3 cvec;
    vec3 rvec;
    cvec[0] = fl; cvec[1] = fl; cvec[2] = fl;
    glm_vec3_cross(vec_1, vec_2, rvec);
    glm_normalize(rvec);
    glm_vec3_mul(rvec, cvec, rvec);
    glm_vec3_sub(vec_3, rvec, vec_3);
}

void process_input(Window* window, Camera* cam, float delta_time)
{
    const float cameraSpeed = 4.5f * delta_time; // adjust accordingly
    if (glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS)
        mul_vec_and_add(cam->camera_pos, cam->camera_front, cameraSpeed);
    if (glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS)
        mul_vec_and_sub(cam->camera_pos, cam->camera_front, cameraSpeed);
    if (glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS)
        sub_norm_cross_mul(cam->camera_front, cam->camera_up, cam->camera_pos, cameraSpeed);
    if (glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS)
        add_norm_cross_mul(cam->camera_front, cam->camera_up, cam->camera_pos, cameraSpeed);
    if (glfwGetKey(window->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam->camera_pos[1] -= cameraSpeed;
    if (glfwGetKey(window->window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam->camera_pos[1] += cameraSpeed;
    if (glfwGetKey(window->window, GLFW_KEY_C) == GLFW_PRESS) {
        cam->fov = 10.0f;
        cam->zoom = 1;
    }
    if (glfwGetKey(window->window, GLFW_KEY_C) != GLFW_PRESS && cam->zoom) {
        cam->fov = 70.0f;
        cam->zoom = 0;
    }
    if (glfwGetKey(window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        switch(cam->cnorm){
            case 0:
                glfwSetInputMode(window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cam->cnorm = 1;
                break;
            case 1:
                glfwSetInputMode(window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                cam->cnorm = 0;
                break;
        }
    }
    if (glfwGetKey(window->window, GLFW_KEY_G) == GLFW_PRESS) {
        switch(cam->polygon){
            case 0:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                cam->polygon = 1;
                break;
            case 1:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                cam->polygon = 0;
                break;
        }
    }
    if (glfwGetKey(window->window, GLFW_KEY_0) == GLFW_PRESS) *cam->id = B_GRASS;
    if (glfwGetKey(window->window, GLFW_KEY_1) == GLFW_PRESS) *cam->id = B_DIRT;
    if (glfwGetKey(window->window, GLFW_KEY_2) == GLFW_PRESS) *cam->id = B_STONE;
    if (glfwGetKey(window->window, GLFW_KEY_3) == GLFW_PRESS) *cam->id = B_WATER;
    if (glfwGetKey(window->window, GLFW_KEY_4) == GLFW_PRESS) *cam->id = B_SAND;
    if (glfwGetKey(window->window, GLFW_KEY_5) == GLFW_PRESS) *cam->id = B_WOOD;
    if (glfwGetKey(window->window, GLFW_KEY_6) == GLFW_PRESS) *cam->id = B_LEAF;
    if (glfwGetKey(window->window, GLFW_KEY_7) == GLFW_PRESS) *cam->id = B_PLANK;
}

#endif