/* 
 * File:   Button.cpp
 * Author: arnau
 * 
 * Created on 21 / d’agost / 2015, 00:58
 */

#include "Button.h"

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderEngine.h"
#include "Texture.h"
#include "definitions.h"
#include "Shader.h"

Button::Button(int x, int y, Clamp clamp, std::string text, std::function<void() > action, GuiElement *parent) : label(x + 2, y + 2, clamp, text) { //Remove parent, support for dynamic guis
    this->parent = parent;
    this->action = action;
    loc.x = x;
    loc.y = y;
    size.x = label.getSize().x + 5;
    size.y = label.getSize().y + 5;
    this->clamp = clamp;
    float fx = (float) getLoc().x;
    float fy = (float) getLoc().y;
    float data[] = {
        fx, fy, 0, 0, 0,
        fx, fy+getSize().y, 0, 0, 1,
        fx + 5, fy+getSize().y, 0, 5.0f / 32.0f, 1,

        fx, fy, 0, 0, 0,
        fx + 5, fy+getSize().y, 0, 5.0f / 32.0f, 1,
        fx + 5, fy, 0, 5.0f / 32.0f, 0,

        fx + 5, fy, 0, 5.0f / 32.0f, 0,
        fx + 5, fy+getSize().y, 0, 5.0f / 32.0f, 1,
        fx+getSize().x - 5, fy+getSize().y, 0, 27.0f / 32.0f, 1,

        fx + 5, fy, 0, 5.0f / 32.0f, 0,
        fx+getSize().x - 5, fy+getSize().y, 0, 27.0f / 32.0f, 1,
        fx+getSize().x - 5, fy, 0, 27.0f / 32.0f, 0,

        fx+getSize().x - 5, fy, 0, 27.0f / 32.0f, 0,
        fx+getSize().x - 5, fy+getSize().y, 0, 27.0f / 32.0f, 1,
        fx+getSize().x, fy+getSize().y, 0, 1, 1,

        fx+getSize().x - 5, fy, 0, 27.0f / 32.0f, 0,
        fx+getSize().x, fy+getSize().y, 0, 1, 1,
        fx+getSize().x, fy, 0, 1, 0
    };
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

void Button::draw() {
    //glUniformMatrix4fv(getShader("gui")->getUniform("MVP"), 1, GL_FALSE, &(RenderEngine::getOrthoMatrix() * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)))[0][0]);
    glBindTexture(GL_TEXTURE_2D, getTexture("gui:button"));
    glBindVertexArray(vaoid);
    glDrawArrays(GL_TRIANGLES, 0, 18);
    label.draw();
}

bool Button::processSDLEvent(SDL_Event event) {
    if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.x > getLoc().x && event.button.x < getLoc().x+getSize().x && event.button.y > getLoc().y && event.button.y < getLoc().y+getSize().y) {
            action();
        }
    }
    return false;
}