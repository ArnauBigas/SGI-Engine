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
#include <iostream>
#include <gtc/type_ptr.hpp>

#include "RenderEngine.h"
#include "Shader.h"
#include "AudioEngine.h"
#include "AudioConfig.h"

void World::renderWorld() {
    glGetError();
    RenderEngine::set3D();
    for(Camera* camera : cameras){
        camera->enable();
        for(std::pair<ShaderProgram*, std::vector<WorldObject*>> p : renderMap){
            RenderEngine::setCurrentShader(p.first);
            if(p.first->hasUniform("cameraPosition")){
                glUniform3fv(p.first->getUniform("cameraPosition"), 1, glm::value_ptr(camera->position));
            }
            for(WorldObject* o : p.second){
                o->render();
            }
        }
        camera->disable();
    }

    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "GL ERROR (world render): " << err << std::endl;
        std::cout << gluErrorString(err) << std::endl;
    }
}

void World::integratePhysics() {        
    AudioEngine::setListenerPosition(player->position);
    AudioEngine::setListenerOrientation(player->rotation);
}

void World::logicUpdate(){
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    bool shouldInteract = keystate[SDL_SCANCODE_E];
    for(WorldObject* o : objects){
        o->update();
        //TODO: raytracing'n'stuff
        if(shouldInteract && glm::length(player->position+glm::vec3(0, player->getEyeLevel(), 0) - o->position) < 1.0f){
            o->interact();
        }
    }
    player->update();
}

void World::addPointLightSource(PointLight light){
    pointlights.push_back(light);
}

std::vector<PointLight>& World::getPointLights(){
    return pointlights;
}

void World::addSpotLightSource(SpotLight light){
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
            //std::cout << "loading object of type " << objects[i]["type"].GetString() << std::endl;
            WorldObject* o = getWorldObject(objects[i]["type"].GetString())->clone();
            o->initFromJson(this, objects[i]);
            addObject(o);
            //std::cout << "object loaded" << std::endl;
        }
        player = new ControllableEntity((float) doc["player"]["speed"].GetDouble());
        player->initFromJson(this, doc["player"]);
        //test
        PointLight light;
        light.position = glm::vec3(10.0f, 2.5f, 10.0f);
        light.color = glm::vec3(1.0f, 0.0f, 0.0f);
        light.constantAttenuation = 0.1f;
        light.linearAttenuation = 0.05f;
        light.exponentialAttenuation = 0.05f;
        addPointLightSource(light);
        light.position = glm::vec3(0.0f, 2.5f, 10.0f);
        light.color = glm::vec3(0.0f, 1.0f, 0.0f);
        addPointLightSource(light);
        light.position = glm::vec3(10.0f, 2.5f, 0.0f);
        light.color = glm::vec3(0.0f, 0.0f, 1.0f);
        addPointLightSource(light);
        SpotLight spot;
        spot.position = glm::vec3(15.0f, 1.5f, 3.0f);
        spot.direction = glm::vec3(-1.0f, 0.0f, 0.0f);
        spot.color = glm::vec3(1.0f, 1.0f, 1.0f);
        spot.coneAngle = 0.9f;
        spot.constantAttenuation = 0.1f;
        spot.linearAttenuation = 0.01f;
        spot.exponentialAttenuation = 0.01f;
        addSpotLightSource(spot);
        std::cout << "Level loaded" << std::endl;
    }
}

ControllableEntity* World::getPlayer(){
    return player;
}

void World::addObject(WorldObject* object){
    ShaderProgram* shader = getShader(object->getShaderRequired());
    if(renderMap.find(shader) == renderMap.end()){
        renderMap.insert(std::make_pair(shader, std::vector<WorldObject*>()));
    }
    renderMap[shader].push_back(object);
    objects.push_back(object);
}

void World::addCamera(Camera* camera){
    cameras.push_back(camera);
}