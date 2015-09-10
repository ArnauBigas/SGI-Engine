/* 
 * File:   AudioData.cpp
 * Author: arnau
 * 
 * Created on 7 / de setembre / 2015, 15:17
 */

#include "AudioData.h"
#include <iostream>
#include <map>

AudioData::AudioData(SDL_AudioSpec spec, Uint8* start, Uint32 length){
    this->spec = spec;
    this->start = start;
    end = start + length;
}

AudioData::~AudioData(){
    //Hardcoded nonsense, don't mind me
    SDL_FreeWAV(start);
}
    
size_t AudioData::generateSamples(float* stream, size_t streamLength, size_t audioPos, SampleInfo sampleInfo){
    Uint8* pos = start + audioPos; 
    
    if(pos >= end || pos < start){
        return (size_t)-1;
    }
    
    Uint32 length = (Uint32)streamLength;
    Uint32 lengthLeft = (Uint32)((end - pos)/sampleInfo.pitch);
    length = (length > lengthLeft ? lengthLeft : length);
    
    //only supports 16-bit samples
    Sint16* samples = (Sint16*)pos;
    float sampleIndex = 0;
        
    float factorL = (float)sampleInfo.volume * 1.0f/32768.0f;
    float factorR = factorL;
    if(sampleInfo.panning < 0){
        factorR *= sampleInfo.panning + 1;
    } else {
        factorL *= 1 - sampleInfo.panning;
    }
    for(Uint32 i = 0; i < length; i++){
        stream[i] = (samples[(size_t)sampleIndex]) * (i%2 == 0 ? factorL : factorR);
        sampleIndex += sampleInfo.pitch;
    }
    
    pos = (Uint8*)(samples + (size_t)sampleIndex);
    return (size_t)(pos - start);
}

std::map<std::string, AudioData*> audios;

void addAudioData(std::string name, AudioData* audio){
    audios.insert(std::make_pair(name, audio));
}

AudioData* getAudioData(std::string name){
    return audios.at(name);
}

AudioData* loadAudioFromWAV(std::string filename){
    SDL_AudioSpec spec;
    Uint8* wavStart;
    Uint32 wavLength;
    if(SDL_LoadWAV(filename.c_str(), &spec, &wavStart, &wavLength) == NULL){
        std::cerr << "Couldn't load wav file: " << filename << "Error: " << SDL_GetError() << std::endl;
        return 0;
    }
    return new AudioData(spec, wavStart, wavLength);
}