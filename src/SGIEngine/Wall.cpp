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
        mass = doc["mass"].GetDouble();
        model = new Model();
        if(!model->loadCollada(dir + "model.dae")){
            std::cout << "Couldn't load model for this prop!" << std::endl;
        }
        shader = doc["shader"].GetString();
        modelLength = getVec3(doc["modelLength"]);
    }
}

//TODO: find a better algorithm
void Wall::render(){
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

void Wall::loadFromJson(rapidjson::Value& json){
    WorldObject::loadFromJson(json);
    modelLength = glm::rotateX(modelLength, glm::radians(rotation.x));
    modelLength = glm::rotateY(modelLength, glm::radians(rotation.y));
    modelLength = glm::rotateZ(modelLength, glm::radians(rotation.z));  
    end = getVec3(json["end"]);
}