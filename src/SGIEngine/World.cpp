/*
 * File:   World.cpp
 * Author: TheArni
 *
 * Created on 8 de marzo de 2015, 16:24
 */

#include "World.h"

#include <rapidjson.h>
#include <document.h>
#include <GL/glew.h>
#include <gtc/type_ptr.hpp>
#include <algorithm>

#include "RenderEngine.h"
#include "Shader.h"
#include "AudioEngine.h"
#include "AudioConfig.h"
#include "definitions.h"
#include "Config.h"
#include "Logger.h"
#include "PhysicsEngine.h"

void World::renderWorld() {
    glGetError();
    RenderEngine::set3D();
    for(Camera* camera : cameras){
        RenderEngine::setCurrentCamera(camera);
        camera->enable();
        if(camera->getRenderingTechnique()->getType() != SHADOW_MAPPING){
            for(std::pair<ShaderProgram*, std::vector<WorldObject*>> p : renderMap){
                RenderEngine::setCurrentShader(p.first);
                if(p.first->hasUniform("cameraPosition")){
                    glUniform3fv(p.first->getUniform("cameraPosition"), 1, glm::value_ptr(camera->position));
                }
                for(WorldObject* o : p.second) {
                    o->render();
                }
            }
        } else {
            for(WorldObject* o : objects){
                o->render();
            }
        }
        camera->disable();
    }

    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        Logger::info << "GL ERROR (world render): " << err << std::endl;
        Logger::info << gluErrorString(err) << std::endl;
    }
}

void World::integratePhysics() {        
    AudioEngine::setListenerPosition(player->position);
    AudioEngine::setListenerOrientation(player->rotation);
    PhysicsEngine::simulate(objects);
}

void World::logicUpdate(){
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    bool shouldInteract = keystate[SDL_SCANCODE_E];
    std::vector<WorldObject *> toRemove;
    for(WorldObject* o : objects){
        o->update();
        //TODO: raytracing'n'stuff
        if(shouldInteract && glm::length(player->position+glm::vec3(0, player->getEyeLevel(), 0) - o->position) < 1.0f){
            o->interact();
        }
        
        if (o->shouldRemove()) {
            toRemove.push_back(o);
        }
    }
    player->update();
    for (WorldObject *o : toRemove) {
        std::vector<WorldObject *> *objs = &renderMap[getShader(o->getShaderRequired())];
        objs->erase(std::remove_if(objs->begin(), objs->end(), [o](WorldObject *obj) { return o == obj; }), objs->end());
        objects.erase(std::remove(objects.begin(), objects.end(), o), objects.end());
        delete o;
        o = NULL;
    }
}

void World::addPointLightSource(PointLight *light){
    pointlights.push_back(light);
}

std::vector<PointLight *>& World::getPointLights(){
    return pointlights;
}

Camera* World::addSpotLightSource(SpotLight light, float fov){
    Camera* cam = new Camera(this, new ShadowMapping(light.shadowMap, SHADOWMAPPINGSHADER), false);
    cam->fov = fov;
    cam->position = light.position;
    cam->useDirection = true;
    cam->direction = light.direction;
    cam->resize(Config::graphics.shadowmapResolution, Config::graphics.shadowmapResolution);
    light.depthBiasVP = glm::mat4(
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f
    ) * cam->getProjectionMatrix() * cam->getViewMatrix();
    addCamera(cam);
    spotlights.push_back(light);
}

std::vector<SpotLight>& World::getSpotLights(){
    return spotlights;
}

bool World::processEvent(SDL_Event event) {
    return player->processSDLEvent(event);
}

bool World::loadFromFile(std::string filename){
    rapidjson::Document doc;
    if(readJsonFile(filename, doc)){
        rapidjson::Value& objects = doc["objects"];
        for (rapidjson::SizeType i = 0; i < objects.Size(); i++) {
            if(!worldObjectExists(objects[i]["type"].GetString())){
                Logger::warning << "World Object \"" << objects[i]["type"].GetString() << "\" doesn't exist, skipping." << std::endl;
                continue;
            }
            WorldObject* o = getWorldObject(objects[i]["type"].GetString())->clone();
            o->initFromJson(this, objects[i]);
            if (objects[i].HasMember("id"))
                idObjectMap[objects[i]["id"].GetString()] = o;
            addObject(o);
        }
        Logger::info << "Level loaded" << std::endl;
    }
}

void World::registerPlayer(ControllableEntity *entity) {
    player = entity;
}

ControllableEntity* World::getPlayer(){
    return player;
}

void World::addObject(WorldObject* object){
    ShaderProgram* shader = getShader(object->getShaderRequired());
    renderMap[shader].push_back(object);
    objects.push_back(object);
}

void World::addCamera(Camera* camera){
    cameras.push_back(camera);
}

WorldObject *World::getWorldObjectById(std::string id) {
    return idObjectMap.at(id);
}
