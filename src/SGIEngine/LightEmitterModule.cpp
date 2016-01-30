/* 
 * File:   LightEmitterModule.cpp
 * Author: arnau
 * 
 * Created on 10 / de setembre / 2015, 11:18
 */

#include "LightEmitterModule.h"

#include "Utility.h"
#include "WorldObject.h"
#include "World.h"
#include "Material.h"
#include <iostream>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <vec3.hpp>
#include "Prop.h"

void LightEmitterModule::loadConfig(WorldObject* obj, rapidjson::Value& json){
    type = json["type"].GetString();
    if(json.HasMember("direction")){
        dir = getVec3(json["direction"]);
    }
    if(json.HasMember("emissiveMesh") && json.HasMember("emissivePolygroup")){
        polyGroup = &(static_cast<Prop*> (obj)->model->getMesh(json["emissiveMesh"].GetString()).getPolyGroup(json["emissivePolygroup"].GetInt()));
    }
}

void LightEmitterModule::onRender() {
    if(polyGroup != 0){
        polyGroup->material.diffuse = color; 
    }
}

void LightEmitterModule::loadModule(WorldObject* prop, World* world, rapidjson::Value& json){
    BaseObjectModule::loadModule(prop, world, json);
    if(type == "pointlight"){
        PointLight *light = new PointLight();
        color = glm::vec4(getVec3(json["color"]), 1.0f);
        light->color = glm::vec3(color);
        light->constantAttenuation = (float) json["constantAttenuation"].GetDouble();
        light->linearAttenuation = (float) json["linearAttenuation"].GetDouble();
        light->exponentialAttenuation = (float) json["exponentialAttenuation"].GetDouble();
        light->position = prop->position;
        world->addPointLightSource(light);
    } else if(type == "spotlight"){
        SpotLight light;
        color = glm::vec4(getVec3(json["color"]), 1.0f);
        light.color = glm::vec3(color);
        light.coneAngle = cos(glm::radians(json["coneAngle"].GetDouble()/2.0f));
        glm::mat4 rot = glm::mat4(1.0f);
        rot = glm::rotate(rot, glm::radians(prop->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        rot = glm::rotate(rot, glm::radians(prop->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        rot = glm::rotate(rot, glm::radians(prop->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        light.direction = glm::vec3(rot * glm::vec4(dir,0.0f));
        light.constantAttenuation = (float) json["constantAttenuation"].GetDouble();
        light.linearAttenuation = (float) json["linearAttenuation"].GetDouble();
        light.exponentialAttenuation = (float) json["exponentialAttenuation"].GetDouble();
        light.position = prop->position;
        world->addSpotLightSource(light, json["coneAngle"].GetDouble());
        
    }
}
