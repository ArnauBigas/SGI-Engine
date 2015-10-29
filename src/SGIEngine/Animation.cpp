/* 
 * File:   Animation.cpp
 * Author: arnau
 * 
 * Created on 29 / d’octubre / 2015, 21:57
 */

#include "Animation.h"

#include <gtx/matrix_interpolation.hpp>

glm::mat4 interpolateAnimation(Animation& anim, float deltaTime){
    anim.time += deltaTime;
    std::pair<float, glm::mat4> keyframe = anim.keyframes.at(anim.current);
    if(anim.time < keyframe.first){
        std::pair<float, glm::mat4> nextKeyframe = anim.keyframes.at(anim.current+1);
        return glm::interpolate(keyframe.second,
                nextKeyframe.second,
                (anim.time - keyframe.first)/(nextKeyframe.first - keyframe.first));
    } else {
       anim.current++;
       if(anim.current == anim.keyframes.size()-1){
           anim.current = 0;
           anim.finished = true;
           return anim.keyframes.at(anim.current).second;
       } else {
           return interpolateAnimation(anim, 0);
       }
    }
}

