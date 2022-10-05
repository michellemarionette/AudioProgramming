// This is used to output messages to the console.
#include <iostream>

// This is used to seed the rand() function.
#include <time.h>

int main()
{
    // Seeds the rand() function that it will generate random values.
    srand(time(NULL));
    
    // Variables storing the upper and lower frequency boundaries.
    int frequencyUpperLimit = 880;
    int frequencyLowerLimit = 220;
    
    // Variables containing values used in the program for readibility.
    int randomNumberModulo = 10000;
    int containerSize = 50;
    int numOscillators = 3;
    
    // Pointers to arrays which will later store data.
    float* volumes = new float[containerSize];
    float* frequencies = new float[containerSize];
    int* oscillatorType = new int[containerSize];
    
    // Variables used to determine the highest and lowest values in the array,
    // they are oppositely set to the lowest and highest value so that as
    // the algorithm progresses the values will be respectively raised or lowered.
    int highestFrequency = frequencyLowerLimit;
    int lowestFrequency = frequencyUpperLimit;
    
    for (int i = 0; i < containerSize; i++)
    {
        // Generates random value between 0 and 1 and stores it.
        float randomVolume = (rand() % randomNumberModulo) * 1.0 / randomNumberModulo;
        
        // Generates random value between lower and upper frequnecy boundaries (inclusive);
        // first a random number is generated between 0 and the upper limit minus the lower limit
        // and then the lower limit is added back on to the randomly generated value.
        float randomFrequency = frequencyLowerLimit + (rand() % (frequencyUpperLimit - frequencyLowerLimit + 1));
        
        // Generated random value between 0 and 3 (inclusive).
        int randomOscillator = rand() % numOscillators + 1;
        
        // Checks to see if the random frequency is lower than what is already stored as the lowest value
        // and if true stores it.
        if (randomFrequency < lowestFrequency)
        {
            lowestFrequency = randomFrequency;
        }
        
        // Checks to see if the random frequency is higher than what is already stored as the lowest value
        // and if true stores it.
        if (randomFrequency > highestFrequency)
        {
            highestFrequency = randomFrequency;
        }
        
        // Stores the generated values inside the arrays.
        volumes[i] = randomVolume;
        frequencies[i] = randomFrequency;
        oscillatorType[i] = randomOscillator;
    }
    
    // Prints to the console the highest and lowest values.
    std::cout << "Highest frequency is: " << highestFrequency << std::endl;
    std::cout << "Lowest frequency is: " << lowestFrequency << std::endl;
    
    return 0;
}
