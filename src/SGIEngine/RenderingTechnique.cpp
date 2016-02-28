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
#include "Logger.h"
#include "Camera.h"
#include "Config.h"
#include "Light.h"
#include "World.h"
#include <gtc/type_ptr.hpp>

#define KERNELSIZE 16
#define NOISESIZE 16
#define NOISEWIDTH 4

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
    glGenTextures(1, &ssaoTexture);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        Logger::error << "There was an error while creating the framebuffer" << std::endl;
    }
    
    if(Config::graphics.ssao){    
        glGenFramebuffers(1, &ssaoFramebuffer);
        glm::vec3 noise[NOISESIZE];
        for (int i = 0; i < NOISESIZE; ++i) {
            noise[i] = glm::normalize(glm::vec3(
                random(-1.0f, 1.0f),
                random(-1.0f, 1.0f),
                0.0f));
        }

        unsigned int noiseTexture;
        glGenTextures(1, &noiseTexture);
        glActiveTexture(GL_TEXTURE0 + RANDOMNOISETEXTUREUNIT);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, noise);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glActiveTexture(GL_TEXTURE0);
    }
}

void DeferredRendering::targetResized(int width, int height){
    GLenum err = glGetError();
    lastWidth = width;
    lastHeight = height;
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
        Logger::error << "There was an error while resizing the framebuffer" << std::endl;
    }
    
    glBindTexture(GL_TEXTURE_2D, ssaoTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    err = glGetError();
    if(err != GL_NONE){
        Logger::error << err << std::endl;
    }
    
    if(Config::graphics.ssao){
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFramebuffer);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ssaoTexture, 0);
    
        GLenum windowBuffClear[] = {GL_COLOR_ATTACHMENT0};
        
        glDrawBuffers(1, windowBuffClear);
        
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            Logger::error << "There was an error while resizing the ssao framebuffer" << std::endl;
        }
    }
}

void DeferredRendering::enable(Camera* cam){
    //Enable drawing to the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!ssaouploaded){
        ssaouploaded = true;
        //SSAO
        ShaderProgram* shaderptr = getShader(SSAOSHADER);
        glm::vec3 kernel[KERNELSIZE];
        for (int i = 0; i < KERNELSIZE; ++i) {
            kernel[i] = glm::normalize(glm::vec3(
            random(-1.0f, 1.0f),
            random(-1.0f, 1.0f),
            random(0.0f, 1.0f)));
            kernel[i] *= random(0.0f, 1.0f);
            float scale = float(i) / float(KERNELSIZE);
            kernel[i] *= lerp(0.1f, 1.0f, scale * scale);
        }
        shaderptr->link();
        glUniform3fv(shaderptr->getUniform("kernel"), KERNELSIZE, glm::value_ptr(kernel[0]));
        glUniform1i(shaderptr->getUniform("kernelSize"), KERNELSIZE);
        GLenum err = glGetError();
        if(err != GL_NONE){
            Logger::error << err << std::endl;
        }
    }
}

void DeferredRendering::uploadBuffers(ShaderProgram* shaderptr, Camera* cam){    
    if(shaderptr->hasUniform("MVP")){
        glm::mat4 mvp = RenderEngine::getProjectionMatrix()*RenderEngine::getViewMatrix();
        glUniformMatrix4fv(shaderptr->getUniform("MVP"), 1, GL_FALSE, &mvp[0][0]);
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
    glActiveTexture(GL_TEXTURE0 + DIFFUSEBUFFERTEXTUREUNIT);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glActiveTexture(GL_TEXTURE0 + NORMALBUFFERTEXTUREUNIT);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glActiveTexture(GL_TEXTURE0 + MATERIALBUFFERTEXTUREUNIT);
    glBindTexture(GL_TEXTURE_2D, materialsTexture);
    glActiveTexture(GL_TEXTURE0 + DEPTHBUFFERTEXTUREUNIT);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    ShaderProgram* shaderptr;
    
    if(Config::graphics.ssao){
        shaderptr = getShader(SSAOSHADER);
        RenderEngine::setCurrentShader(shaderptr);
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFramebuffer);
        glActiveTexture(GL_TEXTURE0 + SSAOTEXTUREUNIT);
        glBindTexture(GL_TEXTURE_2D, ssaoTexture);
        uploadBuffers(shaderptr, cam);
        glUniform2f(shaderptr->getUniform("noiseScale"), (float) lastWidth / (float) NOISEWIDTH, (float) lastHeight / (float) NOISEWIDTH);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    
    //Draw the final output
    glBindFramebuffer(GL_FRAMEBUFFER, target);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE, GL_ONE); 
    
    shaderptr = getShader(AMBIENTSHADER);
    
    RenderEngine::setCurrentShader(shaderptr);
    
    uploadBuffers(shaderptr, cam);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    shaderptr = getShader(POINTLIGHTSHADER);
    
    RenderEngine::setCurrentShader(shaderptr);
    
    uploadBuffers(shaderptr, cam);
    
    for(PointLight *light : cam->world->getPointLights()){
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