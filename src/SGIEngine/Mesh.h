/* 
 * File:   Mesh.h
 * Author: arnau
 *
 * Created on 21 / de setembre / 2015, 15:27
 */

#ifndef MESH_H
#define	MESH_H

#include "Material.h"

#include <vector>
#include <map>

#include <vec3.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>

struct PolyGroup {
    Material material;
    unsigned int vaoid;
    unsigned int elements;
};

class Mesh {
public:

    Mesh() {tmat = glm::mat4(1.f);};
    
    void addPolyGroup(PolyGroup& poly);

    void render();

    PolyGroup& getPolyGroup(int index) {
        return polygroups.at(index);
    }

    glm::mat4 getTransformMatrix();
    void setTransformMatrix(glm::mat4 tmat);
private:
    std::vector<PolyGroup> polygroups;
    glm::mat4 tmat;
};

#endif	/* MESH_H */

