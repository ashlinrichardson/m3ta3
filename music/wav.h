#pragma once
#ifndef __WAV_H
#define __WAV_H

#define sq(x) ((x)*(x))
#define for0(j,n) for(j=0; j<(n); j++)
#include "../glut2/newzpr.h"
#include <iostream>
#include "audiodecoder/audiodecoder.h" // libaudiodecoder
#include "../include/portaudio.h"
#include "../SA.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <pthread.h>
#include <cfloat>
#include <queue>
#include <set>
#include <map>
#include <stack>
#include <fstream>
#include <iostream>
#include "../SA.h"
#include "../image/image.h"
#include <float.h>

const int NUM_CHANNELS = 2; //stereo

class wav{
  public:
  char * filename;
  long int nSamples, nChannels, sampleRate;
  float duration; //check this
  AudioDecoder* pAudioDecoder;
  wav( char * fn);
  long int open();
  void close();
  long int getNSamples();
};

class glMusicSphere;

class glMusicSphere: public glBasicSphere{
  public:
  long int numberOfSamples, sampleRate;
  int isS, isM;
  glMusicSphere(zprInstance * parent, float X, int ind, int i, long int nS, long int sR, long int tcs, int isStart, int isMarker,float Y, float Z);
  void specialFunc();

};

#endif
