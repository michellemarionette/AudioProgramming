#include <iostream>
#include <cmath>
#include "MattsAudioHeader.h"
#include <vector>


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

float sampleRate = 48000;
float duration = 15;
float durationInSamples = duration * sampleRate;

float frequency = 80;

float modulationIndex = 1;

float* samples = new float[durationInSamples];

std::vector<Oscillator> oscillators;
int numOscillators = 10;

//==================================================

int main()
{
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
            oscillators[i].prepareToProcess(frequency * (i + 1), sampleRate);
        }
        
        Oscillator& carrier = oscillators[0];
        float combinedModulators = 0;
        
        for (int i = 1; i < numOscillators; i++)
        {
            combinedModulators += oscillators[i].process();
        }
        
        float carrierPhase = carrier.process();
        
        float combinedPhase = (carrierPhase * 2 * M_PI) + (combinedModulators) * modulationIndex * 10;
        
        samples[i] = std::sin(combinedPhase);
        
        modulationIndex += 0.0001;
    }
    
    writeToWav(samples, durationInSamples, "phase.wav");
    
    return 0;
}
