/*
 * File:   World.cpp
 * Author: TheArni
 *
 * Created on 8 de marzo de 2015, 16:24
 */

#include "World.h"

#include <rapidjson.h>
#include <document.h>
#include <filestream.h>
#include <GL/glew.h>
#include <iostream>

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
            //Does the shader support textures?
            if(p.first->hasUniform("sampler")){
                glUniform1i(p.first->getUniform("sampler"), 0);
                glActiveTexture(GL_TEXTURE0);
            }
            for(WorldObject* o : p.second){
                RenderEngine::setModelMatrix(o->getModelMatrix());
                RenderEngine::updateMatrices();
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
    for(WorldObject* o : objects){
        o->update();
    }
    player->update();
}

void World::addLightSource(PointLight light){
    lights.push_back(light);
}

bool World::processEvent(SDL_Event event) {
    return player->processSDLEvent(event);
}

bool World::loadFromFile(std::string filename){
    FILE* f;
    f = fopen(filename.c_str(), "r");
    if (f == NULL) {
        std::cout << "Couldn't load the map!" << std::endl;
    } else {
        std::cout << "Loading level..." << std::endl;
        rapidjson::Document doc;
        rapidjson::FileStream is(f);
        doc.ParseStream<0>(is);
        rapidjson::Value& objects = doc["objects"];
        for (rapidjson::SizeType i = 0; i < objects.Size(); i++) {
            WorldObject* o = getWorldObject(objects[i]["type"].GetString())->clone();
            o->loadFromJson(objects[i]);
            addObject(o);
        }
        player = new ControllableEntity((float) doc["player"]["speed"].GetDouble());
        player->loadFromJson(doc["player"]);
        fclose(f);
    }
}

ControllableEntity* World::getPlayer(){
    return player;
}

void World::addObject(WorldObject* object){
    ShaderProgram* shader = getShader(object->getShader());
    if(renderMap.find(shader) == renderMap.end()){
        renderMap.insert(std::make_pair(shader, std::vector<WorldObject*>()));
    }
    renderMap[shader].push_back(object);
    objects.push_back(object);
}

void World::addCamera(Camera* camera){
    cameras.push_back(camera);
}