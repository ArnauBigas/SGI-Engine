/* 
 * File:   Gui.h
 * Author: arnau
 *
 * Created on 12 / d’agost / 2015, 22:42
 */

#ifndef GUI_H
#define	GUI_H

#include <vector>

#include "GuiElement.h"

class Gui : public GuiElement {
public:
    Gui();
    Gui(Gui* parent);
    virtual ~Gui();

    virtual void draw() {
        for (GuiElement* e : elements) {
            e->draw();
        }
    }

    void addElement(GuiElement* element) {
        elements.push_back(element);
    }

    bool processSDLEvent(SDL_Event event);

protected:
    Gui* child = 0;
    Gui* parent = 0;
    std::vector<GuiElement*> elements;
};

#endif	/* GUI_H */

