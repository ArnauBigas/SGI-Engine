/*
 * File:   Terrain.cpp
 * Author: TheArni
 *
 * Created on 27 de julio de 2015, 17:48
 */

#include "Terrain.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "Model.h"
#include "definitions.h"

Terrain::Terrain(std::string file, int sizeX, int sizeY, int sizeZ) {
    std::cout << "loading terrain" << std::endl;

    SDL_Surface* img = IMG_Load((FMAPS"world/"+file).c_str());

    std::cout << "bits per pixel: " << unsigned(img->format->BitsPerPixel) << std::endl;

    std::vector<float> data;
    std::vector<unsigned int> indices;

    int width = img->w;
    int height = img->h;
    
    float scaleX = width / (float) sizeX;
    float scaleY = 256 / (float) sizeY;
    float scaleZ = height / (float) sizeZ;

    glm::vec3** vertices = new glm::vec3*[width];
    for (int i = 0; i < width; i++) {
        vertices[i] = new glm::vec3[height];
    }
    glm::vec3*** trigNormals = new glm::vec3**[2];
    for (int i = 0; i < 2; i++) {
        trigNormals[i] = new glm::vec3*[width - 1];
        for (int j = 0; j < width - 1; j++) {
            trigNormals[i][j] = new glm::vec3[height - 1];
        }
    }
    
    std::cout << "loading height data" << std::endl;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            vertices[x][y] = glm::vec3(x / scaleX, getPixel(img, x, y) / scaleY, y / scaleZ);
        }
    }

    SDL_FreeSurface(img);

    std::cout << "calculating face normals" << std::endl;
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            glm::vec3 trig0[3] = {
                vertices[x][y],
                vertices[x + 1][y + 1],
                vertices[x][y + 1]
            };

            glm::vec3 trig1[3] = {
                vertices[x][y],
                vertices[x + 1][y],
                vertices[x + 1][y + 1]
            };

            glm::vec3 trigNorm0 = glm::cross(trig0[1] - trig0[0], trig0[2] - trig0[0]);
            glm::vec3 trigNorm1 = glm::cross(trig1[1] - trig1[0], trig1[2] - trig1[0]);

            trigNormals[0][x][y] = glm::normalize(trigNorm0);
            trigNormals[1][x][y] = glm::normalize(trigNorm1);
        }
    }

    std::cout << "calculating vertex normals and uploading data" << std::endl;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            glm::vec3 normal(0, 0, 0);
            if (x != 0 && y != 0) {
                normal += trigNormals[0][x - 1][y - 1];
                normal += trigNormals[1][x - 1][y - 1];
            }
            if (x != 0 && y != height - 1) {
                normal += trigNormals[0][x - 1][y];
                normal += trigNormals[1][x - 1][y];
            }
            if (x != width - 1 && y != 0) {
                normal += trigNormals[0][x][y - 1];
                normal += trigNormals[1][x][y - 1];
            }
            if (x != width - 1 && y != height - 1) {
                normal += trigNormals[0][x][y];
                normal += trigNormals[1][x][y];
            }

            //normal is inverted for some reason
            normal = glm::normalize(normal) * -1.0f;

            data.push_back(vertices[x][y].x);
            data.push_back(vertices[x][y].y);
            data.push_back(vertices[x][y].z);

            data.push_back(normal.x);
            data.push_back(normal.y);
            data.push_back(normal.z);

            data.push_back(x);
            data.push_back(y);

            if (x < width - 1 && y < height - 1) {
                indices.push_back((y * width) + x);
                indices.push_back(((y + 1) * width) + x + 1);
                indices.push_back((y * width) + x + 1);


                indices.push_back((y * width) + x);
                indices.push_back(((y + 1) * width) + x);
                indices.push_back(((y + 1) * width) + x + 1);
            }
        }
    }

    delete[] trigNormals;
    delete[] vertices;

    std::cout << "terrain loaded (" << indices.size() << " indices)" << std::endl;

    model = new Model(data, indices);
}

Terrain::Terrain(const Terrain& orig) {
}

Terrain::~Terrain() {
    delete model;
}

Uint8 Terrain::getPixel(SDL_Surface* surface, int x, int y) {
    Uint8 *pixels = (Uint8*) surface->pixels;
    return pixels[(y * surface->w) + x];
}
