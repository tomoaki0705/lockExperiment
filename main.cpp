#include <vector>
#include <thread>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <atomic>

const unsigned int defaultThreads = 8;
const unsigned int defaultIterations = 1000;
unsigned int threadNumber = 0;
unsigned int maxIteration = 0;
volatile unsigned int gLock = 0;
std::atomic<unsigned int> gCounter(0);

struct message_t {
    volatile unsigned int* pLock;
    unsigned int* pCounter;
};
typedef struct message_t message;
message baseLocation;

void raceCountUp(const message* m)
{
    volatile unsigned int *lock = m->pLock;
    //unsigned int *counter = m->pCounter;
    bool activeFlag = true;
    //std::atomic<unsigned int> cas_lock(gCounter);

    // get set
    while(*lock != 0);
    // GO

    unsigned int value;
    while(activeFlag)
    {
        value = gCounter.load();
        if(maxIteration <= value)
        {
            activeFlag = false;
        }
        else
        {
            std::atomic_compare_exchange_weak(&gCounter, &value, value+1);
        }
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

    baseLocation.pLock = &gLock;
    //baseLocation.pCounter = &gCounter;
    gLock = 1;
    gCounter = 0;

    std::vector<std::thread> vecThread;
    for(unsigned int i = 0;i < threadNumber;i++)
    {
        vecThread.push_back(std::thread(raceCountUp, &baseLocation));
    }

    std::cout << "created " << vecThread.size() << " thread(s)" << std::endl;
    std::cout << "get set" << std::endl;
    // get set
    auto start = std::chrono::system_clock::now();
    gLock = 0; // GO !

    for(auto&& it : vecThread)
    {
        it.join();
    }
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << threadNumber << '\t' << gCounter << '\t' << duration.count() << std::endl;

    return 0;
}
