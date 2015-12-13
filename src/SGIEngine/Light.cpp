#include "Light.h"

#include <GL/glew.h>

void uploadPointLight(PointLight light, ShaderProgram* shader){
    glUniform3f(shader->getUniform("pointlight.position"), light.position.x, light.position.y, light.position.z);
    glUniform3f(shader->getUniform("pointlight.color"), light.color.r, light.color.g, light.color.b);
    glUniform1f(shader->getUniform("pointlight.constantAttenuation"), light.constantAttenuation);
    glUniform1f(shader->getUniform("pointlight.linearAttenuation"), light.linearAttenuation);
    glUniform1f(shader->getUniform("pointlight.exponentialAttenuation"), light.exponentialAttenuation);
}

void uploadSpotLight(SpotLight light, ShaderProgram* shader){
    glUniform3f(shader->getUniform("spotlight.position"), light.position.x, light.position.y, light.position.z);
    glUniform3f(shader->getUniform("spotlight.direction"), light.direction.x, light.direction.y, light.direction.z);
    glUniform3f(shader->getUniform("spotlight.color"), light.color.r, light.color.g, light.color.b);
    glUniform1f(shader->getUniform("spotlight.coneAngle"), light.coneAngle);
    glUniform1f(shader->getUniform("spotlight.constantAttenuation"), light.constantAttenuation);
    glUniform1f(shader->getUniform("spotlight.linearAttenuation"), light.linearAttenuation);
    glUniform1f(shader->getUniform("spotlight.exponentialAttenuation"), light.exponentialAttenuation);
}