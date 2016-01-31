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

enum Clamp {//VH
    TL, TR, BL, BR,
    TC, CR, CL, BC,
    CC
};

/*
 * TL-TC-TR
 * |  |  |
 * CL-CC-CR
 * |  |  |
 * BL-BC-BR
 */

class GuiElement {
public:
    GuiElement();
    GuiElement(const GuiElement& orig);
    virtual ~GuiElement();

    virtual void draw() = 0;

    virtual bool processSDLEvent(SDL_Event event) = 0;

    glm::vec2 getSize();
    glm::vec2 getLoc();
    
protected:
    glm::vec2 loc;
    glm::vec2 size;
    
    Clamp clamp = TL;
};

#endif	/* GUIELEMENT_H */

