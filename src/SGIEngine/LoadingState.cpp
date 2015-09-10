/* 
 * File:   LoadingState.cpp
 * Author: TheArni
 * 
 * Created on 18 de abril de 2015, 19:36
 */

#include "LoadingState.h"

#include <gl/glew.h>

#include "ResourceEngine.h"
#include "RenderEngine.h"
#include "TaskEngine.h"
#include "Shader.h"
#include "Game.h"

void LoadingState::onEnter() {
}

void LoadingState::run() {
    loadShaders();

    getShader("gui")->link();

    glUniformMatrix4fv(getShader("gui")->getUniform("MVP"), 1, GL_FALSE, &RenderEngine::getOrthoMatrix()[0][0]);

    glProgramUniform1i(getShader("gui")->getProgramID(), getShader("gui")->getUniform("sampler"), 0);

    glDisable(GL_DEPTH_TEST);

    RenderEngine::drawString("Loading textures...", 0, 0);

    RenderEngine::swapBuffers();

    loadTextures();

    RenderEngine::drawString("Loading audio files...", 0, 0);

    RenderEngine::swapBuffers();

    loadAudio();
    
    RenderEngine::drawString("Loading all domain resources...", 0, 0);

    RenderEngine::swapBuffers();

    loadAllDomainResources();
    
    glEnable(GL_DEPTH_TEST);
    Game::enterState(stateName);
}
