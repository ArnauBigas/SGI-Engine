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
#include <filestream.h>

#include "ColladaLoader.h"
#include "Texture.h"

Prop::Prop(std::string dir) {    
    texture = loadTextureFromPNG(dir + "texture.png");
    FILE* f;
    f = fopen((dir + "prop.json").c_str(), "r");
    if (f == NULL) {
        std::cout << "Couldn't load the prop file!" << std::endl;
    } else {
        rapidjson::Document doc;
        rapidjson::FileStream is(f);
        doc.ParseStream<0>(is);
        mass = doc["mass"].GetDouble();
        std::string colliderType = doc["collider"]["type"].GetString();
        if (colliderType == "sphere") {
            collider = new SphereCollider(position, (float) doc["collider"]["radius"].GetDouble());
        }
        std::string modelType = doc["modelType"].GetString();
        if(modelType == "wavefront"){
            model = new Model(dir + "model.obj");
        } else if(modelType == "collada"){
            model = loadColladaModel(dir + "model.dae");
        }
        rapidjson::Value& mdls = doc["modules"];
        for (rapidjson::SizeType i = 0; i < mdls.Size(); i++) {
            PropModule* module = getPropModule(mdls[i]["name"].GetString())->clone();
            module->loadConfig(mdls[i]);
            this->modules.push_back(module);
        }
        fclose(f);
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

Prop::Prop(Prop* orig) {
    this->model = orig->model;
    this->texture = orig->texture;
    if (orig->collider->getType() == SPHERE) {
        this->collider = new SphereCollider(position, ((SphereCollider*) orig->collider)->getRadius());
    }
    for(PropModule* mod : orig->modules){
        this->modules.push_back(mod->clone());
    }
}

void Prop::render(RenderPass pass) {
    if (pass == DIFFUSE) {
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    model->render();
}

Prop::~Prop() {
    delete model;
}

std::map<std::string, Prop*> props;

void addProp(std::string name, Prop* prop) {
    props.insert(std::make_pair(name, prop));
}

Prop* getProp(std::string prop) {
    assert(props.find(prop) != props.end());
    return props.at(prop);
}