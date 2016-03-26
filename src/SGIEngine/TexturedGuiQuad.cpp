/* 
 * File:   TexturedGuiQuad.cpp
 * Author: arnau
 * 
 * Created on 2 / de setembre / 2015, 21:38
 */

#include "TexturedGuiQuad.h"

#include <GL/glew.h>

#include <iostream>
#include "definitions.h"

TexturedGuiQuad::TexturedGuiQuad(unsigned int texture, float x, float y, float w, float h) : texture(texture){
    float data[] = {
        x, y, 0, 0, 0,
        x, y+h, 0, 0, 1,
        x+w, y+h, 0, 1, 1,
        
        x, y, 0, 0, 0,
        x+w, y+h, 0, 1, 1,
        x+w, y, 0, 1, 0
    };
    unsigned int vboid;
    
    //Generate OpenGL buffers
    glGenVertexArrays(1, &vaoid);
    glGenBuffers(1, &vboid);

    //Bind buffers and upload data to OpenGL
    glBindVertexArray(vaoid);

    glBindBuffer(GL_ARRAY_BUFFER, vboid); //Bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof (data), &data[0], GL_STATIC_DRAW); //Upload Vertex Data

    //Setup attrib pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 20, BUFFER_OFFSET(12));
}

void TexturedGuiQuad::draw(){
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vaoid);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

