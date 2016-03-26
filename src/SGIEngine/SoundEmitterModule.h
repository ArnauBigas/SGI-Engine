/* 
 * File:   SoundEmitterModule.h
 * Author: arnau
 *
 * Created on 9 / de setembre / 2015, 22:36
 */

#ifndef SOUNDEMITTERMODULE_H
#define	SOUNDEMITTERMODULE_H

#include "ObjectModule.h"

#include <string>

class SoundEmitterModule : public BaseObjectModule<SoundEmitterModule>{
public:    
    virtual void loadModule(WorldObject* prop, World* world, rapidjson::Value& json);
    virtual void onInteract();
    virtual std::string getName(){return "soundEmitter";};
private:
    std::string audio;
};

#endif	/* SOUNDEMITTERMODULE_H */

