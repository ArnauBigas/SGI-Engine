/* 
 * File:   Material.cpp
 * Author: arnau
 * 
 * Created on 20 / de setembre / 2015, 20:53
 */

#include "Material.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "RenderEngine.h"
#include "definitions.h"

void uploadMaterialData(Material mat){
    ShaderProgram* shader = RenderEngine::getCurrentShader();
    if(shader->hasUniform("matEmission")){
        glUniform4fv(shader->getUniform("matEmission"), 1, glm::value_ptr(mat.emission));
    }
    if(shader->hasUniform("matAmbient")){
        glUniform4fv(shader->getUniform("matAmbient"), 1, glm::value_ptr(mat.ambient));
    }
    if(shader->hasUniform("matDiffuse")){
        glUniform4fv(shader->getUniform("matDiffuse"), 1, glm::value_ptr(mat.diffuse));
    }
    if(shader->hasUniform("specular")){
        glUniform4fv(shader->getUniform("specular"), 1, glm::value_ptr(mat.specular));
    }
    if(shader->hasUniform("shininess")){
        glUniform1f(shader->getUniform("shininess"), mat.shininess);
    }
    //not sure if this is a good idea, leaving it uncommented for now
    //if(shader->hasUniform("diffuseTexture")){
        glActiveTexture(GL_TEXTURE0 + DIFFUSETEXTUREUNIT);
        glBindTexture(GL_TEXTURE_2D, mat.diffuseTexture);
    //}
    //if(shader->hasUniform("bumpmapTexture")){
        glActiveTexture(GL_TEXTURE0 + NORMALMAPTEXTUREUNIT);
        glBindTexture(GL_TEXTURE_2D, mat.bumpmapTexture);
    //}
    //if(shader->hasUniform("specularTexture")){
        glActiveTexture(GL_TEXTURE0 + SPECULARMAPTEXTUREUNIT);
        glBindTexture(GL_TEXTURE_2D, mat.specularTexture);
    //}
}