#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    // Seed the random number generator
    std::srand(std::time(0));
    
    int size = 30000; // Number of integers in our array
    
    // Print the array in JSON format
    std::cout << "[";
    for(int i = 0; i < size; i++) {
        std::cout << std::rand() % 100; // Random number between 0-99
        if(i < size - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;

    return 0;
}