/*
 * File:   GuiState.h
 * Author: TheArni
 *
 * Created on 18 de abril de 2015, 19:03
 */

#ifndef GUISTATE_H
#define	GUISTATE_H

#include <SDL_events.h>

#include "State.h"
#include "Gui.h"

class GuiState : public State {
public:
    GuiState();
    GuiState(const GuiState& orig);
    virtual ~GuiState();
    virtual void run();
    virtual bool processSDLEvent(SDL_Event& event);

    void setGui(Gui* gui) {
        this->gui = gui;
    }
protected:
    Gui* gui = 0;
};

#endif	/* GUISTATE_H */