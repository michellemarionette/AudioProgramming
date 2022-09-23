
// import the iostream library for cout and cin
#include <iostream>

// import the time library for our random number seeding
#include <ctime>

int main(int argc, const char * argv[]) {

    // this line "reseeds" our random number functions so that they are different every time
    srand (time(NULL));
    
    int valueA = rand() % 100; // create a random integer between 0 and 99
    int valueB = rand() % 100; // create a random integer between 0 and 99
    
    // ask the user to input a number
    std::cout << "What is " << valueA << " plus " << valueB << "?\n";
    
    // container for answer
    int answer;

    // get answer to screen
    std::cin >> answer;

    //check if answer == sum of two random values

    if(answer == valueA + valueB)
        std::cout << "Correct" << std::endl;
    else
        std::cout << "Wrong" << std::endl;
    
    return 0;
}


