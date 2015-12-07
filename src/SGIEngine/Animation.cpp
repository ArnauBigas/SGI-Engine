/* 
 * File:   Animation.cpp
 * Author: arnau
 * 
 * Created on 29 / d’octubre / 2015, 21:57
 */

#include "Animation.h"

#include <gtx/matrix_interpolation.hpp>

glm::mat4 interpolateAnimation(Animation* anim, float deltaTime){
    glm::mat4 ret;
    std::pair<float, glm::mat4> keyframe = anim->keyframes.at(anim->current);
    if(anim->time < anim->keyframes.at(anim->current+1).first){
        std::pair<float, glm::mat4> nextKeyframe = anim->keyframes.at(anim->current+1);
        ret =  glm::interpolate(keyframe.second,
                nextKeyframe.second,
                (anim->time - keyframe.first)/(nextKeyframe.first - keyframe.first));
    } else {
       anim->current++;
       if(anim->current == anim->keyframes.size()-1){
           anim->current = 0;
           anim->time = 0.0f;
           anim->finished = true;
           ret = anim->keyframes.at(anim->current).second;
       } else {
           ret = interpolateAnimation(anim, 0);
       }
    }
    anim->time += deltaTime;
    return ret;
}

