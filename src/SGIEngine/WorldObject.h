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
#include "ObjectModule.h"
#include "Config.h"


class WorldObject {
public:
    WorldObject(){};

    WorldObject(const WorldObject& other);
    
    virtual ~WorldObject() {
        delete collider;
    }
    
    virtual void initFromJson(World* world, rapidjson::Value& json);
    
    virtual void render();
    
    virtual void update();
    
    virtual void interact();

    virtual Collider* getCollider() { return collider;};
    
    virtual glm::mat4 getModelMatrix(){
        glm::mat4 modelMatrix(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));        
        return modelMatrix;
    }
    
    virtual WorldObject* clone() = 0; 
    
    virtual bool shouldIntegrate() {return physics;}
    
    std::string getShaderRequired() {return shader;}
    
    bool shouldRemove() {return remove;}
    
    glm::vec3 getForce() {return acceleration * mass;}
    
    glm::vec3 getMomentum() {return velocity * mass;}
    
    void applyForce(glm::vec3 force) { acceleration += force / mass;}
    
    void applyMomentum(glm::vec3 momentum) { velocity += momentum / mass;}

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 rotation;
    float mass;
    float bounciness = 0.75f;
    
    bool physics;
protected:
    void loadFromJson(rapidjson::Value& json);
    
    std::vector<ObjectModule*> modules;
    Collider* collider = 0;
    std::string shader;
    World* world = 0;
    bool remove = false;
};

template<class Derived>
class BaseWorldObject : public WorldObject{
public:
    BaseWorldObject(){};
    
    BaseWorldObject(const BaseWorldObject& other) : WorldObject(other) {};
    
    virtual WorldObject* clone(){
        return new Derived(static_cast<const Derived&>(*this));
    }
};

void registerWorldObject(std::string name, WorldObject* object);

WorldObject* getWorldObject(std::string name);

#endif	/* WORLDOBJECT_H */

