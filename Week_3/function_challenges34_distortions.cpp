#include <iostream>
#include <cmath>                    // for the inverse tangent function
#include "MattsAudioHeader.h"       // for the writeWavFile function


float clipDistortion(float inSample, float threshold)
{
    float distortedSample = inSample; // a placeholder for our distorted value
    
    if (distortedSample > threshold)
    {
        distortedSample = threshold;
    }
    else if (distortedSample < -threshold)
    {
        distortedSample = -threshold;
    }
    
    
    return distortedSample;           // return the result back to the main program that called the function
}

float tanhDistortion(float inSample, float gain)
{
    float scaledInput = inSample * gain;
    float distortedSample = std::tanhf(scaledInput);
    return distortedSample;
}
    

int main() {
    float gain = 20;
 
    // set up DSP things
    float duration = 5.0f;
    float sampleRate = 44100;
    int durationInSamples = duration * sampleRate;
    
    // set up a sine oscillator to test our distortion with
    float frequency = 220.0f;            // freq in Hz
    float phase = 0.0;              // keep track of osc phase
    float phaseDelta = frequency  / sampleRate;  // phaseDelta - how much increment the phase by for each sample
    
    // create an empty array that is the same length
    float* samples = new float[durationInSamples];


    // DSP LOOP
    for (int i = 0 ; i < durationInSamples; i++)
    {
        // STEP 1:   PROCESS OSCILLATOR
        // increment the phase a little
        phase += phaseDelta;
        
        // make sure the phase is in the range 0-1
        if (phase > 1.0)
            phase -= 1.0;
        
        // get sine value from phase
        float sineSample = sin( phase * 2 * 3.141593 );
        
        // STEP 2:   APPLY DISTORTION
        // distort the sine sample and write it into the samples array
        //samples[i] = clipDistortion(sineSample, 0.1);
        samples[i] = tanhDistortion(sineSample, gain);
    }
    // END DSP LOOP
    
    // write to wav file (desktop)
    writeToWav(samples, durationInSamples, "challenge_5_3_distortion.wav");

    std::cout << "done playing audio\n";
    
    return 0;
}
