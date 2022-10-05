#include <iostream>
#include <cmath>
#include "MattsAudioHeader.h"


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


float sampleRate = 48000;
float duration = 15;
float durationInSamples = duration * sampleRate;

float frequency = 10;
float frequency2 = 10;
float frequency3 = 13;

float modulationIndex = 1;

float* samples = new float[durationInSamples];


int main()
{
    Oscillator osc1(true);
    Oscillator osc2(false);
    Oscillator osc3(false);
    
    for (int i = 0; i < durationInSamples; i++)
    {
        osc1.prepareToProcess(frequency, sampleRate);
        osc2.prepareToProcess(frequency2, sampleRate);
        osc3.prepareToProcess(frequency3, sampleRate);
        
        float carrierPhase = osc1.process();
        float modulator = osc2.process();
        float modulator2 = osc3.process();
        
        samples[i] = std::sin((modulationIndex * (modulator + modulator2)) + (2 * M_PI * carrierPhase)) * 0.5;
        
        modulationIndex += 0.0001;
        frequency -= 0.01;
        frequency2 += 0.05;
        frequency3 -= 0.03;
        
    }
    
    writeToWav(samples, durationInSamples, "phase.wav");
    
    return 0;
}
