/*
 * File:   light.h
 * Author: TheArni
 *
 * Created on 15 de julio de 2015, 22:07
 */

#ifndef LIGHT_H
#define	LIGHT_H

#include <glm/vec3.hpp>
#include <string>

enum LightType{
    POINTLIGHT, SPOTLIGHT
};

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float constantAttenuation;
    float linearAttenuation;
    float exponentialAttenuation;
};

void uploadPointLightArray(PointLight light, unsigned int programID, std::string name, int index);

#endif	/* LIGHT_H */

