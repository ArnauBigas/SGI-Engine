/*
 * File:   .h
 * Author: TheArni
 *
 * Created on 4 de marzo de 2015, 16:17
 */

#ifndef RENDERENGINE_H
#define	RENDERENGINE_H

#include <string>
#include <SDL2/SDL_ttf.h>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include "Shader.h"

class Camera;

namespace RenderEngine {

    /**
     * Initializes the rendering engine.
     * 
     * <b>Note</b>: this is automatically called when initializing the Game. 
     * 
     * @param title The title of the window.
     * 
     * @return Wether the engine was initialized successfully or not.
     */
    bool init(std::string title);

    /**
     * Initializes the GL context. It's on a separate function than init() for
     * convinience when we want to change graphical configurations.
     * 
     * @return Wether the GL context was initialized successfully or not.
     */
    bool initGL();

    /**
     * Used for double-buffering. Sends the current frame to the display and
     * if vsync is enabled, waits for the next vblank.
     * 
     * It also clears the GL Color and Depth buffers for the next frame.
     */
    void swapBuffers();
    
    /**
     * Sets the current projection matrix that will be multiplied and sent
     * to the shader when updateMatrices() is called.
     * @param matrix The projection matrix that transforms from worldspace to 
     * screenspace
     */
    void setProjectionMatrix(glm::mat4 matrix);
    
    /**
     * Sets the current view matrix that will be multiplied and sent
     * to the shader when updateMatrices() is called.
     * @param matrix The view matrix that translates from worlspace to
     * cameraspace
     */
    void setViewMatrix(glm::mat4 matrix);
    
    /**
     * Sets the current model matrix that will be multiplied and sent
     * to the shader when updateMatrices() is called.
     * @param matrix The model matrix that modifies the object (translates it,
     * rotates it and scales it)
     */
    void setModelMatrix(glm::mat4 matrix);
    
    glm::mat4 getProjectionMatrix();
    
    glm::mat4 getViewMatrix();
    
    /**
     * Multiplies the matrices and sends the MVP matrix to the shader.
     */
    void updateMatrices();
    
    /**
     * Sets up the rendering engine to be ready for drawing 3D geometry.
     */
    void set3D();
    
    /**
     * Sets up the rendering engine to be ready for drawing 2D geometry.
     */
    void set2D();
    
    /**
     * @return The current shader.
     */
    ShaderProgram* getCurrentShader();
    
    Camera* getCurrentCamera();
    
    void setCurrentCamera(Camera* cam);
    
    /**
     * Sets the current shader that's going to be used
     */
    void setCurrentShader(ShaderProgram* shader);

    /**
     * Returns the current font used for the game.
     * 
     * @return a TTF_Font* pointer. 
     */
    TTF_Font* getFont();

    /**
     * Returns the texture id for the glyph map used for string rendering.
     * 
     * @return a unsigned int used for glBindTexture()
     */
    unsigned int getGlyphTextureMap();

    /**
     * Returns values used for calculating the uv coordinates for a textured
     * quad representing a glyph when drawing a string.
     * 
     * @param c The character you want to obtain the coords for.
     * @return a vector containing the x uv coordinates.
     */
    glm::vec2 getGlyphPos(char c);

    /**
     * Draws a string s at the specified x, y coordinates. Make sure you're
     * using the 2D orthographic projection first.
     * 
     * @param s The text to be drawn
     * @param x The x coord on screenspace
     * @param y The y coord on screenspace
     */
    void drawString(std::string s, int x, int y);

    /**
     * Safely stops the engine, making sure to safely dispose of everything.
     */
    void kill();

    /**
     * Registers a camera
     * Shouldn't really have a reason to call. Automagically called by Camera.
     * @param camera
     */
    void registerCamera(Camera *camera);
    
    /**
     * Unregisters a camera
     * Shouldn't really have a reason to call. Automagically called by Camera.
     * @param camera
     */
    void unregisterCamera(Camera *camera);
    
    /**
     * Returns a vector of all registered cameras
     * @return a vector of all registered cameras
     */
    std::vector<Camera *> getCameras();
    
    /**
     * Returns the current window the game is drawing to.
     * @return a SDL_Window pointer
     */
    SDL_Window* getWindow();
};

#endif	/* RENDERENGINE_H */

