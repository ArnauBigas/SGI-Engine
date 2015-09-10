/* 
 * File:   PropModule.h
 * Author: arnau
 *
 * Created on 9 / de setembre / 2015, 21:37
 */

#ifndef PROPMODULE_H
#define	PROPMODULE_H

#include <document.h>

#include "WorldObject.h"
#include "World.h"

class PropModule{
public:    
    virtual void loadConfig(rapidjson::Value& json){};
    virtual void loadModule(WorldObject* prop, World* world, rapidjson::Value& json);
    virtual void saveModule(rapidjson::Value& json){};
    virtual void update(){};
    virtual void onInteract(){};
    virtual PropModule* clone() = 0;
    virtual std::string getName(){return "none";};
    
protected:
    WorldObject* prop;
    World* world;
    
private:
    std::string name;
};

template<class Derived>
class BasePropModule : public PropModule{
    virtual PropModule* clone(){
        return new Derived(static_cast<const Derived&>(*this));
    }
};

void addPropModule(std::string name, PropModule* module);

PropModule* getPropModule(std::string name);

#endif	/* PROPMODULE_H */

