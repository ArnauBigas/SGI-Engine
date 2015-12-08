/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderingTechnique.h
 * Author: arnaubigas
 *
 * Created on December 8, 2015, 7:09 PM
 */

#ifndef RENDERINGTECHNIQUE_H
#define RENDERINGTECHNIQUE_H

#include <string>

#define FORWARD_RENDERING 0
#define DEFERRED_RENDERING 1

class RenderingTechnique {
public:
    RenderingTechnique(unsigned int target, std::string shader);
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual int getType() = 0;
    virtual void targetResized(int width, int height) = 0;
protected:
    unsigned int target = 0;
    std::string shader;
};

class DeferredRendering : public RenderingTechnique {
public:
    DeferredRendering(unsigned int target, std::string shader);
    virtual void enable();
    virtual void disable();
    virtual int getType();
    virtual void targetResized(int width, int height);
private:
    unsigned int diffuseTexture;
    unsigned int depthTexture;
    unsigned int normalTexture;
    unsigned int framebuffer;
    unsigned int quadVBO;
};

#endif /* RENDERINGTECHNIQUE_H */

