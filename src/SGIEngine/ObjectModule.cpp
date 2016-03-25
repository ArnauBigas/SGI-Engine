#include "ObjectModule.h"

#include <map>

std::map<std::string, ObjectModule*> modules;

void ObjectModule::loadModule(WorldObject* obj, World* world, rapidjson::Value& json){
    this->worldobj = obj;
    this->world = world;
}

void addObjectModule(ObjectModule* module){
    modules.insert(std::make_pair(module->getName(), module));
}

ObjectModule* getObjectModule(std::string name){
    return modules[name];
}

bool hasObjectModule(std::string name){
    return modules.find(name) != modules.end();
}