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
#include <vec4.hpp>
#include "Mesh.h"

class LightEmitterModule : public BaseObjectModule<LightEmitterModule>{
public:    
    void loadConfig(WorldObject* obj, rapidjson::Value& json);
    void loadModule(WorldObject* obj, World* world, rapidjson::Value& json);
    void onRender();
    virtual std::string getName(){return "lightEmitter";};
private:
    std::string type;
    glm::vec3 dir;
    glm::vec4 color;
    PolyGroup* polyGroup = 0;
};

#endif	/* LIGHTEMITTERMODULE_H */

