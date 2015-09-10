/* 
 * File:   AudioData.h
 * Author: arnau
 *
 * Created on 7 / de setembre / 2015, 15:17
 */

#ifndef AUDIODATA_H
#define	AUDIODATA_H

#include "SampleInfo.h"
#include <SDL_audio.h>
#include <string>

class AudioData {
public:
    AudioData(SDL_AudioSpec spec, Uint8* start, Uint32 length);
    
    virtual ~AudioData();
    
    size_t generateSamples(float* stream, size_t streamLength, size_t audioPos, SampleInfo sampleInfo);
    
    size_t getAudioLength(){
        return (size_t)(end - start);
    }
private:
    SDL_AudioSpec spec;
    Uint8* start;
    Uint8* end;
};

void addAudioData(std::string name, AudioData* audio);
AudioData* getAudioData(std::string name);

AudioData* loadAudioFromWAV(std::string filename);

#endif	/* AUDIODATA_H */

