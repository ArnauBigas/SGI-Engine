/* 
 * File:   GuiElement.cpp
 * Author: arnau
 * 
 * Created on 12 / d’agost / 2015, 22:42
 */

#include "GuiElement.h"
#include "Config.h"

GuiElement::GuiElement() {
}

GuiElement::GuiElement(const GuiElement& orig) {
}

GuiElement::~GuiElement() {
}

glm::vec2 GuiElement::getLoc() {
    int w = Config::graphics.width;
    int h = Config::graphics.height;
    switch (clamp) {
        case TL:
            return loc;
        case TC:
            return loc+glm::vec2(w/2, 0);
        case TR:
            return loc+glm::vec2(w, 0);
        case CL:
            return loc+glm::vec2(0, h/2);
        case CC:
            return loc+glm::vec2(w/2, h/2);
        case CR:
            return loc+glm::vec2(w, h/2);
        case BL:
            return loc+glm::vec2(0, h);
        case BC:
            return loc+glm::vec2(w/2, h);
        case BR:
            return loc+glm::vec2(w, h);
    }
}

glm::vec2 GuiElement::getSize() {
    return size;
}

