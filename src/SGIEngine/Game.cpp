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
#include "Timer.h"
#include "Profiler.h"
#include "Logger.h"

bool run = true;
bool _client;
std::string _title;
std::string currentState;
std::map<std::string, State*> states;
std::chrono::high_resolution_clock::time_point startTime;
long long microseconds = 0;

struct PerformanceData {
    long long frames;
    long long ticks;
    int fps;
    int tps;
} pData;

void saveConfig() {
    Logger::info << "Saving config file..." << std::endl;
    FILE* file = fopen("config.json", "w");
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    rapidjson::Document doc;
    Config::serialize(&doc);
    doc.Accept(writer);
    fwrite(buffer.GetString(), buffer.GetSize(), 1, file);
    fclose(file);
    Logger::info << "Saved config file." << std::endl;
}

bool loadConfig() {
    Logger::info << "Loading configuration file." << std::endl;
    rapidjson::Document doc;
    if (readJsonFile("config.json", doc)) {
        Config::deserialize(&doc);
        return true;
    } else {
        return false;
    }
}

bool Game::init(std::string title, bool client) {
    Profiler::init();

    Logger::info << "Starting SGI Engine..." << std::endl;

    _title = title;
    _client = client;

    if (client) {
        Logger::info << "Initializing SDL..." << std::endl;
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            Logger::error << "Couldn't start game." << std::endl;
            return false;
        }
    }

    Logger::info << "Initializing Task Engine..." << std::endl;
    TaskEngine::init();

    if (!loadConfig()) {
        Logger::error << "Couldn't start game." << std::endl;
        return false;
    }

    if (client) {
        Logger::info << "Initializing Rendering Engine..." << std::endl;
        if (!RenderEngine::init(title)) {
            Logger::error << "Couldn't start game." << std::endl;
            return false;
        }

        Logger::info << "Initializing OpenGL..." << std::endl;
        if (!RenderEngine::initGL()) {
            Logger::error << "Couldn't start game." << std::endl;
            return false;
        }

        Logger::info << "Initializing Audio Engine..." << std::endl;
        if (!AudioEngine::init()) {
            Logger::error << "Couldn't start game." << std::endl;
            return false;
        }
    }

    Logger::info << "Initializing Logic Engine..." << std::endl;
    if (!LogicEngine::init()) {
        Logger::error << "Couldn't start game." << std::endl;
        return false;
    }

    return true;
}

void Game::start() {
    startTime = std::chrono::high_resolution_clock::now();
    long long updateMicroseconds = 0;
    Timer secondTimer;
    secondTimer.reset();
    secondTimer.start();
    SDL_Event event;
    while (run) {
        Profiler::reset();
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
        microseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
        microseconds = microseconds > 0 ? microseconds : 0;
        startTime = std::chrono::high_resolution_clock::now();

        updateMicroseconds += microseconds;
        while (updateMicroseconds >= 1000000 / (float) Config::logic.updatesPerSecond) {
            updateMicroseconds -= 1000000 / (float) Config::logic.updatesPerSecond;
            Profiler::start("update");
            states[currentState]->update();
            Profiler::stop();

            pData.ticks++;
            pData.tps++;
        }

        Profiler::start("render");
        states[currentState]->render();
        Profiler::start("swap");
        RenderEngine::swapBuffers();
        Profiler::stop();
        Profiler::stop();

        pData.frames++;
        pData.fps++;

        if (secondTimer.getTime() >= 1000) {
            Logger::info << "FPS: " << +pData.fps << ", TPS: " << +pData.tps << " \tFrames: " << +pData.frames << ", Ticks: " << +pData.ticks << "\t Last second: " << +secondTimer.getTime() << std::endl;
            secondTimer.reset();
            pData.fps = pData.tps = 0;

            Logger::info << "Update: " << +Profiler::get("update")->timer.getTime() << std::endl;
            Logger::info << "Render: " << +Profiler::get("render")->timer.getTime() << std::endl;
            Logger::info << "Render.Swap: " << Profiler::get("render")->get("swap")->timer.getTime() << std::endl;
        }
    }
    Profiler::cleanup();
    saveConfig();
    RenderEngine::kill();
    AudioEngine::kill();
    SDL_Quit();
}

long Game::lastTickTime() {
    return microseconds;
}

bool Game::enterState(std::string name) {
    if (states.find(name) == states.end()) {
        return false;
    }
    if (states[currentState] != NULL) {
        states[currentState]->onExit();
    }
    currentState = name;
    Logger::info << "Entering state " << name << std::endl;
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

bool Game::isClient() {
    return _client;
}
