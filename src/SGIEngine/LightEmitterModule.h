/* 
 * File:   LightEmitterModule.h
 * Author: arnau
 *
 * Created on 10 / de setembre / 2015, 11:18
 */

#ifndef LIGHTEMITTERMODULE_H
#define	LIGHTEMITTERMODULE_H

#include "ObjectModule.h"

#include "Light.h"


class LightEmitterModule : public BaseObjectModule<LightEmitterModule>{
public:    
    virtual void loadConfig(rapidjson::Value& json);
    virtual void loadModule(WorldObject* obj, World* world, rapidjson::Value& json);
    virtual std::string getName(){return "lightEmitter";};
private:
    std::string type;
};

#endif	/* LIGHTEMITTERMODULE_H */

