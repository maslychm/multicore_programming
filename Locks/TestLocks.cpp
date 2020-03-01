#include <iostream>
#include <vector>
#include <thread>
#include "PetersonLock.cpp"

/*
    Compile: g++ TestLocks.cpp -pthread
    run:     ./a.out
*/

void testPeterson(PetersonLock &pl, int id)
{
    // Comment out the lock() line to have 0 and 1 in the stdout overlap
    pl.lock(id);
    std::printf("\nThread %d locked resource\n", id);

    // Keep printing id (if overlaps happen, lock doesn't work)
    for(int i = 0; i < 10000; i++)
    {        
        if (i % 100 == 0)
            std::printf("%d",id); 
    } 
    std::printf("\n");

    // Unlock
    pl.unlock(id);
    std::printf("Thread %d unlocked resource\n", id);
}

int main()
{
    PetersonLock pLock;

    std::thread t1(testPeterson, std::ref(pLock), 0);
    std::thread t2(testPeterson, std::ref(pLock), 1);

    t1.join();
    t2.join();

    std::cout << "Threads finished" << std::endl;
}