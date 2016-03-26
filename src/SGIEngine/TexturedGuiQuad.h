/* 
 * File:   TexturedGuiQuad.h
 * Author: arnau
 *
 * Created on 2 / de setembre / 2015, 21:38
 */

#ifndef TEXTUREDGUIQUAD_H
#define	TEXTUREDGUIQUAD_H

#include "GuiElement.h"

class TexturedGuiQuad : public GuiElement {
public:
    TexturedGuiQuad(unsigned int texture, float x, float y, float w, float h);
    
    virtual void draw();

    virtual bool processSDLEvent(SDL_Event event){
        return false;
    }
    
private:
    unsigned int texture;
    unsigned int vaoid;
};

#endif	/* TEXTUREDGUIQUAD_H */

