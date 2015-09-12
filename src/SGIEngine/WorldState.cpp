/*
 * File:   WorldState.cpp
 * Author: TheArni
 *
 * Created on 18 de abril de 2015, 19:11
 */
#include "WorldState.h"

#include <synchapi.h>

#include "RenderEngine.h"
#include "TaskEngine.h"
#include "Label.h"
#include "Game.h"
#include "definitions.h"

WorldState::WorldState() {
    world = new World();
}

WorldState::WorldState(const WorldState& orig) {
}

WorldState::~WorldState() {
    delete(world);
}

void WorldState::onEnter() {    
    world->loadFromFile(FMAPS"world/map.json");
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void WorldState::run() {
    world->integratePhysics();
    world->logicUpdate();
    world->renderWorld();
    GuiState::run();
}

bool WorldState::processSDLEvent(SDL_Event& event) {
    if (!GuiState::processSDLEvent(event)) {
        return world->processEvent(event);
    } else {
        return true;
    }
}