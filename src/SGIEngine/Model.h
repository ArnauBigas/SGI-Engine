/*
 * File:   Model.h
 * Author: TheArni
 *
 * Created on 25 de junio de 2015, 23:05
 */

#ifndef MODEL_H
#define	MODEL_H

#include <string>
#include <vector>
#include <map>
#include <mat4x4.hpp>

#include "Mesh.h"

class Model {
public:    
    /**
     * Load a "fully" supported collada object.
     * @return true if the loading was successful.
     */
    bool loadCollada(std::string filename);

    void render(glm::mat4 transform);

    Mesh getMesh(std::string name);
private:
    std::map<std::string, std::pair<glm::mat4, Mesh>> meshes;
};

#endif	/* MODEL_H */

