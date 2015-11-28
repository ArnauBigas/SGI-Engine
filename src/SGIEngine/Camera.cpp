#include "Camera.h"

#include <geometric.hpp>
#include <gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include "RenderEngine.h"

Camera::Camera(unsigned int target, unsigned int clearBuffers){
    this->target = target;
    this->clearBuffers = clearBuffers;
}

void Camera::enable(){
    RenderEngine::setProjectionMatrix(projectionMatrix);
    RenderEngine::setViewMatrix(getViewMatrix());
    glBindFramebuffer(GL_FRAMEBUFFER, target);
    glClear(clearBuffers);
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