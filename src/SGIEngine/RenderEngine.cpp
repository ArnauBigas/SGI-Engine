/*
 * File:   .cpp
 * Author: TheArni
 *
 * Created on 4 de marzo de 2015, 16:17
 */

#include "RenderEngine.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL.h>
#include <GL/glu.h>
#include <iostream>
#include <gtx/transform.hpp>
#include <SDL_image.h>
#include <map>
#include <algorithm>

#include "definitions.h"
#include "Config.h"
#include "Texture.h"
#include "Model.h"
#include "Text.h"
#include "Logger.h"

std::string windowTitle;
SDL_Window* window;
SDL_GLContext glContext;

glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;

glm::mat4 projection2D;

ShaderProgram* shader2D;
ShaderProgram* shaderCurrent;

std::map<char, glm::vec2> uvCoords;
TTF_Font* font;
unsigned int glyphTextureMap;
unsigned int stringVao;
unsigned int stringVbo;

std::vector<Camera *> cams;
Camera* currentCamera = 0;

bool RenderEngine::init(std::string title) {
    windowTitle = title;

    //Initialize the TTF SDL helper library
    if (TTF_Init() < 0) {
        printf("TTF SDL Library could not initialize! SDL_TTF error: %s\n", TTF_GetError());
        return false;
    }

    //Initialize the PNG SDL helper library for PNGs
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("IMG SDL Library could not initialize! SDL_image error: %s\n", IMG_GetError());
        return false;
    }

    //Load the main font for the game engine
    font = TTF_OpenFont(FRESOURCES"vera.ttf", 12);
    if (font == NULL) {
        printf("Font could not be loaded: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

bool RenderEngine::initGL() {

    //SETUP WINDOW//==========================================================//

    int flags = SDL_WINDOW_OPENGL;
    if (Config::graphics.fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    if (Config::graphics.borderless) {
        flags |= SDL_WINDOW_BORDERLESS;
    }
    if (Config::graphics.resizeable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }

    window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Config::graphics.width, Config::graphics.height, flags);
    if (window == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, Config::graphics.depthBufferPrecision);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    glContext = SDL_GL_CreateContext(window);
    if (glContext == NULL) {
        printf("GL Context could not be created! SDL Error: %s\n",
                SDL_GetError());
        return false;
    }

    if (Config::graphics.vsync) {
        SDL_GL_SetSwapInterval(1);
    } else {
        SDL_GL_SetSwapInterval(0);
    }

    glewExperimental = true;
    GLenum err = glewInit();

    if (err != GLEW_NO_ERROR) {
        fprintf(stderr, "GLEW Init Error: %s\n", glewGetErrorString(err));
        return false;
    }

    //LOAD OPENGL STUFF//=====================================================//

    //Clear error buffer
    err = glGetError();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glEnable(GL_SCISSOR_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    projection2D = glm::ortho(0.0f, (float) Config::graphics.width, (float) Config::graphics.height, 0.0f);
    
    err = glGetError();
    if (err != GL_NO_ERROR) {
        Logger::error << "there was an error while initializing OpenGL:" << std::endl;
        Logger::error << err << std::endl;
        return false;
    }

    //GENERATE GLYPH TEXTURE MAP//============================================//

    SDL_Color color = {255, 255, 255};

    std::string text = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!?'=<>+-,.;:\"\\#$·%&/()* ";
    std::string temptext;
    int lastPos = 0;
    for (char& c : text) {
        int xStart = lastPos;
        temptext += c;
        TTF_SizeText(font, temptext.c_str(), &lastPos, NULL);
        uvCoords[c] = glm::vec2(xStart, lastPos);
    }

    SDL_Surface* img = TTF_RenderText_Blended(font, text.c_str(), color);

    Logger::info << "GlyphMap texture resolution is " << img->w << "x" << img->h << std::endl;

    for (char c = 0; unsigned(c) < 256; c++) {
        uvCoords[c].x /= img->w;
        uvCoords[c].y /= img->w;
    }
    
    glyphTextureMap = generateTexture(GL_RGBA, img->w, img->h, GL_RGBA, img->pixels, GL_UNSIGNED_BYTE, GL_NEAREST, GL_NEAREST);

    SDL_FreeSurface(img);

    glGenVertexArrays(1, &stringVao);
    glGenBuffers(1, &stringVbo);

    err = glGetError();
    if (err != GL_NO_ERROR) {
        Logger::error << "there was an error while creating the glyph map:" << std::endl;
        Logger::error << err << std::endl;
        return false;
    }
    
    //TO ACTIVATE H4CK3R MODE UNCOMMENT THIS LINE:
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    return true;
}

void RenderEngine::swapBuffers() {
    SDL_GL_SwapWindow(window);
}

void RenderEngine::setProjectionMatrix(glm::mat4 matrix) {
    projection = matrix;
}

void RenderEngine::setViewMatrix(glm::mat4 matrix) {
    view = matrix;
}

void RenderEngine::setModelMatrix(glm::mat4 matrix) {
    model = matrix;
}

glm::mat4  RenderEngine::getProjectionMatrix(){
    return projection;
}

glm::mat4  RenderEngine::getViewMatrix(){
    return view;
}

void RenderEngine::updateMatrices(){
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv(shaderCurrent->getUniform("MVP"), 1, GL_FALSE, &mvp[0][0]);
}

void RenderEngine::set3D(){
    glEnable(GL_DEPTH_TEST);
}

void RenderEngine::set2D(){
    if(shader2D == 0){
        shader2D = getShader(GUISHADER);
    }
    if(shaderCurrent != shader2D){
        setCurrentShader(shader2D);
    }
    projection = projection2D;
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);
    updateMatrices();
    glDisable(GL_DEPTH_TEST);
}

ShaderProgram* RenderEngine::getCurrentShader(){
    return shaderCurrent;
}

Camera* RenderEngine::getCurrentCamera(){
    return currentCamera;
}
    
void RenderEngine::setCurrentCamera(Camera* cam){
    currentCamera = cam;
}

void RenderEngine::setCurrentShader(ShaderProgram* shader){
    shaderCurrent = shader;
    shaderCurrent->link();
}

TTF_Font* RenderEngine::getFont() {
    return font;
}

unsigned int RenderEngine::getGlyphTextureMap() {
    return glyphTextureMap;
}

glm::vec2 RenderEngine::getGlyphPos(char c) {
    return uvCoords.at(c);
}

void RenderEngine::drawString(std::string s, int x, int y) {
    std::vector<float> data;
    int lastpos = 0;

    int mod = -1;
    int mType = -1;
    
    unsigned char r = 0, g = 0, b = 0;
    
    std::string temptext;
    glm::vec2 uv;
    for (char& c : s) {
        if (mod > -1) {
            if (mType == 0) { //Color
                switch (mod) {
                    case 1:
                        r = c;
                        break;
                    case 2:
                        g = c;
                        break;
                    case 3:
                        b = c;
                        mType = -1;
                        mod = -2;
                        break;
                }
            } else if (mType == 1) { //Style
                //TODO styles
            } else { //Not set
                mType = c-1;
            }
            mod++;
            continue;
        }
        if (c == T_SUB[0]) {
            mod = 0;
            continue;
        }
        glUniform4f(getShader(GUISHADER)->getUniform("modColor"), r/255.f, g/255.f, b/255.f, 1);
        
        int h;
        int w = lastpos;
        temptext += c;
        TTF_SizeText(RenderEngine::getFont(), temptext.c_str(), &lastpos, &h);
        uv = RenderEngine::getGlyphPos(c);

        data.push_back((float) x + w);
        data.push_back((float) y);
        data.push_back(0);

        data.push_back(uv.s);
        data.push_back(0);


        data.push_back((float) x + w);
        data.push_back((float) (y + h));
        data.push_back(0);

        data.push_back(uv.s);
        data.push_back(1);


        data.push_back((float) x + lastpos);
        data.push_back((float) (y + h));
        data.push_back(0);

        data.push_back(uv.t);
        data.push_back(1);

        data.push_back((float) x + w);
        data.push_back((float) y);
        data.push_back(0);

        data.push_back(uv.s);
        data.push_back(0);

        data.push_back((float) x + lastpos);
        data.push_back((float) (y + h));
        data.push_back(0);

        data.push_back(uv.t);
        data.push_back(1);

        data.push_back((float) x + lastpos);
        data.push_back((float) y);
        data.push_back(0);

        data.push_back(uv.t);
        data.push_back(0);
    }
    glBindVertexArray(stringVao);

    glBindBuffer(GL_ARRAY_BUFFER, stringVbo); //Bind VBO
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof (float), &data[0], GL_DYNAMIC_DRAW); //Upload Vertex Data

    //Setup attrib pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 20, BUFFER_OFFSET(12));
    glBindTexture(GL_TEXTURE_2D, RenderEngine::getGlyphTextureMap());
    glDrawArrays(GL_TRIANGLES, 0, s.length()*6); //Draw everything
}

void RenderEngine::kill() {
    /* Delete our opengl context, destroy our window, and shutdown SDL */
    TTF_CloseFont(font);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);    
    TTF_Quit();
    IMG_Quit();
}

void RenderEngine::registerCamera(Camera* camera) {
    cams.push_back(camera);
}

void RenderEngine::unregisterCamera(Camera* camera) {
    cams.erase(std::remove(cams.begin(), cams.end(), camera), cams.end());
}
std::vector<Camera *> RenderEngine::getCameras() { return cams; }

SDL_Window* RenderEngine::getWindow(){
    return window;
}