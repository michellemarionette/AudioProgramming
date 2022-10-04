#include <iostream>
#include <time.h>

#include "MattsAudioHeader.h"

int main(){
    srand(time(NULL));

    float duration = 5.0f;
    float sampleRate = 48000.0;
    float durationInSamples = duration * sampleRate;
    float* samples = new float[durationInSamples];

    for(int i=0; i<durationInSamples; i++){
        float randomValue = (((rand() % 10000) * 0.0001f) - 0.5f) * 2.0f;
        samples[i] = randomValue;
    }
    
    writeToWav(samples, (int)durationInSamples, "bla.wav");

    delete[] samples;
}
