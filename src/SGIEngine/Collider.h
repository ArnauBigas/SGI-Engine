/* 
 * File:   Collider.h
 * Author: arnau
 *
 * Created on 18 / d’agost / 2015, 19:20
 */

#ifndef COLLIDER_H
#define	COLLIDER_H

#include <glm/vec3.hpp>

enum ColliderType {
    TRIG, SPHERE
};

struct CollisionData {
    bool collided;
    glm::vec3 direction;
};

class WorldObject;

class Collider {
public:
    virtual Collider* generate(WorldObject* object) = 0;

    virtual CollisionData collide(Collider* other) = 0;

    virtual ColliderType getType() = 0;
    
    WorldObject* getObject() { return obj; }
protected:
    WorldObject* obj = 0;
};

#endif	/* COLLIDER_H */

