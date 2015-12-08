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

void GuiState::render() {
    if(gui != 0){
        RenderEngine::set2D();
    
        glProgramUniform1i(RenderEngine::getCurrentShader()->getProgramID(), RenderEngine::getCurrentShader()->getUniform("sampler"), 0);

        gui->draw();
    }
}

void GuiState::update(){
    //don't care m8
}

bool GuiState::processSDLEvent(SDL_Event& event) {
    if(gui != 0){
        return gui->processSDLEvent(event);
    }
    return false;
}

GuiState::GuiState(const GuiState& orig) {
}

GuiState::~GuiState() {
    delete gui;
}

