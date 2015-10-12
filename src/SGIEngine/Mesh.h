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

struct PolyGroup{
    Material material;
    unsigned int vaoid;
    unsigned int elements;
};

class Mesh {
public:
    
    void addPolyGroup(PolyGroup& poly);
    
    void render();
    
    PolyGroup& getPolyGroup(int index){
        return polygroups.at(index);
    }
private:
    std::vector<PolyGroup> polygroups;
};

#endif	/* MESH_H */

