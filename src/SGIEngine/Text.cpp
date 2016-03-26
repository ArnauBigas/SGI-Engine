#include "Text.h"

std::string Text::color(unsigned char r, unsigned char g, unsigned char b) {
    return T_C + r + g + b;
}

std::string Text::color(float r, float g, float b) {
    return color((unsigned char) (r * 255), (unsigned char) (g * 255), (unsigned char) (b * 255));
}
