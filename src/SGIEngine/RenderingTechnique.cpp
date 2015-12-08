/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderingTechnique.cpp
 * Author: arnaubigas
 * 
 * Created on December 8, 2015, 7:09 PM
 */

#include "RenderingTechnique.h"
#include "gl/glew.h"
#include "RenderEngine.h"
#include "definitions.h"
#include "detail/func_matrix.hpp"
#include <iostream>

RenderingTechnique::RenderingTechnique(unsigned int target, std::string shader){
    this->target = target;
    this->shader = shader;
}

int DeferredRendering::getType(){
    return DEFERRED_RENDERING;
}

DeferredRendering::DeferredRendering(unsigned int target, std::string shader) : RenderingTechnique(target, shader){
    glGenFramebuffers(1, &framebuffer);
    glGenTextures(1, &diffuseTexture);
    glGenTextures(1, &normalTexture);
    glGenTextures(1, &depthTexture);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cerr << "There was an error while creating the framebuffer" << std::endl;
    }
}

void DeferredRendering::targetResized(int width, int height){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, diffuseTexture, 0);
    
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, normalTexture, 0);
    
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
    
    GLenum windowBuffClear[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    
    glDrawBuffers(2, windowBuffClear);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cerr << "There was an error while resizing the framebuffer" << std::endl;
    }
}

void DeferredRendering::enable(){
    //Enable drawing to the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DeferredRendering::disable(){
    //Draw the final output
    glBindFramebuffer(GL_FRAMEBUFFER, target);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ShaderProgram* shaderptr = getShader(shader);
    
    RenderEngine::setCurrentShader(shaderptr);
    
    if(shaderptr->hasUniform("diffuseTexture")){
        glActiveTexture(GL_TEXTURE0 + GBUFFER);
        glBindTexture(GL_TEXTURE_2D, diffuseTexture);
        glUniform1i(shaderptr->getUniform("diffuseTexture"), GBUFFER);
    }
    
    if(shaderptr->hasUniform("normalTexture")){
        glActiveTexture(GL_TEXTURE0 + NBUFFER);
        glBindTexture(GL_TEXTURE_2D, normalTexture);
        glUniform1i(shaderptr->getUniform("normalTexture"), NBUFFER);
    }
    
    if(shaderptr->hasUniform("depthTexture")){
        glActiveTexture(GL_TEXTURE0 + ZBUFFER);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glUniform1i(shaderptr->getUniform("depthTexture"), ZBUFFER);
    }
    
    if(shaderptr->hasUniform("invrVP")){
        glm::mat4 mat = glm::inverse(RenderEngine::getProjectionMatrix()*RenderEngine::getViewMatrix());
        glUniformMatrix4fv(shaderptr->getUniform("invrVP"), 1, GL_FALSE, &(mat)[0][0]);
    }
    
    //vertices in the vertex shader should be as follows: const vec2 quadVertices[4] = { vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) };
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
}