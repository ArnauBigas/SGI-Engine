/*
 * File:   GuiState.cpp
 * Author: TheArni
 *
 * Created on 18 de abril de 2015, 19:03
 */

#include "GuiState.h"

#include <GL/glew.h>
#include <string>

#include "RenderEngine.h"
#include "Game.h"
#include "Shader.h"

GuiState::GuiState() {
}

void GuiState::run() {
    getShader("gui")->link();

    glUniformMatrix4fv(getShader("gui")->getUniform("MVP"), 1, GL_FALSE, &RenderEngine::getOrthoMatrix()[0][0]);

    glProgramUniform1i(getShader("gui")->getProgramID(), getShader("gui")->getUniform("sampler"), 0);

    glDisable(GL_DEPTH_TEST);

    gui->draw();

    if (Game::lastTickTime() > 0) {
        RenderEngine::drawString(std::to_string(1000000 / Game::lastTickTime()), 0, 0);
    }

    glEnable(GL_DEPTH_TEST);
    RenderEngine::swapBuffers();
}

bool GuiState::processSDLEvent(SDL_Event& event) {
    return gui->processSDLEvent(event);
}

GuiState::GuiState(const GuiState& orig) {
}

GuiState::~GuiState() {
    delete gui;
}

