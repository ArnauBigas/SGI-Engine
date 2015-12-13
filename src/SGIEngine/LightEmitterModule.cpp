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
#include <iostream>

void LightEmitterModule::loadConfig(rapidjson::Value& json){
    type = json["type"].GetString();
}

void LightEmitterModule::loadModule(WorldObject* prop, World* world, rapidjson::Value& json){
    BaseObjectModule::loadModule(prop, world, json);
    if(type == "pointlight"){
        PointLight light;
        light.color = getVec3(json["color"]);
        light.constantAttenuation = (float) json["constantAttenuation"].GetDouble();
        light.linearAttenuation = (float) json["linearAttenuation"].GetDouble();
        light.exponentialAttenuation = (float) json["exponentialAttenuation"].GetDouble();
        light.position = prop->position;
        world->addPointLightSource(light);
    }
}
