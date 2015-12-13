/* 
 * File:   FirstPersonCamera.h
 * Author: arnau
 *
 * Created on 12 / de setembre / 2015, 11:47
 */

#ifndef FIRSTPERSONCAMERA_H
#define	FIRSTPERSONCAMERA_H

#include "Camera.h"

#include "World.h"

class FirstPersonCamera : public Camera {
public:
    FirstPersonCamera(World* world, RenderingTechnique* technique);
    
    virtual void enable();
};

#endif	/* FIRSTPERSONCAMERA_H */

