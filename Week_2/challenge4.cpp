#include <iostream>


int main(){
    float duration;
    int sampleRate;
    int durationInSamples;
    double* samples = new double[durationInSamples];

    for(int i=0; i<durationInSamples; i++){
        samples[i] = 0;
    }

    delete[] samples;
}
