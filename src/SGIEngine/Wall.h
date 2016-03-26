/* 
 * File:   Wall.h
 * Author: arnau
 *
 * Created on 12 / de setembre / 2015, 17:05
 */

#ifndef WALL_H
#define	WALL_H

#include "WorldObject.h"
#include "Model.h"


class Wall : public BaseWorldObject<Wall>{
public:
    Wall(std::string dir);
    
    virtual void render();
    
    virtual void initFromJson(World* world, rapidjson::Value& json);

private:
    float modelLength;
    Model* model;
    std::vector<glm::mat4> matrices;
};

#endif	/* WALL_H */

