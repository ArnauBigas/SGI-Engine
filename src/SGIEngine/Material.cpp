/* 
 * File:   Material.cpp
 * Author: arnau
 * 
 * Created on 20 / de setembre / 2015, 20:53
 */

#include "Material.h"

#include <gl/glew.h>
#include <gtc/type_ptr.hpp>

#include "RenderEngine.h"
#include "definitions.h"


//TODO: Shader uniform optimization, don't reupload them.
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
    if(shader->hasUniform("diffuseTexture")){
        glActiveTexture(GL_TEXTURE0 + DIFFUSEID);
        glBindTexture(GL_TEXTURE_2D, mat.diffuseTexture);
        glUniform1i(shader->getUniform("diffuseTexture"), 0);
    }
    if(shader->hasUniform("bumpmapTexture")){
        glActiveTexture(GL_TEXTURE0 + NORMALID);
        glBindTexture(GL_TEXTURE_2D, mat.bumpmapTexture);
        glUniform1i(shader->getUniform("bumpmapTexture"), 1);
    }
    if(shader->hasUniform("specularTexture")){
        glActiveTexture(GL_TEXTURE0 + SPECULARID);
        glBindTexture(GL_TEXTURE_2D, mat.specularTexture);
        glUniform1i(shader->getUniform("specularTexture"), 2);
    }
}