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

//TODO: enhance this class, make it nicer
class Prop : public BaseWorldObject<Prop> {
public:
    Prop(std::string);
    
    Prop(const Prop& orig);

    virtual void render();
    
    virtual void update();
    
    virtual void interact();
    
    std::vector<PropModule*> modules;
private:
    Model* model;    
};

#endif	/* PROP_H */

