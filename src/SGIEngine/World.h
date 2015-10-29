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
    virtual void addLightSource(PointLight light);
    virtual ControllableEntity* getPlayer();
protected:
    std::map<ShaderProgram*, std::vector<WorldObject*> > renderMap;
    std::vector<WorldObject*> objects;
    std::vector<Camera*> cameras;
    std::vector<PointLight> lights;
    ControllableEntity* player;
};

#endif	/* WORLD_H */

