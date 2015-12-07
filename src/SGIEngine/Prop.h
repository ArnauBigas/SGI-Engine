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
#include "ObjectModule.h"

#include <iostream>

//TODO: enhance this class, make it nicer
class Prop : public BaseWorldObject<Prop> {
public:
    Prop(std::string);
    
    Prop(const Prop& other);

    virtual void render();
    
    virtual void interact();
    
    Model* model;    
};

#endif	/* PROP_H */

