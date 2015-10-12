/* 
 * File:   Mesh.cpp
 * Author: arnau
 * 
 * Created on 21 / de setembre / 2015, 15:27
 */

#include "Mesh.h"

#include <GL/glew.h>

void Mesh::addPolyGroup(PolyGroup& poly){
    polygroups.push_back(poly);
}

void Mesh::render(){
    for(PolyGroup poly : polygroups){
        uploadMaterialData(poly.material);
        glBindVertexArray(poly.vaoid);
        glDrawElements(GL_TRIANGLES, poly.elements, GL_UNSIGNED_INT, 0);
    }
}