// Console input and output functions.
#include <iostream>
// Used for the power function.
#include <cmath>


/// Asks user to input a midi note between the values of 0 and 127.
float getMidiNote()
{
    // Switch used to check if the value inputted by the user is in range.
    bool _isNoteInRange = false;
    
    // Variable for midi note value.
    float _midiNote = 0;
    
    // Loop in which the user is asked to input a midi note value.
    // The loop will run until the _isNoteInRange variable is set to true.
    while (!_isNoteInRange)
    {
        std::cout << "Input midi value: " << std::endl;
        std::cin >> _midiNote;
        
        // Checks if the value entered is in the range of 0-127.
        // If true _isNoteInRange is switched and loop will be exited after code is run.
        if (_midiNote >= 0 && _midiNote <= 127)
        {
            _isNoteInRange = true;
        }
        
        
        // If false error message will be output and loop will run again.
        else
        {
            std::cout << "Value must be between 0 and 127." << std::endl;
        }
    }
    
    return _midiNote;
}

/// Takes as input a midi note and converts it into the frequency domain.
float midiToFrequency(float _midiNote)
{
    // Variables holding values for conversion equation.
    double _base = 2;
    double _exponent = (_midiNote - 69.0) / 12.0;
    
    // Caculates the frequancy based on the inputted midi.
    float _frequency = 440 * pow(_base, _exponent);
    
    return _frequency;
}

void printFirstEightHarmonics(float _frequency)
{
    // Loop which runs eight times and prints to the console the first eight harmonics of a given frequency.
    for(int i = 0; i < 8; i++)
    {
        std::cout << "Harmonic " << i + 1 << ": " << (int)_frequency * (i + 1) << " Hz" << std::endl;
    }
}


int main()
{
    // Gets the user to input midi note and stores it.
    float midiNote = getMidiNote();
    
    // Converts user inputted midi to its corresponding frequency.
    float frequency = midiToFrequency(midiNote);
    
    // Prints to the console the midi note value and its corresponding frequency.
    std::cout << "Note " << midiNote << " corresponds to a frequnecy of " << (int)frequency << " Hz." << std::endl;

    // Prints to the console the first eight harmonics of the user given frequency.
    printFirstEightHarmonics(frequency);
    
    return 0;
}
