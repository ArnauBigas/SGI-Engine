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

namespace PhysicsEngine {
    
    void simulate(std::vector<WorldObject*> &objects){
        collisionDetection(objects);
        integrate(objects);
    }
    
    void collisionDetection(std::vector<WorldObject*> &objects){
        for(int i = 0; i < objects.size(); i++){
            WorldObject* objA = objects[i];
            if(objA->position.y <= 0){
                objA->position.y = 0;
                objA->applyForce(glm::vec3(0.f, -objA->getForce().y * objA->bounciness, 0.f));
                objA->applyMomentum(glm::vec3(0.f, -objA->getMomentum().y * objA->bounciness, 0.f));
            }
            if(objA->getCollider() == NULL) continue;
            for(int j = i+1; j < objects.size(); j++){
                WorldObject* objB = objects[j];
                if(objB->getCollider() == NULL) continue;
                CollisionData data = objA->getCollider()->collide(objB->getCollider());
                if(data.collided){
                    glm::vec3 aforce = objA->getForce() * data.direction;
                    glm::vec3 bforce = objB->getForce() * data.direction;
                    objA->applyForce(bforce);
                    objB->applyForce(aforce);
                    glm::vec3 amomentum = objA->getMomentum() * data.direction;
                    glm::vec3 bmomentum = objB->getMomentum() * data.direction;
                    objA->applyMomentum(bmomentum);
                    objB->applyMomentum(amomentum);
                    Logger::info << "woo got a collision m8" << std::endl;
                }
            }
        }
    }
    
    void integrate(std::vector<WorldObject*> &objects){
        float delta = (Game::lastTickTime() / 1000000.f) * Config::physics.scale;
        for(WorldObject* obj : objects){
            //TODO: use r4k
            glm::vec3 a;
            if(obj->shouldIntegrate()){
                a = obj->acceleration + glm::vec3(0.f, -9.8f, 0.f);
            }
            obj->position += (obj->velocity * delta) + (0.5f * a * delta * delta);
            obj->velocity += a;
        }        
    }
};