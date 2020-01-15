#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;

#define NUM_VALUE 10000000

// Even though none of the entries in vector overlap, when ram call
// brings a chunk of memory to cache, previously working cores have a
// changed state that gets invalidated, resulting in skipping over
// some of the primes

// Need a way to make sure no threads access same resource at the same time,
// so maybe in some way I can use atomic counter here. Maybe somehow have
// each thread use GetAndIncrement() to get a newly available prime for next
// composite numbers 

void markSome(vector<bool> &n, int i, int threadnum)
{
    for (int j = i*i + i*threadnum; j <= NUM_VALUE; j+=i*8)
    {
        n[j] = false;
    }
}

void main(void)
{    
    auto start = high_resolution_clock::now();
    int count = 0;
    vector<bool> numbers;

    for (int i = 0; i < NUM_VALUE; i++)
    {
        numbers.push_back(true);
    }

    numbers[0] = numbers[1] = false;

    for (int i = 2; i*i <= NUM_VALUE; i++)
    {
        if (numbers[i] == true)
        {
            thread t1(markSome, ref(numbers), i, 0);
            thread t2(markSome, ref(numbers), i, 1);
            thread t3(markSome, ref(numbers), i, 2);
            thread t4(markSome, ref(numbers), i, 3);
            thread t5(markSome, ref(numbers), i, 4);
            thread t6(markSome, ref(numbers), i, 5);
            thread t7(markSome, ref(numbers), i, 6);
            thread t8(markSome, ref(numbers), i, 7);

            t1.join();
            t2.join();
            t3.join();
            t4.join();
            t5.join();
            t6.join();
            t7.join();
            t8.join();
        }
    }

    for (int i = 0; i < NUM_VALUE; i++)
    {  
        if (numbers[i] == true)
        {
            count++;
            // cout << " " << i;
        }
    }

    cout << endl << count << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken " << (double) duration.count() / 1000000 << " seconds" << endl;
}