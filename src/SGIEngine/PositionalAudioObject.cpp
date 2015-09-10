/* 
 * File:   PositionalAudioObject.cpp
 * Author: arnau
 * 
 * Created on 7 / de setembre / 2015, 22:27
 */

#include "PositionalAudioObject.h"
#include "AudioEngine.h"

#include <iostream>
#include <glm/geometric.hpp>

bool PositionalAudioObject::generateSamples(float* stream, size_t streamLength){
    SampleInfo adjustedInfo = sampleInfo;
    glm::vec3 direction = position - AudioEngine::getListenerPosition();
    if(glm::length(direction) > 128){
        adjustedInfo.volume = 0;
    } else {
        adjustedInfo.volume = 1-(glm::length(direction)/128.0);
    }
    adjustedInfo.panning = glm::dot(glm::cross(AudioEngine::getListenerOrientation(), glm::vec3(0, 1, 0)), glm::normalize(direction));
    audioPos = audioData->generateSamples(stream, streamLength, audioPos, adjustedInfo);
    if(audioPos == (size_t)-1){
        audioPos = 0;
        return true;
    } else {
        return false;
    }
}

