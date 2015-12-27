#include "Camera.h"

#include <geometric.hpp>
#include <gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include "RenderEngine.h"
#include "Config.h"

Camera::Camera(World* world, RenderingTechnique* technique, bool screen){
    this->technique = technique;
    this->world = world;
    resize(Config::graphics.width, Config::graphics.height);
    viewport = {0, 0, 1, 1};
    
    this->mode = CAMERA_3D;
    this->fov = Config::graphics.fov;
    this->near = Config::graphics.nearPlaneClipping;
    this->far = Config::graphics.renderDistance;
    if (screen) {
        RenderEngine::registerCamera(this);
    }
}

Camera::~Camera() {
    RenderEngine::unregisterCamera(this);
    delete technique;
}

void Camera::enable(){
    if (mode == CAMERA_3D) {
        RenderEngine::set3D();
        RenderEngine::setProjectionMatrix(glm::perspective((float)glm::radians(fov), (w*viewport.w)/(h*viewport.h), near, far));
    } else {
        RenderEngine::set2D();
        RenderEngine::setProjectionMatrix(glm::ortho(0, w, 0, h));
    }
    RenderEngine::setViewMatrix(getViewMatrix());
    glViewport(viewport.x*w, viewport.y*h, viewport.w*w, viewport.h*h);
    glScissor(viewport.x*w, viewport.y*h, viewport.w*w, viewport.h*h);
    technique->enable(this);
}

void Camera::disable(){
    technique->disable(this);
}

void Camera::resize(int w, int h) {
    this->w = w;
    this->h = h;
    technique->targetResized(w, h);
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

RenderingTechnique* Camera::getRenderingTechnique(){
    return technique;
}