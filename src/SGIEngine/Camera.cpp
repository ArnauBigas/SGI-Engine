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
    this->fov_ = Config::graphics.fov;
    this->near_ = Config::graphics.nearPlaneClipping;
    this->far_ = Config::graphics.renderDistance;
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
    } else {
        RenderEngine::set2D();
    }
    RenderEngine::setProjectionMatrix(getProjectionMatrix());
    RenderEngine::setViewMatrix(getViewMatrix());
    glViewport(viewport.x*w, viewport.y*h, viewport.w*w, viewport.h*h);
    glScissor(viewport.x*w, viewport.y*h, viewport.w*w, viewport.h*h);
    technique->enable(this);
}

glm::mat4 Camera::getProjectionMatrix(){
    return mode == CAMERA_3D ? glm::perspective((float)glm::radians(fov_), (w*viewport.w)/(h*viewport.h), near_, far_) : glm::ortho(0.0f, (float)w, 0.0f, (float)h);
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
    glm::mat4 view;
    if(useDirection){
        view = glm::lookAt(position,
            position + direction,
            glm::vec3(sin(glm::radians(roll)),cos(glm::radians(roll)),0));
    } else {
        view = glm::lookAt(position,
            position + glm::vec3(
            cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
            sin(glm::radians(pitch)),
            cos(glm::radians(pitch)) * sin(glm::radians(yaw))
            ),
            glm::vec3(sin(glm::radians(roll)),cos(glm::radians(roll)),0));
    }
    return view;
}

RenderingTechnique* Camera::getRenderingTechnique(){
    return technique;
}