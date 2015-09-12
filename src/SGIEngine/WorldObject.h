/*
 * File:   WorldObject.h
 * Author: TheArni
 *
 * Created on 11 de abril de 2015, 23:58
 */

#ifndef WORLDOBJECT_H
#define	WORLDOBJECT_H

#include <vec3.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <string>
#include <document.h>
#include <iostream>

#include "Collider.h"
#include "RenderEngine.h"
#include "Utility.h"


class WorldObject {
public:

    virtual ~WorldObject() {
        delete collider;
    }

    virtual void loadFromJson(rapidjson::Value& json){
        position = getVec3(json["position"]);
        rotation = getVec3(json["rotation"]);
    }
    
    virtual void render() = 0;
    
    virtual void update() {};
    
    virtual void interact() {};

    //TODO: get collider generation working properly
    virtual Collider* getCollider(){
        
    }
    
    //TODO: handle rotation and scalation
    virtual glm::mat4 getModelMatrix(){
        glm::mat4 modelMatrix(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));        
        return modelMatrix;
    }
    
    virtual WorldObject* clone() = 0; 
    
    virtual bool shouldIntegrate() {return true;}
    
    std::string getShaderRequired() {return shader;}

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 rotation;
    float mass;
protected:
    Collider* collider;
    std::string shader;
};

template<class Derived>
class BaseWorldObject : public WorldObject{
public:
    virtual WorldObject* clone(){
        return new Derived(static_cast<const Derived&>(*this));
    }
};

void registerWorldObject(std::string name, WorldObject* object);

WorldObject* getWorldObject(std::string name);

#endif	/* WORLDOBJECT_H */

