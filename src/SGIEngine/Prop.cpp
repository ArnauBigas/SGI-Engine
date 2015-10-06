/* 
 * File:   Prop.cpp
 * Author: TheArni
 * 
 * Created on 11 de abril de 2015, 23:57
 */

#include "Prop.h"

#include <map>
#include <GL/glew.h>
#include <iostream>
#include <document.h>

#include "Utility.h"
#include "Texture.h"

Prop::Prop(std::string dir) {
    rapidjson::Document doc;
    if(readJsonFile(dir + "prop.json", doc)){
        mass = doc["mass"].GetDouble();
        std::string colliderType = doc["collider"]["type"].GetString();
        if (colliderType == "sphere") {
            collider = new SphereCollider(position, (float) doc["collider"]["radius"].GetDouble());
        }
        std::string modelType = doc["modelType"].GetString();
        model = new Model();
        if(!model->loadCollada(dir + "model.dae")){
            std::cout << "Couldn't load model for this prop!" << std::endl;
        }
        shader = doc["shader"].GetString();
        rapidjson::Value& mdls = doc["modules"];
        for (rapidjson::SizeType i = 0; i < mdls.Size(); i++) {
            PropModule* module = getPropModule(mdls[i]["name"].GetString())->clone();
            module->loadConfig(mdls[i]);
            this->modules.push_back(module);
        }
    }
}

void Prop::update(){
    for(PropModule* mod : modules){
        mod->update();
    }
}

void Prop::interact(){
    for(PropModule* mod : modules){
        mod->onInteract();
    }
}

Prop::Prop(const Prop& orig) {
    this->model = orig.model;
    this->shader = orig.shader;
    if (orig.collider->getType() == SPHERE) {
        this->collider = new SphereCollider(position, ((SphereCollider*) orig.collider)->getRadius());
    }
    for(PropModule* mod : orig.modules){
        this->modules.push_back(mod->clone());
    }
}

void Prop::render() {
    model->render(getModelMatrix());
}