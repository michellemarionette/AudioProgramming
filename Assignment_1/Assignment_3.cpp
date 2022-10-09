#include <iostream>
#include <cmath>                // Used for various math functions.
#include "MattsAudioHeader.h"   // Used to render audio file.
#include <vector>               // Used to create a vector array to store objects.
#include <time.h>               // Used to seed rand() function.


//==================================================

/// Base oscillator class.
class Oscillator{
    
public:
    
    /// Oscillator constructor.
    /// @param _isCarrier  sets the oscillator to being a carrier or modulator.
    Oscillator(bool _isCarrier)
    :   m_IsCarrier(_isCarrier)
    {
    }
    
    
    /// Oscillator distructor.
    ~Oscillator()
    {
    }
    
    
private:
    
    /// Sets the oscillator frequency.
    ///  @param _frequency new frequency to switch to.
    void setFrequency(float _frequency)
    {
        m_Frequency = _frequency;
    }
    
    /// Sets the oscillatos sample rate.
    /// @param _sampleRate new sample rate to swtich to.
    void setSampleRate(float _sampleRate)
    {
        m_SampleRate = _sampleRate;
    }
    
    /// Updates phase delta value.
    void updatePhaseDelta()
    {
        m_PhaseDelta = m_Frequency / m_SampleRate;
    }
    
    /// Updates phase value.
    void updatePhase()
    {
        // Adds to current phase value the phase delta.
        m_Phase += m_PhaseDelta;
        
        // If phase is greater than 1 then this will reduce the phase by 1
        // thus restarting the wave cycle.
        if (m_Phase > 1.0f)
        {
            m_Phase -= 1.0f;
        }
    }


public:

    /// Returns a float value representing the phase of the oscillator.
    float getPhase()
    {
        return m_Phase;
    }
    
    /// Updates oscillator values and prepares the internal state for next round of DSP processing.
    /// @param _frequency new frequency value to update to.
    /// @param _sampleRate new sample rate value to update to.
    void prepareToProcess(float _frequency, float _sampleRate)
    {
        setFrequency(_frequency);
        setSampleRate(_sampleRate);
        updatePhaseDelta();
    }
    
    /// Updates internal state of oscillator and returns float value representing phase if oscillator is carrier
    /// or returns sample value if oscillator is modulator.
    float process()
    {
        // If oscillator is carrier updates phase and returns it.
        if (m_IsCarrier)
        {
            updatePhase();
            return m_Phase;
        }
        
        // If oscillator is modulator updates phase and returns new sample.
        else
        {
            // Calculates the new sample value.
            float _sample = std::sin(m_Phase * 2.0f * M_PI);
            updatePhase();
            return _sample;
        }
    }

    
private:
    bool m_IsCarrier;               // Boolean value that sets the oscillator to carrier mode or modulator mode.
    float m_Frequency;              // Frequency of oscillator.
    float m_SampleRate;             // Sample rate of oscillator.
    float m_Phase = 0;              // Phase of oscillator.
    float m_PhaseDelta;             // Phase delta of oscillator.
};


//==================================================

float sampleRate = 48000;                               // Sample rate of the output audio.
float duration = 15;                                    // Duration in seconds of the output audio.
float durationInSamples = duration * sampleRate;        // Product of the duration of the audio and the sample rate.
float frequency = 50;                                   // Frequnecy of the carrier oscillator.

float modulationIndex = 0.7;                            // Scalar value used modify the modulation oscillator.
float randomnessScale = 0.0008;                         // Scalar value to scale signal randomness.
int numOscillators = 4;                                 // Number of oscillators that will be generated (if less than 2 it will break).

bool useSinFunction = false;                            // If true modulation index will be passed into a std::sin().
bool modulateModulationIndex = true;                    // If true modulation index will be modulated.

float modulationIndexModulationConstant = 0.000001;     // Constant which defines by how much modulation index will be modulated.
float frequencyOffset = 2;                              // Constant which defines by how much to increment eacb modulator frequency.

float* samples = new float[durationInSamples];          // Pointer to sample buffer.

float gain = 0.5;                                       // Overall gain scaler for the processed signal.
float attackTime = 0.009;                               // Attack time used for AD processing.
float decayTime = 0.005;                                // Decay time used for the AD processing.
float fadeInDurationInSamples = sampleRate;             // Duration in samples of the overall audio fade-in.
float fadeOutDurationInSamples = sampleRate;            // Duration in samples of the overall audio fade-out.

std::vector<Oscillator> oscillators;                    // Empty vector into which oscillators will be placed.


//==================================================

/// Returns a random float value between 0 and 1.
float generateRandomness()
{
    return (rand() % 10000) * (1.0 / 10000);
}

/// Takes as input a float value and returns the same value with its sign flipped 50% of the time.
/// @param _value value to which a randomized sign flip is to be applied.
float randomizeSign(float _value)
{
    // Randomly generates 0 and 1.
    int _randomNum = rand() % 2;
    
    
    //If random number is 0 return value as is.
    if (_randomNum == 0)
    {
        return _value;
    }
    
    // If random number is 1 return value with flipped sign.
    else
    {
        return _value * -1;
    }
}

/// Returns a float value representing the delta value for the modulation index.
float getModulationIndexDelta()
{
    float _modulationIndexDelta = 0;                            // This will contain the delta value that will be returned.
    float _randomnessAverager = 64;                             // Constant used to average the randomly generated values.
    
    // This loop will add on to the modulation index delta randomly generated value.
    for (int i = 0; i < _randomnessAverager; i++)
    {
        _modulationIndexDelta += generateRandomness();
    }
    
    // Here we take the average of the randomly generated values.
    _modulationIndexDelta = _modulationIndexDelta / _randomnessAverager;
    
    // Flips the sign of the value 50% of the time.
    _modulationIndexDelta = randomizeSign(_modulationIndexDelta);
    
    return _modulationIndexDelta;
}

/// Instantiates all oscillators and stores them inside a vector..
void instantiateOscillators()
{
    for (int i = 0; i < numOscillators; i++)
    {
        // If true Oscillator will be carrier.
        if (i == 0)
        {
            oscillators.push_back(Oscillator(true));
        }
        
        // If above statements is false oscillator will be modulator.
        else
        {
            oscillators.push_back(Oscillator(false));
        }
    }
}

/// Prepares internal state of all oscillators so that processing can be done.
void prepareOscillators()
{
    // Loop that instantiates oscillators and stores them in Oscillators vector.
    for (int i = 0; i < numOscillators; i++)
    {
        // If this is the first iteration of the loop then process oscillator withou frequency offset.
        if (i == 0)
        {
            oscillators[i].prepareToProcess(frequency, sampleRate);
        }
        
        // If this isn't first loop iteration then multiply the frequency using the iteration number + 1 as a scaler
        // and then add the frequency offset.
        else
        {
            oscillators[i].prepareToProcess(frequency * (i + 1) + frequencyOffset, sampleRate);
        }
    }
}

/// Processes all the modulators and returns a float value representing the sum of all the modulators.
float processModulators()
{
    float _combinedModulators = 0;
    
    for (int i = 1; i < numOscillators; i++)
    {
        _combinedModulators += oscillators[i].process();
    }
    
    return _combinedModulators;
}

/// Does DSP processing on the carrier and modulator.
/// Returns the procesed oscillator as a float value.
/// @param _carrierPhase the phase of the carrier oscillator.
/// @param _combinedModulators the combined modulators sample value.
float runDSP(float _carrierPhase, float _combinedModulators)
{
    // Variable that will hold the sample value after being phase modulated.
    float _combinedPhase;
    
    // Runs if modulation index shoud be constrained by sin() function.
    if (useSinFunction)
    {
        // Puts the carrier phase and the modulator wave throught phase modulation algorhithm.
        _combinedPhase = (_carrierPhase * 2 * M_PI) + (_combinedModulators * std::sin(modulationIndex));
    }
    
    else
    {
        // Puts the carrier phase and the modulator wave throught phase modulation algorhithm.
        _combinedPhase = (_carrierPhase * 2 * M_PI) + (_combinedModulators * modulationIndex);
    }
    
    // Returns phase modulated sample.
    return std::sin(_combinedPhase);
}

/// Applies various attack and decay stages to the audio buffer.
void applyAD()
{
    float _attackStage = 0;                                         // Variable used to monitor attack stage.
    float _decayStage = 1;                                          // Variable used to monitor decay stage.
    
    float _attackStageDelta = 1.0 / (attackTime * sampleRate);      // Delta value with which to increment attack stage.
    float _decayStageDelta = 1.0 / (decayTime * sampleRate);        // Delta value with which to decrement decay stage.
    
    bool _ADStage = true;                                           // Variable used to monitor if audio should attack or decay (true = attack, false = decay).
    
    // This loops over the entire audio buffer and applies many attacks
    // and decays based on attack and decay times.
    for (int i = 0; i <  durationInSamples; i++)
    {
        // If the attack stage goes over 1 then the _ADStage will flip
        // and the dacay stage will begin.
        if (_attackStage > 1.0)
        {
            // Flips to decay stage.
            _ADStage = false;
            
            // Resets attack stage to 0.
            _attackStage = 0.0;
        }
    
        // If the decay stage over under 0 the _ADStage will flip
        // and the attack stage will begin.
        else if (_decayStage < 0.0)
        {
            // Flips to attack stage.
            _ADStage = true;
            
            // Resets decay stage.
            _decayStage = 1.0;
            
            // Lengthens the decay time each time the dacay stage ends using a basic squaring function.
            decayTime += decayTime * decayTime;
            
            // Re-evaluates the decay stage delta based on the new decay time.
            _decayStageDelta = 1.0 / (decayTime * sampleRate);
        }
        
        // If loop is in attack stage then add attack to audio buffer.
        if (_ADStage)
        {
            // Applies some easing to the attack with a squaring function.
            samples[i] *= _attackStage * _attackStage;
            
            // Increases the attack stage by the delta value.
            _attackStage += _attackStageDelta;
        }
        
        // If loop is in decay stage then add decay to audio buffer.
        else
        {
            // Applies some easing to the decay with a tanh function.
            samples[i] *= std::tanh(_decayStage);
            
            // Decreases the decay stage by the delta value.
            _decayStage -= _decayStageDelta;
        }
    }
}

/// Applies fades to start and end of the audio buffer.
void applyFades()
{
    float _fadeInDelta = 1.0 / fadeInDurationInSamples;         // Variable containing delta value for fade-in amplitude multiplier.
    float _fadeOutDelta = 1.0 / fadeOutDurationInSamples;       // Variable containing delta value for fade-out amplitude multiplier.
    
    float _fadeInScale = 0;                                     // Value used to scale audio buffer sample fade-in amplitude.
    float _fadeOutScale = 1;                                    // Value used to scale audio buffer sample fade-out amplitude.
    
    // Loop that goes from start of audio buffer to the length of the fade-in in samples.
    for (int i = 0; i < fadeInDurationInSamples; i++)
    {
        // Scales sample value.
        samples[i] *= _fadeInScale;
        
        // Adds delta to fade-in scaler.
        _fadeInScale += _fadeInDelta;
    }
    
    // Loop that goes from the duration in samples - fade-out duration to the end of the audio buffer.
    for (int i = durationInSamples - fadeOutDurationInSamples; i < durationInSamples; i++)
    {
        // Scales sample value.
        samples[i] *= _fadeOutScale;
        
        // Adds delta to face out scaler.
        _fadeOutScale -= _fadeOutDelta;
    }
}

//==================================================

int main()
{
    // Seeds random function.
    srand((int)time(NULL));
    
    // Instatiates all oscillators.
    instantiateOscillators();
    
    // Main DSP loop.
    for (int i = 0; i < durationInSamples; i++)
    {
        // Prepares the oscillators for next DSP iteration.
        prepareOscillators();

        // Reference variable for readibility.
        Oscillator& carrier = oscillators[0];
        
        // Variable storing the combined modulators.
        float combinedModulators = processModulators();
        
        // Variables containing carrier phase.
        float carrierPhase = carrier.process();
        
        // Stores the DSP processed signal into the audio buffer.
        samples[i] = runDSP(carrierPhase, combinedModulators) * gain;
        
        // Update modulation index.
        if(modulateModulationIndex)
        {
            // Stores the new scaled modulation index delta value.
            float modulationIndexDeltaAvg = getModulationIndexDelta() * randomnessScale;
            
            // Updates modulation index based on delta value and the addition of a constant.
            modulationIndex += modulationIndexDeltaAvg + modulationIndexModulationConstant;
        }
    }
    
    applyAD();
    applyFades();
    
    // Creates audio file with audio from the audio buffer.
    writeToWav(samples, durationInSamples, "PhaseModulation.wav");
    
    return 0;
}

