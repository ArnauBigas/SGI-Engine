/*
 * File:   Game.cpp
 * Author: TheArni
 *
 * Created on 15 de febrero de 2015, 13:48
 */

#include "Game.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filestream.h>
#include <rapidjson/prettywriter.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <chrono>

#include "definitions.h"
#include "Config.h"
#include "RenderEngine.h"
#include "TaskEngine.h"
#include "ResourceEngine.h"
#include "LogicEngine.h"
#include "AudioEngine.h"

bool run = true;
std::string title;
std::string currentState;
std::map<std::string, State*> states;
std::chrono::high_resolution_clock::time_point startTime;
long long microseconds;

void saveConfig() {
    std::cout << "Saving config file..." << std::endl;
    FILE* file = fopen("config.json", "w");
    rapidjson::GenericStringBuffer< rapidjson::UTF8<> > buffer;
    rapidjson::PrettyWriter<rapidjson::GenericStringBuffer< rapidjson::UTF8<> > > writer(buffer);
    rapidjson::Document doc;
    Config::serialize(&doc);
    doc.Accept(writer);
    fwrite(buffer.GetString(), buffer.Size(), 1, file);
    fclose(file);
    std::cout << "Saved config file." << std::endl;
}

bool loadConfig() {
    std::cout << "Loading configuration file." << std::endl;
    FILE* file;
    file = fopen("config.json", "r");
    if (file == NULL) {
        fclose(file);
        saveConfig();
    } else {
        rapidjson::Document doc;
        rapidjson::FileStream is(file);
        doc.ParseStream<0>(is);
        Config::deserialize(&doc);
        fclose(file);
    }
    return true;
}

bool Game::init(std::string title) {
    std::cout << "Starting SGI Engine..." << std::endl;

    title = title;
    
    std::cout << "Initializing SDL..." << std::endl;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        std::cerr << "Couldn't start game." << std::endl;
        return false;
    }

    std::cout << "Initializing Task Engine..." << std::endl;
    TaskEngine::init();

    if (!loadConfig()) {
        std::cerr << "Couldn't start game." << std::endl;
        return false;
    }

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
    if(!AudioEngine::init()){
        std::cerr << "Couldn't start game." << std::endl;
        return false;
    }
    
    std::cout << "Initializing Logic Engine..." << std::endl;
    if(!LogicEngine::init()){
        std::cerr << "Couldn't start game." << std::endl;
        return false;
    }
    
    return true;
}

void Game::start() {
    startTime = std::chrono::high_resolution_clock::now();
    SDL_Event event;
    while (run) {        
        while (SDL_PollEvent(& event)) {
            if(!states[currentState]->processSDLEvent(event)){
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    Game::stop();
                }
            }
        }
        states[currentState]->run();
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