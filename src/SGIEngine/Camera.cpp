#include "Camera.h"

#include <geometric.hpp>
#include <gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include "RenderEngine.h"
#include "Config.h"

Camera::Camera(unsigned int target, unsigned int clearBuffers){
    this->target = target;
    this->clearBuffers = clearBuffers;
    resize(Config::graphics.width, Config::graphics.height);
    viewport = {0, 0, 1, 1};
    
    RenderEngine::registerCamera(this);
}

Camera::~Camera() {
    RenderEngine::unregisterCamera(this);
}

void Camera::enable(){
    RenderEngine::setProjectionMatrix(projectionMatrix);
    RenderEngine::setViewMatrix(getViewMatrix());
    setProjectionMatrix(glm::perspective((float)glm::radians(Config::graphics.fov), (w*viewport.w)/(h*viewport.h), (float) Config::graphics.nearPlaneClipping, (float) Config::graphics.renderDistance));
    //TODO 2d support
    //TODO non screen cameras
    glViewport(viewport.x*w, viewport.y*h, viewport.w*w, viewport.h*h);
    glBindFramebuffer(GL_FRAMEBUFFER, target);
    glClear(clearBuffers);
}

void Camera::resize(int w, int h) {
    this->w = w;
    this->h = h;
}

void Camera::setViewport(float x, float y, float w, float h) {
    viewport.x = x;
    viewport.y = y;
    viewport.w = w;
    viewport.h = h;
}

glm::mat4 Camera::getViewMatrix(){
    return glm::lookAt(position,
            position + glm::vec3(
            cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
            sin(glm::radians(pitch)),
            cos(glm::radians(pitch)) * sin(glm::radians(yaw))
            ),
            glm::vec3(sin(glm::radians(roll)),cos(glm::radians(roll)),0));
}

void Camera::setProjectionMatrix(glm::mat4 matrix){
    projectionMatrix = matrix;
}