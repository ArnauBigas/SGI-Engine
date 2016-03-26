/* 
 * File:   Label.cpp
 * Author: arnau
 * 
 * Created on 12 / d’agost / 2015, 23:16
 */

#include "Label.h"

#include <cstring>

#include "RenderEngine.h"
#include "definitions.h"

Label::Label(int x, int y, Clamp clamp, std::string text) {
    this->text = text;
    loc.x = x;
    loc.y = y;
    int w, h;
    TTF_SizeText(RenderEngine::getFont(), text.c_str(), &w, &h);
    size.x = w;
    size.y = h;
    this->clamp = clamp;
}

void Label::draw() {
    RenderEngine::drawString(text, getLoc().x, getLoc().y);
}

Label::Label(const Label& orig) {
}

Label::~Label() {
}

void Label::setText(std::string text) {
    this->text = text;
}

std::string Label::getText() {
    return text;
}
