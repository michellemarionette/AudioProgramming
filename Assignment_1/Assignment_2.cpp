#include <iostream>
#include <cmath>


/// asks user to input a midi note between the values of 0 and 127
float getMidiNote()
{
    bool _isNoteInRange = false;
    float _midiNote = 0;
    
    while (!_isNoteInRange)
    {
        std::cout << "Input midi value: " << std::endl;
        std::cin >> _midiNote;
        
        if (_midiNote >= 0 && _midiNote <= 127)
        {
            _isNoteInRange = true;
        }
        
        else
        {
            std::cout << "Value must be between 0 and 127." << std::endl;
        }
    }
    
    return _midiNote;
}

/// takes as input a midi note and converts it into  the frequency domain
float midiToFrequency(float _midiNote)
{
    double _base = 2;
    double _exponent = (_midiNote - 69.0) / 12.0;
    float _frequency = 440 * pow(_base, _exponent);
    
    return _frequency;
}

void printFirstEightHarmonics(float _frequency)
{
    for(int i = 0; i < 8; i++)
    {
        std::cout << "Harmonic " << i + 1 << ": " << (int)_frequency * (i + 1) << " Hz" << std::endl;
    }
}


int main()
{
    float midiNote = getMidiNote();
    float frequency = midiToFrequency(midiNote);
    
    std::cout << "Note " << midiNote << " corresponds to a frequnecy of " << (int)frequency << std::endl;

    printFirstEightHarmonics(frequency);
    
    return 0;
}
