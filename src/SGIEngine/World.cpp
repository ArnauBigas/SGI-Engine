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
#include "definitions.h"
#include "Config.h"

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
                for(WorldObject* o : p.second){
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
            //std::cout << "loading object of type " << objects[i]["type"].GetString() << std::endl;
            WorldObject* o = getWorldObject(objects[i]["type"].GetString())->clone();
            o->initFromJson(this, objects[i]);
            addObject(o);
            //std::cout << "object loaded" << std::endl;
        }
        player = new ControllableEntity((float) doc["player"]["speed"].GetDouble());
        player->initFromJson(this, doc["player"]);
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