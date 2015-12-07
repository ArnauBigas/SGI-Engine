/*
 * File:   State.h
 * Author: TheArni
 *
 * Created on 18 de abril de 2015, 18:57
 */

#ifndef STATE_H
#define	STATE_H

#include <SDL_events.h>

class State {
public:

    /**
     * Event method called when the game switches states and enters this one.
     */
    virtual void onEnter() {

    }

    /**
     * Event method called when the game switches states and exits this one.
     */
    virtual void onExit() {

    }
    
    /**
     * Event method called when there's some input event.
     * 
     * @return true if the event has been consumed, false if nothing happened.
     */
    virtual bool processSDLEvent(SDL_Event& event) {
        return false;
    }

    /**
     * Called each frame to run state-specific rendering code.
     */
    virtual void render() = 0;
    
    /**
     * Called each tick to run state-specific logic code.
     */
    virtual void update() = 0;
};

#endif	/* STATE_H */

