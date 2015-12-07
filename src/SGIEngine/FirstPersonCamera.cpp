
/* 
 * File:   FirstPersonCamera.cpp
 * Author: arnau
 * 
 * Created on 12 / de setembre / 2015, 11:47
 */

#include "FirstPersonCamera.h"

#include <gtc/matrix_transform.hpp>

#include "Config.h"

FirstPersonCamera::FirstPersonCamera(World* world, unsigned int target, unsigned int clearBuffers) : Camera(target, clearBuffers){
    this->world = world;
//    setProjectionMatrix(glm::perspective(glm::radians((float) Config::graphics.fov), (float) Config::graphics.width / (float) Config::graphics.height, (float) Config::graphics.nearPlaneClipping, (float) Config::graphics.renderDistance));
}

void FirstPersonCamera::enable(){
    ControllableEntity* player = world->getPlayer();
    this->pitch = player->getCameraPitch();
    this->yaw = player->getCameraYaw();
    this->position = glm::vec3(player->position.x, player->position.y + player->getEyeLevel(), player->position.z);
    Camera::enable();
}
