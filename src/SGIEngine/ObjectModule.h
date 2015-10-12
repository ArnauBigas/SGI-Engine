/* 
 * File:   PropModule.h
 * Author: arnau
 *
 * Created on 9 / de setembre / 2015, 21:37
 */

#ifndef OBJECTMODULE_H
#define	OBJECTMODULE_H

#include <document.h>

class WorldObject;
class World;

class ObjectModule{
public:    
    //WARNING: MASTER OBJECT ONLY, NOT FINAL OBJECT
    virtual void loadConfig(WorldObject* obj, rapidjson::Value& json){};
    virtual void loadModule(WorldObject* obj, World* world, rapidjson::Value& json);
    virtual void saveModule(rapidjson::Value& json){};
    virtual void update(){};
    virtual void onInteract(){};
    virtual void onRender(){};
    virtual ObjectModule* clone() = 0;
    virtual std::string getName(){return "none";};
    
protected:
    WorldObject* worldobj;
    World* world;
    
private:
    std::string name;
};

template<class Derived>
class BaseObjectModule : public ObjectModule{
    virtual ObjectModule* clone(){
        return new Derived(static_cast<const Derived&>(*this));
    }
};

void addObjectModule(std::string name, ObjectModule* module);

ObjectModule* getObjectModule(std::string name);

#endif	/* OBJECTMODULE_H */

