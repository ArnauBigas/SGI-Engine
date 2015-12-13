/* 
 * File:   Camera.h
 * Author: arnau
 *
 * Created on 11 / de setembre / 2015, 19:04
 */

#ifndef CAMERA_H
#define	CAMERA_H

#include <mat4x4.hpp>
#include <vec3.hpp>

#include "RenderingTechnique.h"

class World;

class Camera {
    friend class RenderingTechnique;
public:
    Camera(World* world, RenderingTechnique* technique);
    ~Camera();
    virtual void enable();
    virtual void disable();
    RenderingTechnique* getRenderingTechnique();
    
    void resize(int w, int h);
    void setViewport(float x, float y, float w, float h);
    
    void setProjectionMatrix(glm::mat4 matrix);
    glm::vec3 position = glm::vec3(0, 0, 0);
    float pitch = 0;
    float yaw = 0;
    float roll = 0;
    World* world;    
protected:
    virtual glm::mat4 getViewMatrix();
private:
    glm::mat4 projectionMatrix;
    unsigned int target;
    unsigned int clearBuffers;
    RenderingTechnique* technique;
    
    struct Viewport {
        float x, y, w, h;
    } viewport;
    int w, h;
};

#endif	/* CAMERA_H */

