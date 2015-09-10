/* 
 * File:   Label.h
 * Author: arnau
 *
 * Created on 12 / d’agost / 2015, 23:16
 */

#ifndef LABEL_H
#define	LABEL_H

#include <string>

#include "GuiElement.h"

class Label : public GuiElement {
public:
    Label(int x, int y, std::string text);
    Label(const Label& orig);
    virtual ~Label();

    virtual void draw();

    virtual bool processSDLEvent(SDL_Event event) {
        return false;
    }

protected:
    std::string text;
    unsigned int vboid;
    unsigned int iboid;
    unsigned int vaoid;
};

#endif	/* LABEL_H */

