#include <vector>
#include <thread>
#include <cstdlib>
#include <iostream>
#include <chrono>

const unsigned int defaultThreads = 8;
const unsigned int defaultIterations = 1000;
unsigned int threadNumber = 0;
unsigned int maxIteration = 0;
unsigned int gLock = 0;
unsigned int gCounter = 0;

struct message_t {
    unsigned int* pLock;
    unsigned int* pCounter;
};
typedef struct message_t message;
message baseLocation;

void raceCountUp(const message* m)
{
    unsigned int *lock = m->pLock;
    unsigned int *counter = m->pCounter;
    bool activeFlag = true;

    // get set
    while(*lock != 0);
    // GO

    unsigned int value;
    while(activeFlag)
    {
        value = *counter;
        if(maxIteration <= value)
        {
            activeFlag = false;
        }
        __sync_val_compare_and_swap(counter, value, value+1);
    }
    return;
}

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

    baseLocation.pLock = &gLock;
    baseLocation.pCounter = &gCounter;
    gLock = 1;
    gCounter = 0;

    std::vector<std::thread> vecThread;
    for(unsigned int i = 0;i < threadNumber;i++)
    {
        vecThread.push_back(std::thread(raceCountUp, &baseLocation));
    }

    std::cout << threadNumber << " threads started.  Push any key" << std::endl;
    getchar(); // get set
    auto start = std::chrono::system_clock::now();
    gLock = 0; // GO !

    for(auto&& it : vecThread)
    {
        it.join();
    }
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << threadNumber << '\t' << maxIteration << '\t' << duration.count() << std::endl;

    return 0;
}
