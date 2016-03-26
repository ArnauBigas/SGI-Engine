/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PhysicsEngine.h
 * Author: arnaubigas
 *
 * Created on February 14, 2016, 10:05 PM
 */

#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <vector>
#include "WorldObject.h"

namespace PhysicsEngine {
    
    void simulate(std::vector<WorldObject*> &objects);
    
    void collisionDetection(std::vector<WorldObject*> &objects);
    
    void integrate(std::vector<WorldObject*> &objects);
};

#endif /* PHYSICSENGINE_H */

