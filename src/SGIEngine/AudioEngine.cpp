/* 
 * File:   AudioEngine.cpp
 * Author: arnau
 * 
 * Created on 7 / de setembre / 2015, 13:44
 */

#include "AudioEngine.h"
#include "PositionalAudioObject.h"

#include <vector>
#include <iostream>

SDL_AudioDeviceID device;
std::vector<float> stream;
std::vector<AudioObject*> playingAudio;

glm::vec3 listenerPosition(0.0f, 0.0f, 0.0f);
glm::vec3 listenerOrientation(0.0f, 0.0f, 0.0f);

SDL_AudioSpec spec;

SDL_AudioSpec& AudioEngine::getAudioSpec(){
    return spec;
}

void AudioEngine::setListenerPosition(glm::vec3 position){
    listenerPosition = position;
}

glm::vec3 AudioEngine::getListenerPosition(){
    return listenerPosition;
}

void AudioEngine::setListenerOrientation(glm::vec3 orientation){
    listenerOrientation = orientation;
}

glm::vec3 AudioEngine::getListenerOrientation(){
    return listenerOrientation;
}

bool removeAudio(AudioObject& audio){
    std::vector<AudioObject*>::iterator it = playingAudio.begin();
    std::vector<AudioObject*>::iterator end = playingAudio.end();
    for(; it != end; ++it){
        if(*it == &audio){
            playingAudio.erase(it);
            return true;
        }
    }
    return false;
}

bool AudioEngine::init(){    
    SDL_zero(spec);
    spec.freq = 44800;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.samples = 2048;
    spec.callback = &AudioEngine::generateSamples;
    
    device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    if(device == 0){
        std::cerr << "Couldn't get audio device: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_PauseAudioDevice(device, 0);
    return true;
}

void AudioEngine::kill(){
    SDL_CloseAudioDevice(device);
}

void AudioEngine::playAudio(AudioObject& audio){
    SDL_LockAudioDevice(device);
    
    //prevent duplicates
    removeAudio(audio);
    playingAudio.push_back(&audio);
    
    SDL_UnlockAudioDevice(device);
}

void AudioEngine::playAudio(std::string audio){
    playAudio(*(new AudioObject(SampleInfo(), getAudioData(audio))));
}
    
void AudioEngine::playAudio(std::string audio, glm::vec3 position){
    playAudio(*(new PositionalAudioObject(SampleInfo(), getAudioData(audio), position)));
}

void AudioEngine::pauseAudio(AudioObject& audio){
    SDL_LockAudioDevice(device);
    
    removeAudio(audio);
    
    SDL_UnlockAudioDevice(device);
}

void AudioEngine::stopAudio(AudioObject& audio){
    SDL_LockAudioDevice(device);
    
    if(removeAudio(audio)){
        audio.setPos(0.0);
    }
    
    SDL_UnlockAudioDevice(device);
}
    
void AudioEngine::generateSamples(void*, Uint8* streamIn, int streamInLength){
    size_t streamLen = (size_t)(streamInLength/2);
    
    stream.reserve(streamLen);
    float* floatStream = *(float**)(&stream);
    
    for(size_t i = 0; i < streamLen; i++){
        floatStream[i] = 0.0f;
    }
    
    std::vector<AudioObject*>::iterator it = playingAudio.begin();
    std::vector<AudioObject*>::iterator end = playingAudio.end();
    for(; it != end; ++it){
        if((*it)->generateSamples(floatStream, streamLen)){
            removeAudio(*(*it));
        }
    }
    
    Sint16* stream = (Sint16*)streamIn;
    for(size_t i = 0; i < streamLen; i++){
        float val = floatStream[i];
        
        if(val > 1.0f){
            val = 1.0f;
        } else if( val < -1.0f){
            val = -1.0f;
        }
        
        stream[i] = (Sint16)(val * 32767);
    }
}