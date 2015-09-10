#include "PropModule.h"

#include <map>

std::map<std::string, PropModule*> modules;

void PropModule::loadModule(WorldObject* prop, World* world, rapidjson::Value& json){
    this->prop = prop;
    this->world = world;
}

void addPropModule(std::string name, PropModule* module){
    modules.insert(std::make_pair(name, module));
}

PropModule* getPropModule(std::string name){
    return modules.at(name);
}