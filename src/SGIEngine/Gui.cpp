/* 
 * File:   Gui.cpp
 * Author: arnau
 * 
 * Created on 12 / d’agost / 2015, 22:42
 */

#include "Gui.h"

Gui::Gui() {
}

Gui::Gui(Gui* parent) {
    this->parent = parent;
    parent->child = this;
}

Gui::~Gui() {
    for (GuiElement* e : elements) {
        delete e;
    }
    if (parent) {
        parent->child = 0;
    }
}

bool Gui::processSDLEvent(SDL_Event event) {
    if (child) {
        return child->processSDLEvent(event);
    } else {
        for (GuiElement* e : elements) {
            if (e->processSDLEvent(event)) {
                return true;
            }
        }
    }
    return false;
}