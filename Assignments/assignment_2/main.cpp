// Mykola Maslych for CAP4520 Multicore Programming
// Testing of a Lock-Free LIFO

#include <iostream>
#include <random>
#include "ConcurrentStack.cpp"

// To spawn on multiple threads.
// Randomly push or pop
void tFunc(concurrentStack<int> &st, int &target) {
    auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());
    
    for (int i = 0; i < target; i++) {
        bool push_or_pop = gen();
        
        if (push_or_pop)
            st.push(i);
        else
            st.pop();
    }
}

int main()
{
    std::cout << "Lock Free Stack on 4 threads" << std::endl;
    
    concurrentStack<int> lfStack;

    //Pre-populate stack with 50000 entries
    for (int i = 0; i < 50000; i++)
    {
        lfStack.push(i);
    }

    int targetNumOps = 150000;
    // Spawn 4 threads pushing and popping
    std::thread threads[] = {
        std::thread(tFunc, std::ref(lfStack), std::ref(targetNumOps)),
        std::thread(tFunc, std::ref(lfStack), std::ref(targetNumOps)),
        std::thread(tFunc, std::ref(lfStack), std::ref(targetNumOps)),
        std::thread(tFunc, std::ref(lfStack), std::ref(targetNumOps))
    };

    threads[0].join();
    threads[1].join();
    threads[2].join();
    threads[3].join();

    // Checking target and actual number of operations
    std::cout << "Target numOps: " << targetNumOps * 4 + 50000 << std::endl;
    std::cout << "Actual numOps: " << lfStack.getNumOps() << std::endl;
    return 0;
}