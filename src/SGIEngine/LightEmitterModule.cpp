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
    type.name = json["type"].GetString();
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
    if(type.name == "pointlight"){
        type.point = new PointLight();
        color = glm::vec4(getVec3(json["color"]), 1.0f);
        type.point->color = glm::vec3(color);
        type.point->constantAttenuation = (float) json["constantAttenuation"].GetDouble();
        type.point->linearAttenuation = (float) json["linearAttenuation"].GetDouble();
        type.point->exponentialAttenuation = (float) json["exponentialAttenuation"].GetDouble();
        type.point->position = prop->position;
        world->addPointLightSource(type.point);
    } else if(type.name == "spotlight"){
        color = glm::vec4(getVec3(json["color"]), 1.0f);
        type.spot.color = glm::vec3(color);
        type.spot.coneAngle = cos(glm::radians(json["coneAngle"].GetDouble()/2.0f));
        glm::mat4 rot = glm::mat4(1.0f);
        rot = glm::rotate(rot, glm::radians(prop->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        rot = glm::rotate(rot, glm::radians(prop->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        rot = glm::rotate(rot, glm::radians(prop->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        type.spot.direction = glm::vec3(rot * glm::vec4(dir,0.0f));
        type.spot.constantAttenuation = (float) json["constantAttenuation"].GetDouble();
        type.spot.linearAttenuation = (float) json["linearAttenuation"].GetDouble();
        type.spot.exponentialAttenuation = (float) json["exponentialAttenuation"].GetDouble();
        type.spot.position = prop->position;
        world->addSpotLightSource(type.spot, json["coneAngle"].GetDouble());
    }
}
