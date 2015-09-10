#include "Light.h"

#include <GL/glew.h>

void uploadPointLightArray(PointLight light, unsigned int programID, std::string name, int index) {
    GLint location;
    location = glGetUniformLocation(programID, (name + "[" + std::to_string(index) + "].position").c_str());
    glUniform3f(location, light.position.x, light.position.y, light.position.z);
    location = glGetUniformLocation(programID, (name + "[" + std::to_string(index) + "].color").c_str());
    glUniform3f(location, light.color.r, light.color.g, light.color.b);
    location = glGetUniformLocation(programID, (name + "[" + std::to_string(index) + "].constantAttenuation").c_str());
    glUniform1f(location, light.constantAttenuation);
    location = glGetUniformLocation(programID, (name + "[" + std::to_string(index) + "].linearAttenuation").c_str());
    glUniform1f(location, light.linearAttenuation);
    location = glGetUniformLocation(programID, (name + "[" + std::to_string(index) + "].exponentialAttenuation").c_str());
    glUniform1f(location, light.exponentialAttenuation);
}