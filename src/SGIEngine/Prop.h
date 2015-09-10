/*
 * File:   Prop.h
 * Author: TheArni
 *
 * Created on 11 de abril de 2015, 23:57
 */

#ifndef PROP_H
#define	PROP_H

#include <string>
#include <vector>

#include "Model.h"
#include "WorldObject.h"
#include "SphereCollider.h"
#include "PropModule.h"

#include <iostream>

class Prop : public WorldObject {
public:
    Prop(std::string dir);
    Prop(Prop* orig);
    virtual ~Prop();

    virtual Collider* getCollider() {
        if (collider->getType() == SPHERE) {
            SphereCollider* sphere = static_cast<SphereCollider*> (collider);
            sphere->update(getPosition());
        }
        return collider;
    }

    virtual void render(RenderPass pass);
    
    virtual void update();
    
    virtual void interact();
    
    std::vector<PropModule*> modules;
private:
    Model* model;
    unsigned int texture;
    
};

void addProp(std::string name, Prop* prop);

Prop* getProp(std::string name);

#endif	/* PROP_H */

