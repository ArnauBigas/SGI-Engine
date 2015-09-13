/* 
 * File:   Wall.cpp
 * Author: arnau
 * 
 * Created on 12 / de setembre / 2015, 17:05
 */

#include "Wall.h"

#include "Texture.h"
#include <iostream>
#include "ColladaLoader.h"
#include <document.h>
#include <GL/glew.h>
#include <gtx/rotate_vector.hpp>
#include <gtc/type_ptr.hpp>

Wall::Wall(std::string dir) {
    texture = loadTextureFromPNG(dir + "texture.png");
    rapidjson::Document doc;
    if(readJsonFile(dir+"wall.json", doc)){
        mass = doc["mass"].GetDouble();
        std::string modelType = doc["modelType"].GetString();
        if(modelType == "wavefront"){
            model = new Model(dir + "model.obj");
        } else if(modelType == "collada"){
            model = loadColladaModel(dir + "model.dae");
        }
        shader = doc["shader"].GetString();
        modelLength = getVec3(doc["modelLength"]);
    }
}

//TODO: find a better algorithm
void Wall::render(){
    glBindTexture(GL_TEXTURE_2D, texture);
    glm::vec3 dist = end - position;
    glm::vec3 current = position;
    glm::vec3 oldPos = position;
    while((glm::length(dist) - glm::length(modelLength)) >= 0){
        position = current;
        RenderEngine::setModelMatrix(getModelMatrix());
        RenderEngine::updateMatrices();
        if(RenderEngine::getCurrentShader()->hasUniform("Model")){
            glUniformMatrix4fv(RenderEngine::getCurrentShader()->getUniform("Model"), 1, GL_FALSE, &getModelMatrix()[0][0]);
        }
        model->render();
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