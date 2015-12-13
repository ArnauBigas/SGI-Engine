/*
 * File:   light.h
 * Author: TheArni
 *
 * Created on 15 de julio de 2015, 22:07
 */

#ifndef LIGHT_H
#define	LIGHT_H

#include <vec3.hpp>
#include <string>

#include "Shader.h"

enum LightType{
    POINTLIGHT, SPOTLIGHT, AREALIGHT
};

struct BasePositionalLight {
    glm::vec3 position;
    glm::vec3 color;
    float constantAttenuation;
    float linearAttenuation;
    float exponentialAttenuation;
    LightType type;
};

struct PointLight : BasePositionalLight{
    LightType type = POINTLIGHT;
};

struct SpotLight : BasePositionalLight {
    glm::vec3 direction;
    float coneAngle;
    LightType type = SPOTLIGHT;
};

struct AreaLight : BasePositionalLight {
    glm::vec3 end;
    LightType type = AREALIGHT;
};

void uploadPointLight(PointLight light, ShaderProgram* shader);

void uploadSpotLight(SpotLight light, ShaderProgram* shader);

#endif	/* LIGHT_H */

