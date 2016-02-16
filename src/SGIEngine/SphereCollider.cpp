/* 
 * File:   SphereCollider.cpp
 * Author: arnau
 * 
 * Created on 18 / d’agost / 2015, 19:28
 */

#include "SphereCollider.h"
#include "WorldObject.h"

Collider* SphereCollider::generate(WorldObject* obj) {
    SphereCollider* result = new SphereCollider(offset, radius);
    result->obj = obj;
    return result;
}

glm::vec3 SphereCollider::getCenter() {
    return obj->position + offset;
}

CollisionData SphereCollider::collide(Collider* other) {
    CollisionData data;
    if(other == NULL || other->getObject() == NULL || obj == NULL){
        data.collided = false;
        return data;
    }else if (other->getType() == SPHERE) {
        SphereCollider* sphere = (SphereCollider*) other;
        float radDistance = sphere->getRadius() + radius;
        glm::vec3 dir = sphere->getCenter() - getCenter();
        float posDistance = glm::length(dir);
        dir /= posDistance;
        float distance = posDistance - radDistance;
        data.collided = distance < 0;
        data.direction = dir * distance;
        return data;
    }
}
