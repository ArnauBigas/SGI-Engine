/* 
 * File:   SphereCollider.cpp
 * Author: arnau
 * 
 * Created on 18 / d’agost / 2015, 19:28
 */

#include "SphereCollider.h"

CollisionData SphereCollider::collide(Collider* other) {
    if (other->getType() == SPHERE) {
        CollisionData data;
        SphereCollider* sphere = (SphereCollider*) other;
        float radDistance = sphere->getRadius() + radius;
        glm::vec3 dir = sphere->getCenter() - center;
        float posDistance = dir.length();
        dir /= posDistance;
        float distance = posDistance - radDistance;
        data.collided = distance < 0;
        data.direction = dir * distance;
        return data;
    }
}
