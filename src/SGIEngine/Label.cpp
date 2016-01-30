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

Label::Label(int x, int y, std::string text) {
    this->text = text;
    min.x = x;
    min.y = y;
    int x2, y2;
    TTF_SizeText(RenderEngine::getFont(), text.c_str(), &x2, &y2);
    max.x = x + x2;
    max.y = y + y2;
}

void Label::draw() {
    RenderEngine::drawString(text, min.x, min.y);
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
