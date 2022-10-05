/* Challenges:
 
 1) update the code so that it plays a sine tone at 660Hz for 10 seconds
 
 2) currently the volume is 0.5. Change it to 0.1 (1 tenth of full volume)
 
 3) set up a new variable for the volume above the frequency variable. Replace the 0.1 with your volume variable.
 
 4) update the code so that after half of the total time has elapsed, the frequency drops to 330 Hz
 
 5) add a second oscillator at a different frequency from the first
 -- -- hint: you'll need to add extra variables for the new osc, including a new phase variable, phaseDelta, and frequency. Make sure these all have different names! e.g. phase2, phaseDelta2.
 -- -- hint: the final sample[i] should be the SUM of the two oscillator values - add them together.
 
*/
 

#include <iostream>
#include <cmath>                // for the sin() function
#include "MattsAudioHeader.h"   // for the writeToWav() function

int main(int argc, const char * argv[]) {
    
    int sampleRate = 44100;
    
    int duration = 5;                               // duration in seconds
    int durationInSamples = sampleRate * duration;  // total number of samples for the whole output
    
    float* samples = new float[durationInSamples];  // create an empty array to store all of our samples
    
    float gain = 0.2f;
    float frequency = 660.0f;                          // the frequency of the tone to generate
    float frequency2 = 930.0f;
    
    float phase = 0.0;                                  // keep track of the phase of our oscillator
    float phase2 = 0.0;
    float phaseDelta = frequency  / float(sampleRate);  // phaseDelta - how much increment the phase by for each sample
    float phaseDelta2 = frequency2  / float(sampleRate);

    // ************
    // OUR DSP loop
    for (int i=0; i<durationInSamples; i++)
    {
        if(i > durationInSamples / 2.0f){
            frequency = 330.0f;
            phaseDelta = frequency / float(sampleRate);
        }
            
        // increment the phase a little
        phase += phaseDelta;
        phase2 += phaseDelta2;
        
        // make sure the phase is in the range 0-1
        if (phase > 1.0)
        {
            phase -= 1.0;
        }
        
        if (phase2 > 1.0)
        {
            phase2 -= 1.0;
        }
        
        // calculate the sine tone value from this phase --> with sin(phase * 2 * pi)
        float sample = sin( phase * 2 * 3.141593 );
        float sample2 = sin( phase2 * 2 * 3.141593 );
        
        // add this sample to our samples array (also halving the volume)
        samples[i] = (sample + sample2) * gain;
        
    }
    // END OF DSP LOOP
    // ***********
    
    // write to wav file (desktop)
    writeToWav(samples, durationInSamples, "challenge_4_2_osc.wav");
    
    
    std::cout << "Done!\n";
    return 0;
}