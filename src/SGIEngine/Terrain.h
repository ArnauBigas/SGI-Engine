/*
 * File:   Terrain.h
 * Author: TheArni
 *
 * Created on 27 de julio de 2015, 17:48
 */

#ifndef TERRAIN_H
#define	TERRAIN_H

#include <SDL2/SDL_surface.h>
#include <glm/mat4x4.hpp>

#include "Model.h"

class Terrain {
public:
    Terrain(std::string file, int sizeX, int sizeY, int sizeZ);
    Terrain(const Terrain& orig);
    virtual ~Terrain();

    void render() {
        model->render(glm::mat4(1.0f));
    }
private:
    unsigned int vboid;
    unsigned int iboid;
    unsigned int vaoid;
    Uint8 getPixel(SDL_Surface* surface, int x, int y);
    Model* model;

};

#endif	/* TERRAIN_H */

