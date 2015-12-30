/* 
 * File:   Collider.h
 * Author: arnau
 *
 * Created on 18 / d’agost / 2015, 19:20
 */

#ifndef COLLIDER_H
#define	COLLIDER_H

#include <vec3.hpp>

enum ColliderType {
    TRIG, SPHERE
};

struct CollisionData {
    bool collided;
    glm::vec3 direction;
};

class Collider {
public:
    virtual Collider* copy() = 0;

    virtual CollisionData collide(Collider* other) = 0;

    virtual ColliderType getType() = 0;
};

#endif	/* COLLIDER_H */

