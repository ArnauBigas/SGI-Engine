/* 
 * File:   Wall.cpp
 * Author: arnau
 * 
 * Created on 12 / de setembre / 2015, 17:05
 */

#include "Wall.h"

#include "Texture.h"
#include <iostream>
#include <document.h>
#include <GL/glew.h>
#include <gtx/rotate_vector.hpp>
#include <gtc/type_ptr.hpp>

Wall::Wall(std::string dir) {
    rapidjson::Document doc;
    if(readJsonFile(dir+"wall.json", doc)){
        loadFromJson(doc);
        modelLength = getVec3(doc["modelLength"]);
        model = new Model();
        if(!model->loadCollada(dir + "model.dae")){
            std::cout << "Couldn't load model for this prop!" << std::endl;
        }        
    }
}

//TODO: find a better algorithm
void Wall::render(){
    BaseWorldObject::render();
    glm::vec3 dist = end - position;
    glm::vec3 current = position;
    glm::vec3 oldPos = position;
    while((glm::length(dist) - glm::length(modelLength)) >= 0){
        position = current;
        model->render(getModelMatrix());
        current += modelLength;
        dist -= modelLength;
    }
    position = oldPos;
}

void Wall::initFromJson(World* world, rapidjson::Value& json){
    WorldObject::initFromJson(world, json);
    modelLength = glm::rotateX(modelLength, glm::radians(rotation.x));
    modelLength = glm::rotateY(modelLength, glm::radians(rotation.y));
    modelLength = glm::rotateZ(modelLength, glm::radians(rotation.z));  
    end = getVec3(json["end"]);
}