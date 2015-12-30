#include "Texture.h"

#include <SDL_image.h>
#include <map>

GLuint generateTexture(GLint internalFormat, int width, int height, GLint imageFormat, void* data, GLenum type, GLint mag_filter, GLint min_filter) {
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, type, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

    GLfloat fLargest;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
    return id;
}

GLuint loadTextureFromPNG(string filename) {
    SDL_Surface* img = IMG_Load(filename.c_str());
    GLuint id = generateTexture(GL_RGBA, img->w, img->h, getSurfaceFormat(img), img->pixels, GL_UNSIGNED_BYTE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    SDL_FreeSurface(img);
    return id;
}

GLuint getSurfaceFormat(SDL_Surface* surface) {
    int nOfColors = surface->format->BytesPerPixel;
    if (nOfColors == 4) {
        if (surface->format->Rmask == 0x000000ff) {
            return GL_RGBA;
        } else {
            return GL_BGRA;
        }
    } else if (nOfColors == 3) {
        if (surface->format->Rmask == 0x000000ff) {
            return GL_RGB;
        } else {
            return GL_BGR;
        }
    }
}

std::map<std::string, unsigned int> textures;

void addTexture(std::string name, unsigned int id) {
    textures.insert(std::make_pair(name, id));
}

unsigned int getTexture(std::string name) {
    return textures.at(name);
}