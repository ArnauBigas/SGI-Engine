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
#include "Camera.h"
#include "Config.h"
#include "Light.h"
#include "World.h"
#include <gtc/type_ptr.hpp>

RenderingTechnique::RenderingTechnique(unsigned int target, std::string shader){
    this->target = target;
    this->shader = shader;
}

int DeferredRendering::getType(){
    return DEFERRED_RENDERING;
}

//TODO: performance test
DeferredRendering::DeferredRendering(unsigned int target, std::string shader) : RenderingTechnique(target, shader){
    glGenFramebuffers(1, &framebuffer);
    glGenTextures(1, &diffuseTexture);
    glGenTextures(1, &normalTexture);
    glGenTextures(1, &materialsTexture);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, normalTexture, 0);
    
    glBindTexture(GL_TEXTURE_2D, materialsTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, materialsTexture, 0);
    
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
    
    GLenum windowBuffClear[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    
    glDrawBuffers(3, windowBuffClear);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cerr << "There was an error while resizing the framebuffer" << std::endl;
    }
}

void DeferredRendering::enable(Camera* cam){
    //Enable drawing to the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DeferredRendering::uploadBuffers(ShaderProgram* shaderptr, Camera* cam){
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
    
    if(shaderptr->hasUniform("materialsTexture")){
        glActiveTexture(GL_TEXTURE0 + MBUFFER);
        glBindTexture(GL_TEXTURE_2D, materialsTexture);
        glUniform1i(shaderptr->getUniform("materialsTexture"), MBUFFER);
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
    
    if(shaderptr->hasUniform("cameraPosition")){
        glUniform3fv(shaderptr->getUniform("cameraPosition"), 1, glm::value_ptr(cam->position));
    }
}

void DeferredRendering::disable(Camera* cam){
    //Draw the final output
    glBindFramebuffer(GL_FRAMEBUFFER, target);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE, GL_ONE);
    
    ShaderProgram* shaderptr = getShader(AMBIENTSHADER);
    
    RenderEngine::setCurrentShader(shaderptr);
    
    uploadBuffers(shaderptr, cam);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    shaderptr = getShader(POINTLIGHTSHADER);
    
    RenderEngine::setCurrentShader(shaderptr);
    
    uploadBuffers(shaderptr, cam);
    
    for(PointLight light : cam->world->getPointLights()){
        uploadPointLight(light, shaderptr);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    
    shaderptr = getShader(SPOTLIGHTSHADER);
    
    RenderEngine::setCurrentShader(shaderptr);
    
    uploadBuffers(shaderptr, cam);
    
    for(SpotLight light : cam->world->getSpotLights()){
        uploadSpotLight(light, shaderptr);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }    
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}