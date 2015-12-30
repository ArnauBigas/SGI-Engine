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

    SphereCollider(glm::vec3& center, float radius) : center(center), radius(radius) {
    };

    void update(glm::vec3 center) {
        this->center = center;
    }

    virtual Collider* copy() {
        return new SphereCollider(center, radius);
    }

    virtual CollisionData collide(Collider* other);

    virtual ColliderType getType() {
        return SPHERE;
    }

    glm::vec3 getCenter() {
        return center;
    }

    float getRadius() {
        return radius;
    }

private:
    glm::vec3& center;
    float radius;
};

#endif	/* SPHERECOLLIDER_H */

