#include "ObjectModule.h"

#include <map>

std::map<std::string, ObjectModule*> modules;

void ObjectModule::loadModule(WorldObject* obj, World* world, rapidjson::Value& json){
    this->worldobj = obj;
    this->world = world;
}

void addObjectModule(std::string name, ObjectModule* module){
    modules.insert(std::make_pair(name, module));
}

ObjectModule* getObjectModule(std::string name){
    return modules.at(name);
}