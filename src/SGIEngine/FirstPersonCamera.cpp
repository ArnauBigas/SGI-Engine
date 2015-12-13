
/* 
 * File:   FirstPersonCamera.cpp
 * Author: arnau
 * 
 * Created on 12 / de setembre / 2015, 11:47
 */

#include "FirstPersonCamera.h"

#include <gtc/matrix_transform.hpp>

#include "Config.h"

FirstPersonCamera::FirstPersonCamera(World* world, RenderingTechnique* technique) : Camera(world, technique){
}

void FirstPersonCamera::enable(){
    ControllableEntity* entity = world->getPlayer();
    this->pitch = entity->getCameraPitch();
    this->yaw = entity->getCameraYaw();
    this->position = glm::vec3(entity->position.x, entity->position.y + entity->getEyeLevel(), entity->position.z);
    Camera::enable();
}
