/* 
 * File:   SphereCollider.h
 * Author: arnau
 *
 * Created on 18 / d’agost / 2015, 19:28
 */

#ifndef SPHERECOLLIDER_H
#define	SPHERECOLLIDER_H

#include "Collider.h"

class SphereCollider : public Collider {
public:

    SphereCollider(glm::vec3 offset, float radius) : offset(offset), radius(radius) { };

    virtual Collider* generate(WorldObject* obj);

    virtual CollisionData collide(Collider* other);

    virtual ColliderType getType() {
        return SPHERE;
    }

    glm::vec3 getCenter();

    float getRadius() {
        return radius;
    }
    
private:
    glm::vec3 offset;
    float radius;
};

#endif	/* SPHERECOLLIDER_H */

