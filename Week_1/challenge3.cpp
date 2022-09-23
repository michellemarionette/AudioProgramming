#include <iostream>
#include <ctime>

int main(){
    srand(time(NULL));
    int randomNumber = rand() % 100;
    
    if(randomNumber > 50)
        std::cout << randomNumber << " is greater than 50.";
    else
        std::cout << randomNumber << " is less than 50.";
        
    return 0;
}
