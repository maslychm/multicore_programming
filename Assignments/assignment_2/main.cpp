#include <iostream>
#include <chrono>
#include "ConcurrentStack.cpp"

// Function to be spawned on multiple threads
void tFunc(concurrentStack<int> &st, int &target) {
    
    for (int i = 0; i < target; i++) {
        st.push(i);
    }
}

int main()
{
    std::cout << "Lock Free Stack on 4 threads" << std::endl;
    
    int targetNumOps = 150000;
    
    concurrentStack<int> lfStack;

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

    for (int i = 0; i < 10000; i++) {
        lfStack.pop();
    }

    std::cout << "[";
    for (int i = 0; i < 10; i++) {
        std::cout << lfStack.pop()->data << " ";
    }
    std::cout << "]" << std::endl;

    // Checking target and actual number of operations
    std::cout << "Target numOps: " << targetNumOps * 4 << std::endl;
    std::cout << "Actual numOps: " << lfStack.getNumOps() << std::endl;
    return 0;
}