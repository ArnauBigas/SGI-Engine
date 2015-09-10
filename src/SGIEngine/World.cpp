/*
 * File:   World.cpp
 * Author: TheArni
 *
 * Created on 8 de marzo de 2015, 16:24
 */

#include "World.h"

#include <vec3.hpp>
#include <gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <rapidjson.h>
#include <document.h>
#include <filestream.h>
#include <iostream>

#include "definitions.h"
#include "RenderEngine.h"
#include "Prop.h"
#include "Utility.h"
#include "Game.h"
#include "Shader.h"
#include "Texture.h"
#include "Config.h"
#include "AudioEngine.h"
#include "PositionalAudioObject.h"

unsigned int fbo;
unsigned int depthTexture;

glm::vec3 lightInvDir = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 lightDir = glm::normalize(glm::vec3(cos(glm::radians(-90.0f)) * cos(glm::radians(0.0f)),
        sin(glm::radians(-90.0f)),
        cos(glm::radians(-90.0f)) * sin(glm::radians(0.0f))
        ));
        
glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos + lightDir, glm::vec3(0,1,0));

glm::mat4 biasMatrix(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
        );

unsigned int World::getShadowmapTexture(){
    return depthTexture;
}

World::World() {

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &depthTexture);

    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, Config::graphics.shadowmapResolution, Config::graphics.shadowmapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);


    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    glDrawBuffer(GL_NONE); // No color buffer is drawn to.

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "there was an error while setting up the shadow frame buffer" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    std::cout << "Map loaded." << std::endl;
}

World::World(const World& orig) {
}

World::~World() {
}

void World::renderWorld(RenderPass pass) {
    glm::mat4 view;
    glm::mat4 mvp;
    ShaderProgram* program;

    if (pass == DIFFUSE) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        view = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, upDirection);
        mvp = RenderEngine::getProjectionMatrix() * view * glm::mat4(1.0f);

        program = getShader("light");
    } else if (pass == SHADOWMAPPING) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClear(GL_DEPTH_BUFFER_BIT);

        mvp = depthProjectionMatrix * depthViewMatrix * glm::mat4(1.0f);
        program = getShader("shadowmap");
    }

    program->link();

    glUniformMatrix4fv(program->getUniform("MVP"), 1, GL_FALSE, &mvp[0][0]);

    if (pass == DIFFUSE) {

        glm::mat4 depthBiasMVP = biasMatrix * depthProjectionMatrix * depthViewMatrix;
        glUniformMatrix4fv(program->getUniform("shadowMVP"), 1, GL_FALSE, &depthBiasMVP[0][0]);

        glUniform3f(program->getUniform("fragmentPosition_worldspace"), 0.0f, 0.0f, 0.0f);

        glUniform1i(program->getUniform("sampler"), 0);
        
        glUniform1i(program->getUniform("multi1"), 1);
        glUniform1i(program->getUniform("multi2"), 2);
        glUniform1i(program->getUniform("multi3"), 3);       

        glUniform1i(program->getUniform("shadowMap"), 4);

        glActiveTexture(GL_TEXTURE0);

        glUniform3f(program->getUniform("ambientLight"), ambientLight.r, ambientLight.g, ambientLight.b);

        glUniform1i(program->getUniform("numLights"), lights.size());

        for (int i = 0; i < lights.size(); i++) {
            uploadPointLightArray(lights[i], program->getProgramID(), "lights", i);
        }

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, getTexture("grass"));
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, getTexture("dirt"));
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, getTexture("snow"));
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, depthTexture);        
    }
    
    if(pass == DIFFUSE){
        glUniform1f(program->getUniform("multiSample"),1.0f);
    }
    
    terrain->render();
    
    if(pass == DIFFUSE){
        glUniform1f(program->getUniform("multiSample"),0.0f);
    }
    
    glActiveTexture(GL_TEXTURE0);

    for (WorldObject* o : objects) {
        if (pass == DIFFUSE) {
            mvp = RenderEngine::getProjectionMatrix() * view * glm::translate(glm::mat4(1.0f), o->getPosition());
            glUniform3f(program->getUniform("fragmentPosition_worldspace"), o->getPosition().x, o->getPosition().y, o->getPosition().z);
            glm::mat4 depthBiasMVP = biasMatrix * (depthProjectionMatrix * depthViewMatrix * glm::translate(glm::mat4(1.0f), o->getPosition()));
            glUniformMatrix4fv(program->getUniform("shadowMVP"), 1, GL_FALSE, &depthBiasMVP[0][0]);
        } else if (pass == SHADOWMAPPING) {
            mvp = depthProjectionMatrix * depthViewMatrix * glm::translate(glm::mat4(1.0f), o->getPosition());
        }

        glUniformMatrix4fv(program->getUniform("MVP"), 1, GL_FALSE, &mvp[0][0]);

        o->render(pass);
    }

    GLint err = glGetError();
    if (err != GL_NO_ERROR) {

        std::cout << "GL ERROR (world render, " << (pass == DIFFUSE ? "diffuse" : "shadowmap") << " pass): " << err << std::endl;
        std::cout << gluErrorString(err) << std::endl;
    }
}

void World::integratePhysics() {
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraDirection = glm::normalize(front);
    if (frwd) {
        cameraPosition += cameraDirection * speed;
    }
    if (back) {
        cameraPosition -= cameraDirection * speed;
    }
    if (right) {
        cameraPosition += glm::cross(cameraDirection, upDirection) * speed;
    }
    if (left) {
        cameraPosition -= glm::cross(cameraDirection, upDirection) * speed;
    }
    if (up) {
        cameraPosition += upDirection * speed;
    }
    if (down) {
        cameraPosition -= upDirection * speed;
    }
        
    AudioEngine::setListenerPosition(cameraPosition);
    AudioEngine::setListenerOrientation(cameraDirection);
    
    for(WorldObject* o : objects){
        o->update();
    }
}

void World::addLightSource(PointLight light){
    lights.push_back(light);
}

bool World::loadFromFile(std::string filename) {
    FILE* f;
    f = fopen(filename.c_str(), "r");
    if (f == NULL) {
        std::cout << "Couldn't load the map!" << std::endl;
    } else {
        rapidjson::Document doc;
        rapidjson::FileStream is(f);
        doc.ParseStream<0>(is);
        rapidjson::Value& props = doc["props"];
        for (rapidjson::SizeType i = 0; i < props.Size(); i++) {
            Prop* prop = new Prop(getProp(props[i]["prop"].GetString()));
            prop->setPosition(getVec3(props[i]["pos"]));
            prop->setRotation(getVec3(props[i]["rot"]));
            for(PropModule* mod : prop->modules){
                mod->loadModule(prop, this, props[i]["modules"][mod->getName().c_str()]);
            }
            objects.push_back(prop);
        }
        terrain = new Terrain(doc["terrain"].GetString(), doc["sizeX"].GetInt(), doc["sizeY"].GetInt(), doc["sizeZ"].GetInt());
        fclose(f);
    }
}

bool World::processEvent(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                frwd = true;
                break;
            case SDLK_s:
                back = true;
                break;
            case SDLK_a:
                left = true;
                break;
            case SDLK_d:
                right = true;
                break;
            case SDLK_SPACE:
                up = true;
                break;
            case SDLK_c:
                down = true;
                break;
            case SDLK_LSHIFT:
                speed = 1.0f;
                break;
            case SDLK_LCTRL:
                speed = 0.01f;
                break;
            case SDLK_PAGEUP:
                ambientLight += 0.005f;
                break;
            case SDLK_PAGEDOWN:
                ambientLight -= 0.005f;
                break;
            case SDLK_e:
                //test interaction implementation, use raycasting later on
                for(WorldObject* o : objects){
                    if(glm::length(o->position - cameraPosition) <= 2){
                        o->interact();
                        break;
                    }
                }
                break;
            default:
                return false;
        }
    } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                frwd = false;
                break;
            case SDLK_s:
                back = false;
                break;
            case SDLK_a:
                left = false;
                break;
            case SDLK_d:
                right = false;
                break;
            case SDLK_LSHIFT:
                speed = 0.1f;
                break;
            case SDLK_LCTRL:
                speed = 0.1f;
                break;
            case SDLK_SPACE:
                up = false;
                break;
            case SDLK_c:
                down = false;
                break;
            default:
                return false;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        yaw += (event.motion.xrel) * 0.1f;
        pitch += (event.motion.yrel) * -0.1f;
        if(pitch > 89.0f)
            pitch =  89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
    } else {
        return false;
    }
    return true;
}