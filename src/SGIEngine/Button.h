/* 
 * File:   Button.h
 * Author: arnau
 *
 * Created on 21 / d’agost / 2015, 00:58
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include "GuiElement.h"
#include "Label.h"

#include <functional>

class Button : public GuiElement {
public:
    Button(int x, int y, Clamp clamp, std::string text, std::function<void() > action, GuiElement *parent);

    virtual void draw();

    virtual bool processSDLEvent(SDL_Event event);
private:
    Label label;
    unsigned int vaoid;
    unsigned int vboid;
    std::function<void() > action;
};

#endif	/* BUTTON_H */

