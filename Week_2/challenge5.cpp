#include <iostream>
#include <time.h>
#include <cmath>
#include "MattsAudioHeader.h"

int main(){
    srand(time(NULL));

    float duration = 10.0f;
    float sampleRate = 48000.0f;
    float durationInSamples = duration * sampleRate;
    
    float gainDelta = 1.0f / durationInSamples;
    float frequency = 1.0f;
    float phase = 0.0f;
    float phaseDelta = 1.0f / (sampleRate / frequency);
    
    float* samples = new float[durationInSamples];

    for(int i=0; i<durationInSamples; i++){
        if(phase>1.0f)
            phase -= 1.0f;

        float randomValue = (((rand() % 10000) * 0.0001f) - 0.5f) * 2.0f;

        samples[i] = randomValue * std::sin(phase * 2.0f * M_PI);
        
        phase += phaseDelta;
    }
    
    writeToWav(samples, (int)durationInSamples, "bla.wav");

    delete[] samples;
}
