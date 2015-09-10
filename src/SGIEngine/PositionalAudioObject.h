/* 
 * File:   PositionalAudioObject.h
 * Author: arnau
 *
 * Created on 7 / de setembre / 2015, 22:27
 */

#ifndef POSITIONALAUDIOOBJECT_H
#define	POSITIONALAUDIOOBJECT_H

#include "AudioObject.h"

#include <vec3.hpp>

class PositionalAudioObject : public AudioObject {
public:
    PositionalAudioObject(const SampleInfo& info, AudioData* data) : AudioObject(info, data) {};
    PositionalAudioObject(const SampleInfo& info, AudioData* data, glm::vec3 pos) : AudioObject(info, data), position(pos) {};
    
    virtual bool generateSamples(float* stream, size_t streamLength);
    
    void setPosition(glm::vec3 pos){
        position = pos;
    }
private:
    glm::vec3 position;
};

#endif	/* POSITIONALAUDIOOBJECT_H */

