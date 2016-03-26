/* 
 * File:   Entity.h
 * Author: arnau
 *
 * Created on 11 / de setembre / 2015, 20:46
 */

#ifndef ENTITY_H
#define	ENTITY_H

#include "WorldObject.h"

//TODO: make an actual entity class
class Entity : public BaseWorldObject<Entity> {
public:
    virtual void render();
    
    virtual void update();
    
    virtual void interact();
    
    float getEyeLevel() {return eyeLevel;}
    
    float getCameraPitch() {return cameraPitch;}
    
    float getCameraYaw() {return cameraYaw;}
protected:
    float eyeLevel = 1.75f;
    float cameraPitch = 0;
    float cameraYaw = 0;
};

#endif	/* ENTITY_H */

