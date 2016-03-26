/* 
 * File:   Prop.cpp
 * Author: TheArni
 * 
 * Created on 11 de abril de 2015, 23:57
 */

#include "Prop.h"

#include <map>
#include <GL/glew.h>
#include <rapidjson/document.h>

#include "Utility.h"
#include "Texture.h"
#include "Logger.h"

Prop::Prop(std::string dir) {
    rapidjson::Document doc;
    if(readJsonFile(dir + "prop.json", doc)){        
        model = new Model();
        if(!model->loadCollada(dir + "model.dae")){
            Logger::info << "Couldn't load model for this prop!" << std::endl;
        }
        loadFromJson(doc);
    }
}

Prop::Prop(const Prop& other) : BaseWorldObject(other) {
        this->model = other.model;
}

void Prop::render() {
    BaseWorldObject::render();
    model->render(getModelMatrix());
}