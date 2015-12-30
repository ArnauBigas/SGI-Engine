/*
 * File:   light.h
 * Author: TheArni
 *
 * Created on 15 de julio de 2015, 22:07
 */

#ifndef LIGHT_H
#define	LIGHT_H

#include <mat4x4.hpp>
#include <vec3.hpp>
#include <string>

#include "Shader.h"
#include "RenderingTechnique.h"

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
    unsigned int shadowMap;
    glm::mat4 depthBiasVP;
    LightType type = SPOTLIGHT;
};

struct AreaLight : BasePositionalLight {
    glm::vec3 end;
    LightType type = AREALIGHT;
};

class ShadowMapping : public RenderingTechnique {
public:
    ShadowMapping(unsigned int &target, std::string shader);
    virtual void enable(Camera* cam);
    virtual void disable(Camera* cam){};
    virtual int getType() {return SHADOW_MAPPING;};
    virtual void targetResized(int width, int height){};
private:
    unsigned int framebuffer;
};

void uploadPointLight(PointLight light, ShaderProgram* shader);

void uploadSpotLight(SpotLight light, ShaderProgram* shader);

#endif	/* LIGHT_H */

