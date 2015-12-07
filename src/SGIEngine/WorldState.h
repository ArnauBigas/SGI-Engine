/*
 * File:   WorldState.h
 * Author: TheArni
 *
 * Created on 18 de abril de 2015, 19:11
 */

#ifndef WORLDSTATE_H
#define	WORLDSTATE_H

#include <string>
#include <SDL_events.h>

#include "GuiState.h"
#include "World.h"

class WorldState : public GuiState {
public:
    WorldState();
    WorldState(const WorldState& orig);
    virtual ~WorldState();

    virtual void onEnter();

    virtual void render();
    
    virtual void update();

    virtual bool processSDLEvent(SDL_Event& event);

    World* getWorld(){
        return world;
    }
private:
    World* world;
};

#endif	/* WORLDSTATE_H */

