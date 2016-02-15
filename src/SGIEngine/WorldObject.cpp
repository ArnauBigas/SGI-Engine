#include "WorldObject.h"
#include "SphereCollider.h"

#include <map>
#include <iostream>

WorldObject::WorldObject(const WorldObject& other) {
    this->shader = other.shader;
    this->physics = other.physics;
    if (other.collider != 0 && other.collider->getType() == SPHERE) {
        this->collider = new SphereCollider(position, ((SphereCollider*) other.collider)->getRadius());
    }
    for(ObjectModule* mod : other.modules){
        this->modules.push_back(mod->clone());
    }
}

void WorldObject::loadFromJson(rapidjson::Value& json){
    mass = json["mass"].GetDouble();
    std::string colliderType = json["collider"]["type"].GetString();
    if (colliderType == "sphere") {
        collider = new SphereCollider(position, (float) json["collider"]["radius"].GetDouble());
    }
    shader = json["shader"].GetString();
    physics = json["physics"].GetBool();
    rapidjson::Value& mdls = json["modules"];
    for (rapidjson::SizeType i = 0; i < mdls.Size(); i++) {
        ObjectModule* module = getObjectModule(mdls[i]["name"].GetString())->clone();
        module->loadConfig(this, mdls[i]);
        this->modules.push_back(module);
    }
}

void WorldObject::update(){
    for(ObjectModule* mod : modules){
        mod->update();
    }
}
    
void WorldObject::interact(){
    for(ObjectModule* mod : modules){
        mod->onInteract();
    }
}

void WorldObject::initFromJson(World* world, rapidjson::Value& json){
    position = getVec3(json["position"]);
    rotation = getVec3(json["rotation"]);
    if (json.HasMember("physics")) physics = json["physics"].GetBool();
    this->world = world;
    for(ObjectModule* m : modules){
        m->loadModule(this, world, json["modules"][m->getName().c_str()]);
    }
}

void WorldObject::render(){
    for(ObjectModule* m : modules){
        m->onRender();
    }
}

std::map<std::string, WorldObject*> objects;

void registerWorldObject(std::string name, WorldObject* object){
    objects.insert(std::make_pair(name, object));
}

WorldObject* getWorldObject(std::string name){
    return objects.at(name);
}