#include "wav.h"
//so, wtf is it about the "resizing the window" bug? Why do we need to resize the window?

using namespace std;
const long int nS [] = {
  15938014, 12237750, 34415640, 27689472, 19321344, 27452160, 22378752, 28900200, 35453952, 43538688, 43811880, 38486016, 36897000, 34564608, 22235904, 17616384, 34154496
};

float param;
std::vector<glMusicSphere *> mySpheres;

int audioCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
void* userData){

  AudioDecoder* pAudioDecoder = static_cast<AudioDecoder*>(userData);
  memset(output, 0, frameCount * NUM_CHANNELS * sizeof(float)); //protect your ears.

  // Grab the number of samples that PortAudio said it needs to send to soundcard.
  int samplesRead = pAudioDecoder->read(frameCount * NUM_CHANNELS, static_cast<SAMPLE*>(output));
  int ci = 0;
  int i = 0;
  float rmax, gmax, bmax;
  float rmin, gmin, bmin;
  float dx, dy, dz;
  float x, y, z, xmin, xmax, ymin, ymax, zmin, zmax;
  std::vector<glMusicSphere *>::iterator it;
  for(it=mySpheres.begin(); it!=mySpheres.end(); it++){
    dx = ((float *)output)[ci++];// +((*it)->x.x) ;
    dy = ((float *)output)[ci++];// +((*it)->x.x);
    dz = ((float *)output)[ci++];

    ((*it)->x.x) += dx; //((float *)output)[ci++];// +((*it)->x.x) ;
    ((*it)->x.y) += dy;//((float *)output)[ci++];// +((*it)->x.x);
    ((*it)->x.z) += dz;//((float *)output)[ci++];// +((*it)->x.z);
    //ci++;
    ((*it)->rgb.x) +=(- ((float *)output)[ci++]);//+=abs( ((*it)->x.x));
    ((*it)->rgb.y) +=(- ((float *)output)[ci++]);//)+=abs( ((*it)->x.y));
    ((*it)->rgb.z) +=(+ ((float *)output)[ci++]);//+=abs( ((*it)->x.z));
    ((*it)->rgb.x) = ((*it)->x.x);// (double)fmod( (double)((*it)->rgb.x), 1.);
    ((*it)->rgb.y) = ((*it)->x.y);//(double)fmod( (double)((*it)->rgb.y), 1.);
    ((*it)->rgb.z) = ((*it)->x.z);//(double)fmod( (double)((*it)->rgb.z), 1.);

    /*float sum = ((*it)->rgb.x) + ((*it)->rgb.y) + ((*it)->rgb.z);
    ((*it)->rgb.x)/=sum;
    ((*it)->rgb.y)/=sum;
    ((*it)->rgb.z)/=sum;*/

    i++;
  }
  for(it=mySpheres.begin(); it!=mySpheres.end(); it++){
    float r = ((*it)->rgb.x);
    float g = ((*it)->rgb.y);
    float b = ((*it)->rgb.z);
    if(it == mySpheres.begin()){
      rmax=rmin=r; gmax=gmin=g; bmax=bmin=b;
    }
    if(r>rmax)rmax=r;
    if(r<rmin)rmin=r;
    if(g>gmax)gmax=g;
    if(g<gmin)gmin=g;
    if(b>bmax)bmax=b;
    if(b<bmin)bmin=b;

    x = ((*it)->x.x);
    y = ((*it)->x.y);
    z = ((*it)->x.z);
    if(it == mySpheres.begin()){
      xmax=xmin=x; ymax=ymin=y; zmax=zmin=z;
    }
    if(x>xmax)xmax=x;
    if(x<xmin)xmin=x;
    if(y>ymax)ymax=y;
    if(y<ymin)ymin=y;
    if(z>zmax)zmax=z;
    if(z<zmin)zmin=z;
  }

  for(it=mySpheres.begin(); it!=mySpheres.end(); it++){
    float r = ((*it)->rgb.x);
    float g = ((*it)->rgb.y);
    float b = ((*it)->rgb.z);
    ((*it)->rgb.x) = (r-rmin)/(rmax-rmin);//((*it)->x.x);// (double)fmod( (double)((*it)->rgb.x), 1.);
    ((*it)->rgb.y) = (g-gmin)/(gmax-gmin);//((*it)->x.y);//(double)fmod( (double)((*it)->rgb.y), 1.);
    ((*it)->rgb.z) = (b-bmin)/(bmax-bmin);//((*it)->x.z);//(double)fmod( (double)((*it)->rgb.z), 1.);

    if(false){
      ((*it)->x.x) = ((*it)->x.x - xmin)/(xmax-xmin);//((*it)->x.x);// (double)fmod( (double)((*it)->rgb.x), 1.);
      ((*it)->x.y) = ((*it)->x.y - ymin)/(ymax-ymin);//((*it)->x.y);//(double)fmod( (double)((*it)->rgb.y), 1.);
      ((*it)->x.z) = ((*it)->x.z - zmin)/(zmax-zmin);
    }
    (*it)->parentZprInstance->mark();
  }

  /*
  float sumx, sumy, sumz;
  sumx=sumy=sumz=0.;
  t)->x.xfor(it=mySpheres.begin(); it!=mySpheres.end(); it++){
    sumx+= abs(((*it)->x.x));//= ((float *)output)[ci++] +((*it)->x.z) ;
    sumy+= abs(((*it)->x.y));//= ((float *)output)[ci++] +((*it)->x.x);
    sumz+= abs(((*it)->x.z));//= ((float *)output)[ci++] +((*it)->x.y);
  }
  sumx+=(sumy+sumz);
  for(it=mySpheres.begin(); it!=mySpheres.end(); it++){
    (((*it)->x.x))/=sumx;//= ((float *)output)[ci++] +((*it)->x.z) ;
    (((*it)->x.y))/=sumy;//= ((float *)output)[ci++] +((*it)->x.x);
    (((*it)->x.z))/=sumz;//= ((float *)output)[ci++] +((*it)->x.y);
  }

  */

  //it =mySpheres.begin();
  //(*it)->parentZprInstance->mark();
  //(*it)->parentZprInstance->Update= true;
  return paContinue;
}

/*

int seek(int sampleIdx);
// Read a maximum of 'size' samples of audio into buffer.
// Samples are always returned as 32-bit floats, with stereo interlacing.
// Returns the number of samples read.
int read(int size, const SAMPLE *buffer);
// Get the number of audio samples in the file. This will be a good estimate of the
// number of samples you can get out of read(), though you should not rely on it
// being perfectly accurate always. (eg. it might be slightly inaccurate with VBR MP3s)//
inline int numSamples() const;
// Get the number of channels in the audio file
inline int channels() const;
// Get the sample rate of the audio file (samples per second)
inline int sampleRate() const;
// Get the duration of the audio file (seconds)
inline float duration() const;
// Get the current playback position in samples
inline int positionInSamples() const;
// Get a list of the filetypes supported by the decoder, by extension
static std::vector<std::string> supportedFileExtensions()
*/

int main (int argc, char * argv[]) {
  int nFiles = argc-1;
  int i;
  wav** wavArray;
  SA< SA<float> * > dat(nFiles);
  wavArray = (wav** ) (void *)malloc(sizeof(wav *)*nFiles);
  wav * myWav;
  long int tcs = 0;
  for(i=0; i<nFiles; i++){
    tcs+=nS[i];
    myWav = new wav( argv[i+1] );
    dat[i] = new SA<float>( (nS[i]) * NUM_CHANNELS);
    wavArray[i] = myWav;
    long int myNS = myWav->open();
    //printf("ns %ld\n", myNS);
    myWav->pAudioDecoder->read( (((int)nS[i]) * NUM_CHANNELS), static_cast<SAMPLE*>( (void *) (&((dat[i])->at(0))) ));

    //myWav->getNSamples();
    //myWav->open();
  }
  printf("Finished reading..\n");

  myWav = wavArray[0];
  myWav->open();

  if (Pa_Initialize() != paNoError) {
    // Initialize the PortAudio library.
    std::cerr << "Failed to initialize PortAudio." << std::endl; return 1;
  };
  PaStream* pStream = NULL;
  if (Pa_OpenDefaultStream(&pStream, // Open the PortAudio stream (opens the soundcard device).
  0, // No input channels
  2, // 2 output channel
  paFloat32, // Sample format (see PaSampleFormat)
  44100, // Sample Rate
  paFramesPerBufferUnspecified, // Frames per buffer
  &audioCallback,
  static_cast<void*>(myWav->pAudioDecoder)) != paNoError){
  std::cerr << "Failed to open the default PortAudio stream." << std::endl;return 1;}

  // Start the audio stream. PortAudio will then start calling our callback function
  // every time the soundcard needs audio.
  // Note that this is non-blocking by default!
  if (Pa_StartStream(pStream) != paNoError){
    std::cerr << "Failed to start the PortAudio stream." << std::endl; return 1;
  }
  //return control to program..
  cout <<"Open zprinstance."<<endl;
  zprManager * myManager = zprManager::Instance(argc,argv);

  int NRow = 500; int NCol = 500; const int NBand = 3;
  zprInstance * myZPR = myManager->newZprInstance(NRow, NCol, NBand);
  /*
  float sphereR, sphereG, sphereB; sphereR = sphereB = sphereG = .7;
  float x,y,z; x=y=z=0.;
  glBasicSphere * newSphere = new glBasicSphere(0,myZPR, x,y,z, sphereR, sphereG, sphereB,0.45, 10,10);
  newSphere->isLinkd = false;
  glBasicSphere * newSphere2 = new glBasicSphere(0,myZPR, x+1.,y,z, sphereR, sphereG, sphereB,0.45, 10,10);
  newSphere2->isLinkd = false;
  glBasicSphere * newSphere3 = new glBasicSphere(0,myZPR, x-1.,y,z-3, sphereR, sphereG, sphereB,0.45, 10,10);
  newSphere3->isLinkd = false;
  */

  long int cS=0;
  float cor=0.;
  int nPairs = 66;
  for(i=0; i<nPairs; i++){
    cor = ((float)cS)/((float)tcs);
    //cor =1.;
    glMusicSphere * a = new glMusicSphere( myZPR, cor-1, 2*i-1, 2*i, nS[0] /*ns[i]*/, 44100,tcs, 1, 0, (float)i,-(float)(i));
    cS+=nS[i];
    cor = ((float)cS)/((float)tcs);
    glMusicSphere * b = new glMusicSphere( myZPR, cor, i, (2*i)+1, nS[0]/*ns[i]*/, 44100,tcs, 0,0, (float)i,-((float)i));
    mySpheres.push_back(a);
    mySpheres.push_back(b);
  }

  std::vector<glMusicSphere *>::iterator it;
  for(it=mySpheres.begin(); it!=mySpheres.end(); it++){
    ((*it)->x.x) =0.;//+= ((float *)output)[ci++];// +((*it)->x.x) ;
    ((*it)->x.y) =0.;//+= ((float *)output)[ci++];// +((*it)->x.x);
    ((*it)->x.z) =0.;//+= ((float *)output)[ci++];// +((*it)->x.z);
  }

  glutMainLoop();

  // Shutdown:stop PortAudio stream (closes soundcard.
  if (Pa_StopStream(pStream) != paNoError){
    std::cerr << "Failed to stop the PortAudio stream." << std::endl; return 1;
  }

  // Tell the PortAudio library that we're done
  if (Pa_Terminate() != paNoError){
    std::cerr << "Failed to terminate PortAudio." << std::endl; return 1;
  }

  // Close the AudioDecoder object and free it.
  //delete pAudioDecoder;

  return 0;
}

/*

// IMPORTANT:
// Note that in a real application, you will probably want to call read()
// in a separate thread because it is NOT realtime safe. That means it does
// not run in constant time because it might be allocating memory, calling
// system functions, or doing other things that can take an
// unpredictably long amount of time.
//
// The danger is that if read() takes too long, then audioCallback() might not
// finish quickly enough, and there will be a "dropout" or pop in the audio
// that comes out your speakers.
//
// If you want to guard against dropouts, you should either decode the entire file into
// memory or decode it on-the-fly in a separate thread. To implement the latter, you would:
// - Set up a secondary thread and read() a few seconds of audio into a ringbuffer
// - When the callback runs, you want to fetch audio from that ringbuffer.
// - Have the secondary thread keep read()ing and ensuring the ringbuffer is full.
//
// Ross Bencina has a great introduction to realtime programming and goes into
// more detail here:
// http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing

class AudioDecoder
AudioDecoder(const std::string filename);
int open();
int seek(int sampleIdx);
// Read a maximum of 'size' samples of audio into buffer.
// Samples are always returned as 32-bit floats, with stereo interlacing.
// Returns the number of samples read.
int read(int size, const SAMPLE *buffer);
// Get the number of audio samples in the file. This will be a good estimate of the
number of samples you can get out of read(), though you should not rely on it
being perfectly accurate always. (eg. it might be slightly inaccurate with VBR MP3s)//
inline int numSamples() const;
// Get the number of channels in the audio file
inline int channels() const;
// Get the sample rate of the audio file (samples per second)
inline int sampleRate() const;
// Get the duration of the audio file (seconds)
inline float duration() const;
// Get the current playback position in samples
inline int positionInSamples() const;
// Get a list of the filetypes supported by the decoder, by extension
static std::vector<std::string> supportedFileExtensions()

// Declaration for audio callback called by PortAudio.
//int audioCallback(const void *input, void *output, unsigned long frameCount,const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);

*/