/* Challenges:
 
 1) change the oscillator type so that it is a square wave (±0.5) instead of a sinusoid
 
 2) change the oscillator type so that it is a linear ramp (±0.5) instead of a sinusoid
 
 3) change the oscillator type so that it is a triangle wave (±0.5) instead of a sinusoid
 
 
*/
 
#include <iostream>
#include <cmath>                // for the sin() function
#include "MattsAudioHeader.h"   // for the writeToWav() function

int main(int argc, const char * argv[]) {
    
    int sampleRate = 44100;
    
    int duration = 5;                               // duration in seconds
    int durationInSamples = sampleRate * duration;  // total number of samples for the whole output
    
    float* samples = new float[durationInSamples];  // create an empty array to store all of our samples
    
    float frequency = 60;                          // the frequency of the tone to generate
    
    float phase = 0.0;                                  // keep track of the phase of our oscillator
    float phaseDelta = frequency  / float(sampleRate);  // phaseDelta - how much increment the phase by for each sample
    bool direction = true;
    
    
    // ************
    // OUR DSP loop
    for (int i=0; i<durationInSamples; i++)
    {
        // make sure the phase is in the range 0-1
        if(phase + phaseDelta > 1.0f)
            direction = false;
        else if(phase - phaseDelta < -1.0f)
            direction = true;
        
        if(direction == true)
            phase += phaseDelta;
        else
            phase -= phaseDelta;
            
        float sample = phase;
        
        // add this sample to our samples array (also halving the volume)
        samples[i] = 0.5 * sample;
        
    }
    // END OF DSP LOOP
    // ***********
    
    // write to wav file (desktop)
    writeToWav(samples, durationInSamples, "challenge_4_4_osc_types.wav");
    
    
    std::cout << "Done!\n";
    return 0;
}
