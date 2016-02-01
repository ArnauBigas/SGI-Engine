/* 
 * File:   Gui.cpp
 * Author: arnau
 * 
 * Created on 12 / d’agost / 2015, 22:42
 */

#include "Gui.h"

std::map<std::string, Gui *> guis;

Gui *GuiManager::getGui(std::string name) {
    return guis[name];
}

Gui::Gui(std::string name) {
    guis[name] = this;
}

Gui::Gui(std::string name, Gui* parent) {
    this->parent = parent;
    parent->child = this;
    guis[name] = this;
}

Gui::~Gui() {
    for (std::pair<std::string, GuiElement*> p : elements) {
        delete p.second;
    }
    if (parent) {
        parent->child = 0;
    }
}

bool Gui::processSDLEvent(SDL_Event event) {
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        size.x = event.window.data1;
        size.y = event.window.data2;
    }
    
    if (child) {
        return child->processSDLEvent(event);
    } else {
        for (std::pair<std::string, GuiElement*> p : elements) {
            if (p.second->processSDLEvent(event)) {
                return true;
            }
        }
    }
    return false;
}