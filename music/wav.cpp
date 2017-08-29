#include "wav.h"
wav::wav( char * _filename){
  // Initialize AudioDecoder object and open demo.mp3
  filename = _filename;
}
long int wav::open(){
  std::string fn(filename);
  pAudioDecoder = new AudioDecoder(fn);
  if (pAudioDecoder->open() != AUDIODECODER_OK){
    std::cerr << "Failed to open " << fn << std::endl; return 0 ;
  }
  sampleRate = pAudioDecoder->sampleRate();
  return(sampleRate);
}
void wav::close(){
  delete pAudioDecoder;
}
long int wav::getNSamples(){
  this->open();
  long int retVal=0;
  float output[NUM_CHANNELS];
  int nr; long int tr=0;
  do{
    nr = pAudioDecoder->read(NUM_CHANNELS, static_cast<SAMPLE*>(output));
    if(nr==NUM_CHANNELS){
      tr+=nr;
    }
  }
  while(nr==NUM_CHANNELS);
  close();
  return tr;
}

glMusicSphere::glMusicSphere( zprInstance * parent, float X, int ind, int i, long int nS, long int sR, long int tcs, int isStart, int isMarker, float Y, float Z) : glBasicSphere( i, parent, X,0.,0., (isStart?0.:1.) ,.5,.5,0.1,10,10 ){
  myType = new std::string("glMusicSphere");
  isS = isStart;
  isM = isMarker;
  float rr, gg, bb;
  if(isMarker==1){
    rr=0.; gg=0.;bb=1.;
  }
  else{
    if(isStart){
      rr=1.; gg=0.; bb=0.;
    }
    else{
      rr=0.; gg=1.; bb=0.;
    }
  }
  size= .21;
  x.init(X,Y,Z);
  circles=20;
  stacks=20;
  setLabel(i);
  initName(parent,true);
  addPointToBoundingBox(rr,gg,bb);
  parentZprInstance = parent;
  setRelativePosition = 0;

  numberOfSamples = nS;
  sampleRate = sR;
  setRelativePosition =1;

  x.init(X,Y,Z);
}
void glMusicSphere::specialFunc(){
}