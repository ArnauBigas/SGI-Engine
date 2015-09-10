/*
 * File:   .h
 * Author: TheArni
 *
 * Created on 4 de marzo de 2015, 16:17
 */

#ifndef RENDERENGINE_H
#define	RENDERENGINE_H

#include <string>
#include <SDL_ttf.h>
#include <mat4x4.hpp>
#include <vec2.hpp>

enum RenderPass {
    DIFFUSE, SHADOWMAPPING, POSTFX
};

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
     * Gets the projection matrix with the current graphical settings.
     * Used for 3D rendering using glsl shaders.
     * 
     * @return a 4x4 matrix containing the 3d prespective projection.
     */
    glm::mat4 getProjectionMatrix();

    /**
     * Gets the orthographic projection matrix with the current graphical
     * settings. Used for 2D rendering using glsl shaders.
     * 
     * @return a 4x4 matrix containing the 2d orthographic projection.
     */
    glm::mat4 getOrthoMatrix();

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
};

#endif	/* RENDERENGINE_H */

