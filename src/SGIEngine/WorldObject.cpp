#include "WorldObject.h"

#include <map>
#include <iostream>

std::map<std::string, WorldObject*> objects;

void registerWorldObject(std::string name, WorldObject* object){
    objects.insert(std::make_pair(name, object));
}

WorldObject* getWorldObject(std::string name){
    return objects.at(name);
}