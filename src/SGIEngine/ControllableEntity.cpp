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

bool ControllableEntity::processSDLEvent(SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        rotation.y += (event.motion.xrel) * 0.1f;
        rotation.x += (event.motion.yrel) * -0.1f;
        if (rotation.x > 89.9f)
            rotation.x = 89.9f;
        else if (rotation.x < -89.9f)
            rotation.x = -89.9f;
        if (rotation.y > 360)
            rotation.y = 0;
        else if (rotation.y < 0)
            rotation.y = 360;
    } else {
        return false;
    }
    return true;
}

void ControllableEntity::update() {
    motionX = 0;
    motionY = 0;
    motionZ = 0;
    float movSpeed = speed;

    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_W]) motionX++;
    if (keystate[SDL_SCANCODE_S]) motionX--;
    if (keystate[SDL_SCANCODE_A]) motionZ--;
    if (keystate[SDL_SCANCODE_D]) motionZ++;
    if (keystate[SDL_SCANCODE_F] || keystate[SDL_SCANCODE_SPACE]) motionY++;
    if (keystate[SDL_SCANCODE_C]) motionY--;
    if (keystate[SDL_SCANCODE_LSHIFT]) movSpeed *= 2;
    if (keystate[SDL_SCANCODE_LCTRL]) movSpeed /= 2;

    glm::vec3 mov = glm::normalize(glm::vec3(
            xTrig(motionX, motionZ, rotation.y),

            motionY,

            zTrig(motionX, motionZ, rotation.y))) * movSpeed;

    if (glm::isnan(mov.x) || glm::isnan(mov.y) || glm::isnan(mov.z)) mov = glm::vec3(0.f, 0.f, 0.f);

    position += mov;
}