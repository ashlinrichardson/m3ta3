#ifndef __PLAYSONG_H
#define __PLAYSONG_H
#endif
#pragma once

#include "../glut2/newzpr.h"
class wav{
public:
    char * filename;
    long int nSamples, nChannels, sampleRate;
    float duration; //check this
    AudioDecoder* pAudioDecoder;
    wav(char * fn);
    long int open();
    void close();
    long int getNSamples();
};

