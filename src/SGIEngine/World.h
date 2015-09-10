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
#include <glm/vec3.hpp>
#include <sdl2/SDL_events.h>

#include "WorldObject.h"
#include "Terrain.h"
#include "Light.h"

class World {
public:
    World();
    World(const World& orig);
    virtual ~World();
    void renderWorld(RenderPass pass);
    void integratePhysics();
    bool loadFromFile(std::string filename);
    bool processEvent(SDL_Event event);
    unsigned int getShadowmapTexture();
    glm::vec3 getCameraPos(){
        return cameraPosition;
    }
    void addLightSource(PointLight light);
private:
    std::vector<WorldObject*> objects;
    glm::vec3 cameraPosition = glm::vec3(0.0f, 2.0f, 0.0f);
    glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 sunDirection = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 sunUp = glm::vec3(1, 1, 1);
    std::vector<PointLight> lights;
    glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 ambientLight = glm::vec3(1, 1, 1);
    bool frwd = false;
    bool back = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool lightMovement = false;
    int dx, dy = 0;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float speed = 0.1f;
    Terrain* terrain;
};

#endif	/* WORLD_H */

