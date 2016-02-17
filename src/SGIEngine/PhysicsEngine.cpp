/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PhysicsEngine.cpp
 * Author: arnaubigas
 * 
 * Created on February 14, 2016, 10:05 PM
 */

#include "PhysicsEngine.h"
#include "Game.h"
#include "Logger.h"
#include "Config.h"
#include "Utility.h"

namespace PhysicsEngine {
    
    void simulate(std::vector<WorldObject*> &objects){
        collisionDetection(objects);
        integrate(objects);
    }
    
    void collisionDetection(std::vector<WorldObject*> &objects){
        for(int i = 0; i < objects.size(); i++){
            WorldObject* objA = objects[i];
            if(objA->position.y <= 0){
                objA->applyForce(glm::vec3(0.f, -objA->getForce().y, 0.f));
                objA->applyMomentum(glm::vec3(0.f, -objA->getMomentum().y, 0.f));
            }
            if(objA->getCollider() == NULL) continue;
            for(int j = i+1; j < objects.size(); j++){
                WorldObject* objB = objects[j];
                if(objB->getCollider() == NULL) continue;
                CollisionData data = objA->getCollider()->collide(objB->getCollider());
                if(data.collided){
                    glm::vec3 normal = glm::normalize(data.direction);
                    Logger::info << "============before=============" << std::endl;
                    Logger::info << printVec3(normal) << std::endl;
                    Logger::info << printVec3(objA->getForce()) << std::endl;
                    Logger::info << printVec3(objB->getForce()) << std::endl;
                    glm::vec3 tangent = glm::cross(normal, glm::vec3(0.f, 1.f, 0.f));
                    glm::vec3 va = objA->velocity;
                    glm::vec3 vb = objB->velocity;
                    objA->velocity = glm::reflect((va * (objA->mass - objB->mass) + 2 * objB->mass * vb) / (objA->mass + objB->mass), tangent);
                    objB->velocity = glm::reflect((vb * (objB->mass - objA->mass) + 2 * objA->mass * va) / (objA->mass + objB->mass), tangent);
                    glm::vec3 fa = objA->getForce() * normal;
                    glm::vec3 fb = objB->getForce() * normal;
                    objA->applyForce(-fa);
                    objA->applyForce(fb);
                    objB->applyForce(fa);
                    objB->applyForce(-fb);
                    glm::vec3 correction = (std::max(glm::length(data.direction)-((float) Config::physics.correctionThreshold), 0.0f)) * ((float) Config::physics.correctionPercentage) * normal;
                    objA->position += correction * objA->mass;
                    objB->position -= correction * objB->mass;
                    Logger::info << "============after=============" << std::endl;
                    Logger::info << printVec3(normal) << std::endl;
                    Logger::info << printVec3(objA->getForce()) << std::endl;
                    Logger::info << printVec3(objB->getForce()) << std::endl;
                }
            }
        }
    }
    
    void integrate(std::vector<WorldObject*> &objects){
        float delta = (Game::lastTickTime() / 1000000.f) * Config::physics.scale;
        for(WorldObject* obj : objects){
            //TODO: use r4k
            if(obj->shouldIntegrate()){
                obj->position += (obj->velocity * delta) + (0.5f * obj->acceleration * delta * delta);
                obj->velocity += obj->acceleration;
                obj->acceleration = glm::vec3(0.f, -Config::physics.gravity, 0.f);
                if(obj->position.y <= 0){
                    obj->position.y = 0;
                }
            }
        }        
    }
};