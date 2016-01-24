/*
 * File:   World.h
 * Author: TheArni
 *
 * Created on 8 de marzo de 2015, 16:24
 */

#ifndef WORLD_H
#define	WORLD_H

#include <string>
#include <vector>
#include <map>
#include <SDL_events.h>

#include "WorldObject.h"
#include "Light.h"
#include "Entity.h"
#include "Camera.h"
#include "ControllableEntity.h"

class World {
public:
    virtual void renderWorld();
    virtual void integratePhysics();
    virtual void logicUpdate();
    virtual bool loadFromFile(std::string filename);
    virtual bool processEvent(SDL_Event event);
    virtual void addObject(WorldObject* object);
    virtual void addCamera(Camera* camera);
    virtual void addPointLightSource(PointLight light);
    virtual Camera* addSpotLightSource(SpotLight light, float fov);
    virtual std::vector<PointLight>& getPointLights();
    virtual std::vector<SpotLight>& getSpotLights();
    virtual ControllableEntity* getPlayer();
    virtual void registerPlayer(ControllableEntity *entity);
protected:
    std::map<ShaderProgram*, std::vector<WorldObject*> > renderMap;
    std::vector<WorldObject*> objects;
    std::vector<Camera*> cameras;
    std::vector<PointLight> pointlights;
    std::vector<SpotLight> spotlights;
    ControllableEntity* player;
    rapidjson::Value& playerData;
};

#endif	/* WORLD_H */

