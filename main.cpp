#include <vector>
#include <thread>
#include <cstdlib>
#include <iostream>

const unsigned int defaultThreads = 8;
const unsigned int defaultIterations = 1000;
unsigned int threadNumber = 0;
unsigned int maxIteration = 0;

int main(int argc, char** argv)
{
    threadNumber = defaultThreads;
    maxIteration = defaultIterations;
    switch(argc)
    {
        default:
        case 3:
            threadNumber = std::atoi(argv[2]);
        case 2:
            maxIteration = std::atoi(argv[1]);
            break;
        case 1:
        case 0:
            break;
    }
    threadNumber = 0 == threadNumber ? 1 : threadNumber;
    maxIteration = 0 == maxIteration ? 1 : maxIteration;
    std::cout << "threadNumber: " << threadNumber << std::endl;
    std::cout << "maxIteration: " << maxIteration << std::endl;
    return 0;
}
