/*
 * File:   Texture.h
 * Author: TheArni
 *
 * Created on 12 de abril de 2015, 10:57
 */

#ifndef TEXTURE_H
#define	TEXTURE_H

#include <GL/glew.h>
#include <string>
#include <SDL_surface.h>

using namespace std;

GLuint generateTexture(GLint internalFormat, int width, int height, GLint imageFormat, void* data, GLenum type, GLint mag_filter, GLint min_filter);

GLuint loadTextureFromPNG(string filename);

GLuint getSurfaceFormat(SDL_Surface* surface);

void addTexture(std::string name, unsigned int id);

unsigned int getTexture(std::string name);

#endif	/* TEXTURE_H */

