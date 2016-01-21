#include "Light.h"
#include "Config.h"
#include "definitions.h"

#include <GL/glew.h>
#include "Logger.h"
#include "RenderEngine.h"

ShadowMapping::ShadowMapping(unsigned int &target, std::string shader) : RenderingTechnique(target, shader){
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &target);
    glBindTexture(GL_TEXTURE_2D, target);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, Config::graphics.shadowmapResolution, Config::graphics.shadowmapResolution, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, 0);
    glDrawBuffer(GL_NONE); // No color buffer is drawn to.
    
    // Check framebuffer
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        Logger::error << "There was an error while creating the framebuffer" << std::endl;
    }
}

void ShadowMapping::enable(Camera* cam){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderEngine::setCurrentShader(getShader(shader));
}

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
    glActiveTexture(GL_TEXTURE0 + SHADOWMAPTEXTUREUNIT);
    glBindTexture(GL_TEXTURE_2D, light.shadowMap);
    glUniformMatrix4fv(shader->getUniform("depthBiasVP"), 1, GL_FALSE, &(light.depthBiasVP)[0][0]);
}