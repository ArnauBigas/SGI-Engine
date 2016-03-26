/*
 * File:   Model.h
 * Author: TheArni
 *
 * Created on 25 de junio de 2015, 23:05
 */

#ifndef MODEL_H
#define	MODEL_H

#include <string>
#include <map>
#include <glm/mat4x4.hpp>
#include <list>

#include "Mesh.h"
#include "Animation.h"

class Model {
public:    
    /**
     * Load a "fully" supported collada object.
     * @return true if the loading was successful.
     */
    bool loadCollada(std::string filename);

    void render(glm::mat4 transform);

    Mesh& getMesh(std::string name);
    
    void playAnimation(std::string animation);
private:
    std::map<std::string, std::pair<glm::mat4, Mesh>> meshes;
    std::map<std::string, Animation> animations;
    std::list<Animation*> animationUpdates;
};

#endif	/* MODEL_H */

