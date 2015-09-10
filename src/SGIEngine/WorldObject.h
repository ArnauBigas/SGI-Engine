/*
 * File:   WorldObject.h
 * Author: TheArni
 *
 * Created on 11 de abril de 2015, 23:58
 */

#ifndef WORLDOBJECT_H
#define	WORLDOBJECT_H

#include <vec3.hpp>
#include "Collider.h"
#include "RenderEngine.h"

class WorldObject {
public:

    virtual ~WorldObject() {
        delete collider;
    }

    virtual void render(RenderPass pass) = 0;

    virtual Collider* getCollider() = 0;

    glm::vec3 getPosition() {
        return position;
    }

    void setPosition(glm::vec3 pos) {
        position = pos;
    }

    glm::vec3 getRotation() {
        return rotation;
    }

    void setRotation(glm::vec3 rot) {
        rotation = rot;
    }

    glm::vec3 getVelocity() {
        return velocity;
    }

    void setVelocity(glm::vec3 vel) {
        velocity = vel;
    }

    float getMass() {
        return mass;
    }

    virtual bool shouldIntegrate() {
        return true;
    }
    
    virtual void update() = 0;
    
    virtual void interact() = 0;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 rotation;
protected:
    float mass;
    Collider* collider;
};

#endif	/* WORLDOBJECT_H */

