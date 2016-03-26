/* 
 * File:   Camera.h
 * Author: arnau
 *
 * Created on 11 / de setembre / 2015, 19:04
 */

#ifndef CAMERA_H
#define	CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "RenderingTechnique.h"

class World;

enum CameraMode {
    CAMERA_2D, CAMERA_3D
};

class Camera {
    friend class RenderingTechnique;
public:
    Camera(World* world, RenderingTechnique* technique, bool screen = true);
    ~Camera();
    virtual void enable();
    virtual void disable();
    RenderingTechnique* getRenderingTechnique();

    void resize(int w, int h);
    void setViewport(float x, float y, float w, float h);

    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 direction = glm::vec3(0, 0, 0);
    bool useDirection = false;
    float pitch = 0;
    float yaw = 0;
    float roll = 0;
    World* world;

    CameraMode mode;
    float fov, far, near;
    virtual glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
private:
    unsigned int target;
    unsigned int clearBuffers;
    RenderingTechnique* technique;

    struct Viewport {
        float x, y, w, h;
    } viewport;
    int w, h;
};

#endif	/* CAMERA_H */

