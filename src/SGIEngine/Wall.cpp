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
        modelLength = doc["modelLength"].GetDouble();
        model = new Model();
        if(!model->loadCollada(dir + "model.dae")){
            std::cout << "Couldn't load model for this prop!" << std::endl;
        }        
    }
}

//TODO: find a better algorithm
void Wall::render(){
    BaseWorldObject::render();
    for(glm::mat4 matrix : matrices){
        model->render(matrix);
    }
}

void Wall::initFromJson(World* world, rapidjson::Value& json){
    WorldObject::initFromJson(world, json);
    std::vector<glm::vec3> points;
    rapidjson::Value& pts = json["points"];
    for (rapidjson::SizeType i = 0; i < pts.Size(); i++) {
        points.push_back(getVec3(pts[i]));
    }
    for(int i = 0; i < points.size()-1; i++){
        glm::vec3 dir = points[i+1] - points[i];
        float f = glm::length(dir) / modelLength;
        int segments = floor(f);
        float scale = f / (float) segments;
        //float x = atan2(dir.y, dir.z);
        float x = 0;
        float y = atan2(dir.x, dir.z);
        //float y = 0;
        //float z = atan2(dir.y, dir.x);
        float z = 0;
        for(int j = 0; j < segments; j++){
            glm::mat4 modelMatrix(1.0f);
            modelMatrix = glm::translate(modelMatrix, points[i] + ((dir / (float) segments) * (float) j));
            modelMatrix = glm::rotate(modelMatrix, x, glm::vec3(1.0f, 0.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, y, glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, z, glm::vec3(0.0f, 0.0f, 1.0f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, scale));
            matrices.push_back(modelMatrix);
        }        
    }
}