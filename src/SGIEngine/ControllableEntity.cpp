/* 
 * File:   ControllableEntity.cpp
 * Author: arnau
 * 
 * Created on 11 / de setembre / 2015, 20:54
 */

#include "ControllableEntity.h"

ControllableEntity::ControllableEntity(float speed) {
    this->speed = speed;
}


//TODO: Mappable controls
bool ControllableEntity::processSDLEvent(SDL_Event& event){
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                motionX = 1;
                break;
            case SDLK_s:
                motionX = -1;
                break;
            case SDLK_a:
                motionZ = -1;
                break;
            case SDLK_d:
                motionZ = 1;
                break;
            case SDLK_SPACE:
                motionY = 1;
                break;
            case SDLK_c:
                motionY = -1;
                break;
            case SDLK_LSHIFT:
                speed = 1.0f;
                break;
            case SDLK_LCTRL:
                speed = 0.01f;
                break;
            default:
                return false;
        }
    } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                motionX = 0;
                break;
            case SDLK_s:
                motionX = 0;
                break;
            case SDLK_a:
                motionZ = 0;
                break;
            case SDLK_d:
                motionZ = 0;
                break;
            case SDLK_SPACE:
                motionY = 0;
                break;
            case SDLK_c:
                motionY = 0;
                break;
            case SDLK_LSHIFT:
                speed = 0.1f;
                break;
            case SDLK_LCTRL:
                speed = 0.1f;
                break;
            default:
                return false;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        cameraYaw += (event.motion.xrel) * 0.1f;
        cameraPitch += (event.motion.yrel) * -0.1f;
        if(cameraPitch > 89.0f)
            cameraPitch =  89.0f;
        else if(cameraPitch < -89.0f)
            cameraPitch = -89.0f;
        if(cameraYaw > 360)
            cameraYaw = 0;
        else if(cameraYaw < 0)
            cameraYaw = 360;
    } else {
        return false;
    }
    return true;
}

//This won't really work when we introduce the physics system. maybe.
void ControllableEntity::update(){
    glm::vec3 front;
    front.x = cos(glm::radians(cameraPitch)) * cos(glm::radians(cameraYaw));
    front.y = sin(glm::radians(cameraPitch));
    front.z = cos(glm::radians(cameraPitch)) * sin(glm::radians(cameraYaw));
    front = glm::normalize(front);
    position += front * motionX * speed;
    position += glm::vec3(sin(glm::radians(-cameraYaw)), 0, cos(glm::radians(-cameraYaw))) * motionZ * speed;
    position += glm::vec3(0,1,0) * motionY * speed;
}