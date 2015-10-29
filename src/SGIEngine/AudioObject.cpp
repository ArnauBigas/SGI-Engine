/* 
 * File:   AudioObject.cpp
 * Author: arnau
 * 
 * Created on 7 / de setembre / 2015, 15:17
 */

#include "AudioObject.h"

AudioObject::AudioObject(const SampleInfo& info, AudioData* data) :
        audioPos(0),
        audioLength(data->getAudioLength()),
        sampleInfo(info),
        audioData(data)
{}
    
bool AudioObject::generateSamples(float* stream, size_t streamLength){
    audioPos = audioData->generateSamples(stream, streamLength, audioPos, sampleInfo);
    if(audioPos == (size_t)-1){
        audioPos = 0;
        return !sampleInfo.loop;
    } else {
        return false;
    }
}
    
void AudioObject::setPos(double pos){
    if(pos < 0.0){
        pos = 0.0;
    } else if(pos > 1.0){
        pos = 1.0;
    }
    audioPos = posToAbsolutePos(pos);
}

size_t AudioObject::posToAbsolutePos(double pos){
    return (size_t)(pos * audioLength);
}