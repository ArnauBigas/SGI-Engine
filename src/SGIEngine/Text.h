/* 
 * File:   Text.h
 * Author: Sam
 *
 * Created on 07 January 2016, 19:01
 */

#ifndef TEXT_H
#define	TEXT_H

#define T_SUB       "\x1A"
#define T_C         T_SUB "\x1"
#define T_S         T_SUB "\x2"

#define C_BLACK     T_C"\x00\x00\x00"
#define C_WHITE     T_C"\xFF\xFF\xFF"
#define C_RED       T_C"\x98\x07\x07"
#define C_GREEN     T_C"\x04\x66\x04"
#define C_BLUE      T_C"\x04\x44\xEE"
#define C_YELLOW    T_C"\xEF\xF1\x06"
#define C_CYAN      T_C"\x0F\xD2\xE3"
#define C_MAGENTA   T_C"\xFF\x00\xFF"

#include <string>

namespace Text {
    std::string color(unsigned char r, unsigned char g, unsigned char b);
    std::string color(float r, float g, float b);
}

#endif	/* TEXT_H */

