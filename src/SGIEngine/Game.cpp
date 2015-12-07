/*
 * File:   Game.cpp
 * Author: TheArni
 *
 * Created on 15 de febrero de 2015, 13:48
 */

#include "Game.h"

#include <document.h>
#include <stringbuffer.h>
#include <prettywriter.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <chrono>

#include "Camera.h"
#include "definitions.h"
#include "Config.h"
#include "RenderEngine.h"
#include "TaskEngine.h"
#include "ResourceEngine.h"
#include "LogicEngine.h"
#include "AudioEngine.h"
#include "Utility.h"

bool run = true;
bool _client;
std::string _title;
std::string currentState;
std::map<std::string, State*> states;
std::chrono::high_resolution_clock::time_point startTime;
long long microseconds = 0;

void saveConfig() {
    std::cout << "Saving config file..." << std::endl;
    FILE* file = fopen("config.json", "w");
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    rapidjson::Document doc;
    Config::serialize(&doc);
    doc.Accept(writer);
    fwrite(buffer.GetString(), buffer.GetSize(), 1, file);
    fclose(file);
    std::cout << "Saved config file." << std::endl;
}

bool loadConfig() {
    std::cout << "Loading configuration file." << std::endl;
    rapidjson::Document doc;
    if (readJsonFile("config.json", doc)) {
        Config::deserialize(&doc);
        return true;
    } else {
        return false;
    }
}

bool Game::init(std::string title, bool client) {
    std::cout << "Starting SGI Engine..." << std::endl;

    _title = title;
    _client = client;

    if(client){
        std::cout << "Initializing SDL..." << std::endl;
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            std::cerr << "Couldn't start game." << std::endl;
            return false;
        }
    }

    std::cout << "Initializing Task Engine..." << std::endl;
    TaskEngine::init();

    if (!loadConfig()) {
        std::cerr << "Couldn't start game." << std::endl;
        return false;
    }
    
    if(client){
        std::cout << "Initializing Rendering Engine..." << std::endl;
        if (!RenderEngine::init(title)) {
            std::cerr << "Couldn't start game." << std::endl;
            return false;
        }

        std::cout << "Initializing OpenGL..." << std::endl;
        if (!RenderEngine::initGL()) {
            std::cerr << "Couldn't start game." << std::endl;
            return false;
        }

        std::cout << "Initializing Audio Engine..." << std::endl;
        if (!AudioEngine::init()) {
            std::cerr << "Couldn't start game." << std::endl;
            return false;
        }
    }
    
    std::cout << "Initializing Logic Engine..." << std::endl;
    if (!LogicEngine::init()) {
        std::cerr << "Couldn't start game." << std::endl;
        return false;
    }

    return true;
}

void Game::start() {
    startTime = std::chrono::high_resolution_clock::now();
    long long updateMicroseconds = 0;
    SDL_Event event;
    while (run) {
        while (SDL_PollEvent(& event)) {
            if (!states[currentState]->processSDLEvent(event)) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT) {
                    Game::stop();
                }
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                for (Camera *camera : RenderEngine::getCameras()) {
                    camera->resize(event.window.data1, event.window.data2);
                }
            }
        }
        updateMicroseconds += microseconds;
        while(updateMicroseconds >= 1000000 / (float) Config::logic.updatesPerSecond){
            updateMicroseconds -= 1000000 / (float) Config::logic.updatesPerSecond;
            states[currentState]->update();
        }
        states[currentState]->render();
        microseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
        startTime = std::chrono::high_resolution_clock::now();
    }
    saveConfig();
    RenderEngine::kill();
    AudioEngine::kill();
    SDL_Quit();
}

long Game::lastTickTime() {
    return microseconds > 0 ? microseconds : 1;
}

bool Game::enterState(std::string name) {
    if (states.find(name) == states.end()) {
        return false;
    }
    if (states[currentState] != NULL) {
        states[currentState]->onExit();
    }
    currentState = name;
    std::cout << "Entering state " << name << std::endl;
    states[currentState]->onEnter();
    return true;
}

void Game::stop() {
    run = false;
}

State* Game::getState(std::string name) {
    return states[name];
}

void Game::addState(std::string name, State* state) {
    states.insert(std::pair<std::string, State*>(name, state));
}

bool Game::isClient(){
    return _client;
}
