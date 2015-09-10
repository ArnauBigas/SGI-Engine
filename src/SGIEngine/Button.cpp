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

Button::Button(int x, int y, std::string text, std::function<void() > action) : label(x + 5, y + 5, text) {
    this->action = action;
    min.x = x;
    min.y = y;
    max.x = label.getMax().x + 5;
    max.y = label.getMax().y + 5;
    float fx = (float) x;
    float fy = (float) y;
    float data[] = {
        fx, fy, 0, 0, 0,
        fx, max.y, 0, 0, 1,
        fx + 5, max.y, 0, 5.0f / 32.0f, 1,

        fx, fy, 0, 0, 0,
        fx + 5, max.y, 0, 5.0f / 32.0f, 1,
        fx + 5, fy, 0, 5.0f / 32.0f, 0,

        fx + 5, fy, 0, 5.0f / 32.0f, 0,
        fx + 5, max.y, 0, 5.0f / 32.0f, 1,
        max.x - 5, max.y, 0, 27.0f / 32.0f, 1,

        fx + 5, fy, 0, 5.0f / 32.0f, 0,
        max.x - 5, max.y, 0, 27.0f / 32.0f, 1,
        max.x - 5, fy, 0, 27.0f / 32.0f, 0,

        max.x - 5, fy, 0, 27.0f / 32.0f, 0,
        max.x - 5, max.y, 0, 27.0f / 32.0f, 1,
        max.x, max.y, 0, 1, 1,

        max.x - 5, fy, 0, 27.0f / 32.0f, 0,
        max.x, max.y, 0, 1, 1,
        max.x, fy, 0, 1, 0
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
        if (event.button.x > min.x && event.button.x < max.x && event.button.y > min.y && event.button.y < max.y) {
            action();
        }
    }
    return false;
}