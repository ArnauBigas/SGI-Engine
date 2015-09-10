/*
 * File:   Terrain.h
 * Author: TheArni
 *
 * Created on 27 de julio de 2015, 17:48
 */

#ifndef TERRAIN_H
#define	TERRAIN_H

#include <SDL_surface.h>

#include "Model.h"

class Terrain {
public:
    Terrain(std::string file, int sizeX, int sizeY, int sizeZ);
    Terrain(const Terrain& orig);
    virtual ~Terrain();

    void render() {
        model->render();
    }
private:
    unsigned int vboid;
    unsigned int iboid;
    unsigned int vaoid;
    Uint8 getPixel(SDL_Surface* surface, int x, int y);
    Model* model;

};

#endif	/* TERRAIN_H */

