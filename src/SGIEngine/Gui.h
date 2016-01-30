/* 
 * File:   Gui.h
 * Author: arnau
 *
 * Created on 12 / d’agost / 2015, 22:42
 */

#ifndef GUI_H
#define	GUI_H

#include <map>
#include <string>
#include "GuiElement.h"

class Gui : public GuiElement {
public:
    Gui(std::string name);
    Gui(std::string name, Gui* parent);
    virtual ~Gui();

    virtual void draw() {
        for (std::pair<std::string, GuiElement*> p : elements) {
            p.second->draw();
        }
    }

    void addElement(GuiElement* element, std::string name) {
        elements[name] = element;
    }
    
    GuiElement *getElement(std::string name) {
        return elements[name];
    }

    bool processSDLEvent(SDL_Event event);

protected:
    Gui* child = 0;
    Gui* parent = 0;
    std::map<std::string, GuiElement*> elements;
};

namespace GuiManager {
    Gui *getGui(std::string name);
}

#endif	/* GUI_H */

