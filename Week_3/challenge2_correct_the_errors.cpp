// FIND THE ERRORS!
// This code should compile and run, but we don't hear the 5-second tone. Why not?

// The code needs a few changes, and many of the changes relate to one particular issue
// See if you can spot it, and if it is not clear (it is not obvious!) then look at the previous challenge to try to spot the difference



#include <iostream>
#include <cmath>                // for the sin() function
#include "MattsAudioHeader.h"   // for the writeToWav() function

int main() {
    float sampleRate = 44100.0f;
    
    float duration = 5.0f;                               // duration in seconds
    float durationInSamples = sampleRate * duration;
    float* samples = new float[(int)durationInSamples];           // create an empty array to store all of our samples
    
    float frequency = 440.0f;                          // the frequency of the tone to generate
    
    float phase = 0.0f;                                  // keep track of the phase of our oscillator
    float phaseDelta = frequency / sampleRate;  // phaseDelta - how much increment the phase by for each sample
    
    
    // ************
    // OUR DSP loop
    for (int i=0; i<durationInSamples; i++)
    {
        // make sure the phase is in the range 0-1
        if (phase > 1.0f)
        {
            phase -= 1.0f;
        }
        
        // calculate the sine tone value from this phase --> with sin(phase * 2 * pi)
        float sample = std::sin(phase * 2.0f * M_PI);
        
        // add this sample to our samples array (also halving the volume)
        samples[i] = 0.5 * sample;
        
        // increment the phase a little
        phase += phaseDelta;
        
    }
    // END OF DSP LOOP
    // ***********
    
    // write to wav file (desktop)
    writeToWav(samples, durationInSamples, "blah.wav");
    
    
    std::cout << "Done!\n";
    
    return 0;
}
