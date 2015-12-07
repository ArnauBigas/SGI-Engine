/* 
 * File:   Animation.h
 * Author: arnau
 *
 * Created on 29 / d’octubre / 2015, 21:57
 */

#ifndef ANIMATION_H
#define	ANIMATION_H

#include <vector>
#include <utility>
#include <mat4x4.hpp>
#include <string>

struct Animation{
    //TODO: interpolation types blah blah
    std::string mesh;
    int current = 0;
    float time = 0.0f;
    bool finished = true;
    std::vector<std::pair<float, glm::mat4>> keyframes;
};

//TODO: enable scale interpolation
glm::mat4 interpolateAnimation(Animation* anim, float deltaTime);

#endif	/* ANIMATION_H */

