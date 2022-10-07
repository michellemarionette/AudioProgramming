#include <iostream>
#include <cmath>
#include "MattsAudioHeader.h"
#include <vector>
#include <time.h>


//==================================================

class Oscillator{
    
public:
    
    Oscillator(bool _isCarrier)
    :   m_IsCarrier(_isCarrier)
    {
    }
    
    ~Oscillator()
    {
    }
    
    
private:
    
    void setFrequency(float _frequency)
    {
        m_Frequency = _frequency;
    }
    
    void setSampleRate(float _sampleRate)
    {
        m_SampleRate = _sampleRate;
    }
    
    void updatePhaseDelta()
    {
        m_PhaseDelta = m_Frequency / m_SampleRate;
    }
    
    void updatePhase()
    {
        m_Phase += m_PhaseDelta;
        
        if (m_Phase > 1.0f)
        {
            m_Phase -= 1.0f;
        }
    }


public:

    float getPhase()
    {
        return m_Phase;
    }
    
    void prepareToProcess(float _frequency, float _sampleRate)
    {
        setFrequency(_frequency);
        setSampleRate(_sampleRate);
        updatePhaseDelta();
    }
    
    float process()
    {
        if (m_IsCarrier)
        {
            updatePhase();
            return m_Phase;
        }
        
        else
        {
            float _sample = std::sin(m_Phase * 2.0f * M_PI);
            
            updatePhase();
            
            return _sample;
        }
    }

    
private:
    float m_IsCarrier;
    float m_Frequency;
    float m_SampleRate;
    float m_Phase = 0;
    float m_PhaseDelta;
};


//==================================================

float generateRandomness()
{
    return (rand() % 10000) * (1.0 / 10000) - 0.5;
}

float getModulationIndexDelta()
{
    float _modulationIndexDelta = 0;
    
    for (int i = 0; i < 64; i++)
    {
        _modulationIndexDelta += generateRandomness();
    }
    
    return _modulationIndexDelta;
}

float getFrequencyDelta()
{
    float _frequencyDelta = 0;
    
    for (int i = 0; i < 64; i++)
    {
        _frequencyDelta += generateRandomness();
    }
    
    return _frequencyDelta;
}


//==================================================

float sampleRate = 48000;
float duration = 30;
float durationInSamples = duration * sampleRate;

float frequency = 0.001;

float modulationIndex = 0.1;
float randomnessScale = 0.2;
int numOscillators = 8;

bool useSinFunction = true;
bool modulateFrequency = true;
bool modulateModulationIndex = true;

float modulationIndexModulationConstant = -0.07;
float frequencyModulationConstant = -0.002;
float frequencyOffset = -5;

float* samples = new float[durationInSamples];

std::vector<Oscillator> oscillators;


//==================================================

int main()
{
    srand(time(NULL));
    
    for (int i = 0; i < numOscillators; i++)
    {
        if (i == 0)
        {
            oscillators.push_back(Oscillator(true));
        }
        
        else
        {
            oscillators.push_back(Oscillator(false));
        }
    }
    
    for (int i = 0; i < durationInSamples; i++)
    {
        for (int i = 0; i < numOscillators; i++)
        {
            oscillators[i].prepareToProcess(frequency * (i + 1) + frequencyOffset, sampleRate);
        }
        
        Oscillator& carrier = oscillators[0];
        float combinedModulators = 0;
        
        for (int i = 1; i < numOscillators; i++)
        {
            combinedModulators += oscillators[i].process();
        }
         
        float carrierPhase = carrier.process();
        
        
        float combinedPhase;
        
        if (useSinFunction)
        {
            combinedPhase = (carrierPhase * 2 * M_PI) + (combinedModulators * std::sin(modulationIndex));
        }
        
        else
        {
            combinedPhase = (carrierPhase * 2 * M_PI) + (combinedModulators * modulationIndex);
        }
        
        samples[i] = std::sin(combinedPhase);
        
        if(modulateModulationIndex)
        {
            float modulationIndexDeltaAvg = getModulationIndexDelta() * randomnessScale;
            modulationIndex += modulationIndexDeltaAvg + modulationIndexModulationConstant;
        }
        
        if (modulateFrequency)
        {
            float frequencyDeltaAvg = getFrequencyDelta() * randomnessScale;
            frequency += frequencyDeltaAvg + frequencyModulationConstant;
        }
    }
    
    writeToWav(samples, durationInSamples, "phase.wav");
    
    return 0;
}

