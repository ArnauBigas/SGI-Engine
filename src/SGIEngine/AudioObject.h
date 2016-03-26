/* 
 * File:   AudioObject.h
 * Author: arnau
 *
 * Created on 7 / de setembre / 2015, 15:17
 */

#ifndef AUDIOOBJECT_H
#define	AUDIOOBJECT_H

#include "AudioData.h"
#include "SampleInfo.h"


class AudioObject {
public:
    AudioObject(const SampleInfo& info, AudioData* data);
    
    virtual bool generateSamples(float* stream, size_t streamLength);
    
    void setPos(double pos);
protected:
    size_t audioPos;
    size_t audioLength;
    SampleInfo sampleInfo;
    AudioData* audioData;
    
    size_t posToAbsolutePos(double pos);
};

#endif	/* AUDIOOBJECT_H */

