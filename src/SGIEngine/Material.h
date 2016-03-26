/* 
 * File:   Material.h
 * Author: arnau
 *
 * Created on 20 / de setembre / 2015, 20:53
 */

#ifndef MATERIAL_H
#define	MATERIAL_H

#include <glm/vec4.hpp>

struct Material{
    glm::vec4 emission = glm::vec4(0, 0, 0, 1);
    glm::vec4 ambient = glm::vec4(0, 0, 0, 1);
    glm::vec4 diffuse = glm::vec4(1, 1, 1, 1);    
    glm::vec4 specular = glm::vec4(1, 1, 1, 1);
    float shininess = 1;
    unsigned int diffuseTexture = 0;
    unsigned int bumpmapTexture = 0;
    unsigned int specularTexture = 0;
};

void uploadMaterialData(Material mat);

#endif	/* MATERIAL_H */

