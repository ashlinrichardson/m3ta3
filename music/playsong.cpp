/*  music: playsong */
#include <iostream>
#include "audiodecoder.h" // libaudiodecoder
#include "../include/portaudio.h"
#include "../SA.h"
#include "playsong.h"
#include "music.h"


const int NUM_CHANNELS = 2; //stereo
using namespace std;
const long int nS [] = {15938014, 12237750, 34415640, 27689472, 19321344, 27452160, 22378752, 28900200, 35453952, 43538688, 43811880, 38486016, 36897000, 34564608, 22235904, 17616384, 34154496};

wav::wav( char * _filename){    // Initialize an AudioDecoder object and open demo.mp3
    filename = _filename;
    //nSamples=getNSamples();
   // printf("%ld\n", nSamples);
}
long int wav::open(){
    std::string fn(filename);// = "./demo.mp3";
    pAudioDecoder = new AudioDecoder(fn);
    if (pAudioDecoder->open() != AUDIODECODER_OK){ std::cerr << "Failed to open " << fn << std::endl; return 0 ;}
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
        nr =  pAudioDecoder->read(NUM_CHANNELS, static_cast<SAMPLE*>(output));
        if(nr==NUM_CHANNELS) tr+=nr;
    }while( nr==NUM_CHANNELS);
    close();
    return tr;
}


int audioCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
void* userData){
    
    AudioDecoder* pAudioDecoder = static_cast<AudioDecoder*>(userData);    
    memset(output, 0, frameCount * NUM_CHANNELS * sizeof(float));  //protect your ears.
    
    // Grab the number of samples that PortAudio said it needs to send to soundcard. 
    int samplesRead = pAudioDecoder->read(frameCount * NUM_CHANNELS, static_cast<SAMPLE*>(output));
    return paContinue;
}

    /*

int seek(int sampleIdx);
        // Read a maximum of 'size' samples of audio into buffer. 
          //  Samples are always returned as 32-bit floats, with stereo interlacing.
         //   Returns the number of samples read. 
int read(int size, const SAMPLE *buffer);
        // Get the number of audio samples in the file. This will be a good estimate of the 
//            number of samples you can get out of read(), though you should not rely on it
 //           being perfectly accurate always. (eg. it might be slightly inaccurate with VBR MP3s)//
inline int    numSamples()        const;
        // Get the number of channels in the audio file 
inline int    channels()          const;
        // Get the sample rate of the audio file (samples per second) 
inline int    sampleRate()        const;
        // Get the duration of the audio file (seconds) 
inline float  duration()          const;
        // Get the current playback position in samples 
inline int    positionInSamples() const;
        // Get a list of the filetypes supported by the decoder, by extension 
static std::vector<std::string> supportedFileExtensions()
*/



int main (int argc, char * const argv[]) {
    int nFiles = argc-1; 
    int i;
    wav** wavArray;
    SA< SA<float> * > dat(nFiles);
    wavArray = (wav** ) (void *)malloc(sizeof(wav *)*nFiles);
    wav * myWav;
    for(i=0; i<nFiles; i++){  //printf("Malloc %ld\n", nS[i]);
        myWav = new wav( argv[i+1] );
        dat[i] = new SA<float>( (nS[i]) * NUM_CHANNELS); 
        wavArray[i] = myWav;
        long int myNS = myWav->open();
        //printf("ns %ld\n", myNS);
        myWav->pAudioDecoder->read( (((int)nS[i]) * NUM_CHANNELS), static_cast<SAMPLE*>(  (void *) (&((dat[i])->at(0)))     ));

        //myWav->getNSamples();
        //myWav->open();
    }
    printf("Finished reading..\n");

    myWav = wavArray[0];
    myWav->open();

    if (Pa_Initialize() != paNoError) {// Initialize the PortAudio library.
        std::cerr << "Failed to initialize PortAudio." << std::endl; return 1;
    };
    PaStream* pStream = NULL;
    if (Pa_OpenDefaultStream(&pStream,  // Open the PortAudio stream (opens the soundcard device).
                  0, // No input channels
                  2, // 2 output channel
                  paFloat32, // Sample format (see PaSampleFormat)           
                  44100, // Sample Rate
                  paFramesPerBufferUnspecified,  // Frames per buffer 
                  &audioCallback,
                  static_cast<void*>(myWav->pAudioDecoder)) != paNoError)
    {std::cerr << "Failed to open the default PortAudio stream." << std::endl;return 1;}
    
    // Start the audio stream. PortAudio will then start calling our callback function
    // every time the soundcard needs audio.
    // Note that this is non-blocking by default!
    if (Pa_StartStream(pStream) != paNoError){std::cerr << "Failed to start the PortAudio stream." << std::endl; return 1;}               
//return control to program.. 
    sleep(20);
    
    // Shutdown:stop  PortAudio stream (closes soundcard.
    if (Pa_StopStream(pStream) != paNoError){std::cerr << "Failed to stop the PortAudio stream." << std::endl; return 1;}        

    // Tell the PortAudio library that we're done
    if (Pa_Terminate() != paNoError){ std::cerr << "Failed to terminate PortAudio." << std::endl; return 1; }
    
    // Close the AudioDecoder object and free it.
    //delete pAudioDecoder;
    
    return 0;
}

    
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
    //   - Set up a secondary thread and read() a few seconds of audio into a ringbuffer
    //   - When the callback runs, you want to fetch audio from that ringbuffer.
    //   - Have the secondary thread keep read()ing and ensuring the ringbuffer is full.
    //
    // Ross Bencina has a great introduction to realtime programming and goes into
    // more detail here:
    // http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing
    
    
//}

/*
class AudioDecoder {
        AudioDecoder(const std::string filename);
int open();
int seek(int sampleIdx);
        // Read a maximum of 'size' samples of audio into buffer. 
          //  Samples are always returned as 32-bit floats, with stereo interlacing.
         //   Returns the number of samples read. 
int read(int size, const SAMPLE *buffer);
        // Get the number of audio samples in the file. This will be a good estimate of the 
            number of samples you can get out of read(), though you should not rely on it
            being perfectly accurate always. (eg. it might be slightly inaccurate with VBR MP3s)//
inline int    numSamples()        const;
        // Get the number of channels in the audio file 
inline int    channels()          const;
        // Get the sample rate of the audio file (samples per second) 
inline int    sampleRate()        const;
        // Get the duration of the audio file (seconds) 
inline float  duration()          const;
        // Get the current playback position in samples 
inline int    positionInSamples() const;
        // Get a list of the filetypes supported by the decoder, by extension 
static std::vector<std::string> supportedFileExtensions()
    };

*/

// Declaration for audio callback called by PortAudio.
//int audioCallback(const void *input, void *output, unsigned long frameCount,const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);



