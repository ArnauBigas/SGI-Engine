/* 
 * File:   GuiElement.h
 * Author: arnau
 *
 * Created on 12 / d’agost / 2015, 22:42
 */

#ifndef GUIELEMENT_H
#define	GUIELEMENT_H

#include <vec2.hpp>
#include <SDL_events.h>

class GuiElement {
public:
    GuiElement();
    GuiElement(const GuiElement& orig);
    virtual ~GuiElement();

    virtual void draw() = 0;

    virtual bool processSDLEvent(SDL_Event event) = 0;

    glm::vec2 getMin() {
        return min;
    }

    glm::vec2 getMax() {
        return max;
    }
protected:
    glm::vec2 min;
    glm::vec2 max;
};

#endif	/* GUIELEMENT_H */

