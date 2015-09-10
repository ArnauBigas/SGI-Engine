/* 
 * File:   AudioEngine.h
 * Author: arnau
 *
 * Created on 7 / de setembre / 2015, 13:44
 */

#ifndef AUDIOENGINE_H
#define	AUDIOENGINE_H

#include <string>
#include <SDL_audio.h>
#include <vec3.hpp>

#include "AudioObject.h"

namespace AudioEngine {
    bool init();
    void kill();
    
    void playAudio(AudioObject& audio);
    void pauseAudio(AudioObject& audio);
    void stopAudio(AudioObject& audio);
    
    void playAudio(std::string audio);
    
    void playAudio(std::string audio, glm::vec3 position);
    
    void setListenerPosition(glm::vec3 position);
    glm::vec3 getListenerPosition();
    
    void setListenerOrientation(glm::vec3 orientation);
    glm::vec3 getListenerOrientation();
    
    void generateSamples(void*, Uint8* stream, int streamLength);
};

#endif	/* AUDIOENGINE_H */

