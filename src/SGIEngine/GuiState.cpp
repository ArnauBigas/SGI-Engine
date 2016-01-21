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
#include "definitions.h"
#include "Config.h"

GuiState::GuiState() {
}

void GuiState::render() {
    if(gui != 0){
        RenderEngine::set2D();
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, Config::graphics.width, Config::graphics.height);
        glScissor(0, 0, Config::graphics.width, Config::graphics.height);
        
        glActiveTexture(GL_TEXTURE0 + GUITEXTUREUNIT);
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

