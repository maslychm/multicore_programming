// Written by Mykola Maslych for CAP4520 with Damian Dechev in UCF Spring 2020
// Find primes in first 10^8 numbers using 8 threads concurrently

// REASONING: Implementing a sieve of Eratosthenes for prime numbers, where using
// the fact that each composite number is a multiple of previous primes, we throw
// away all composites.
// When using 8 threads, for each prime found, use all 8 threads to perform the work of
// throwing away composites of that one prime. Using the formula Prime^2 + Prime*ThreadNum
// to start the loop and incrementing by Prime*TotalThreadNum each time, I avoid overlaps

// Expected num: 5761455
// Expected sum: 279209790387276

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;

#define NUM_VALUE 100000000
#define THREADCOUNT 8
bool numbers[NUM_VALUE];

// Give each thread a specific offset (threadnum) so no overlap
void markSome(bool * n, int i, int threadnum)
{
    for (int j = i*i + i*threadnum; j <= NUM_VALUE; j+=i*8)
    {
        n[j] = 0;
    }
}

void main(void)
{    
    auto start = high_resolution_clock::now();
    int count = 0;
    long long int sum = 0;
    thread threads[THREADCOUNT];
    int last10[10];

    for (int i = 0; i < NUM_VALUE; i++)
    {
        numbers[i] = 1;
    }

    numbers[0] = numbers[1] = 0;

    for (int i = 2; i*i <= NUM_VALUE; i++)
    {
        // Split the work approximately evenly among threads
        if (numbers[i] == 1)
        {
            for (int t = 0; t < THREADCOUNT; t++)
            {
                threads[t] = thread(markSome, ref(numbers), i, t);
            }

            for (int t = 0; t < THREADCOUNT; t++)
            {
                threads[t].join();
            }
        }
    }

    for (int i = 0; i < NUM_VALUE; i++)
    {  
        if (numbers[i] == 1)
        {
            count++;
            sum+=i;
        }
    }

    for (int i = NUM_VALUE, c = 0; i >= 0, c < 10; i--)
    {
        if (numbers[i] == 1)
        {
            last10[c];
            c++;
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken " << (double) duration.count() / 1000000 << " seconds" << endl;
    cout << count << endl;
    cout << sum << endl;

    for(int i = 0; i < 10; i++)
    {
        cout << last10[i] << ' ';
    }

    cout << endl;
}