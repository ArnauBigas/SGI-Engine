/*
 * File:   GraphicsConfig.h
 * Author: TheArni
 *
 * Created on 16 de febrero de 2015, 22:36
 */

#ifndef GRAPHICSCONFIG_H
#define	GRAPHICSCONFIG_H

#include <string>

struct GraphicsConfig {
    int width = 1280;
    int height = 720;
    bool fullscreen = false;
    double fov = 90;
    bool vsync = true;
    bool borderless = false;
    bool resizeable = true;
    double renderDistance = 200;
    int antialiasing = 16; //Not implemented
    bool bobbing = false; //Not implemented
    int modelDetail = 4; //Not implemented
    int textureDetail = 1024; //Not implemented
    double fog = 5; //Not implemented
    int anisotropicFiltering = 16; //Not implemented
    int shadowmapResolution = 1024;
    int depthBufferPrecision = 24;
    bool doubleBuffering = true; //Probably not implemented. Actually it's forced.
    double nearPlaneClipping = 0.1f;
    bool ssao = true;
};

#endif	/* GRAPHICSCONFIG_H */

