/* 
 * File:   ControllableEntity.h
 * Author: arnau
 *
 * Created on 11 / de setembre / 2015, 20:54
 */

#ifndef CONTROLLABLEENTITY_H
#define	CONTROLLABLEENTITY_H

#include "Entity.h"

#include <SDL_events.h>

#define xTrig(x, z, y) (cos(glm::radians(y))*x + sin(glm::radians(-y))*z)
#define zTrig(x, z, y) (sin(glm::radians(y))*x + cos(glm::radians(-y))*z)

class ControllableEntity : public Entity {
public:
    ControllableEntity(float speed);
    
    virtual bool processSDLEvent(SDL_Event& event);
    
    virtual void update();
    
protected:
    float speed = 1.0f;
    float motionX = 0;
    float motionY = 0;
    float motionZ = 0;

};

#endif	/* CONTROLLABLEENTITY_H */

