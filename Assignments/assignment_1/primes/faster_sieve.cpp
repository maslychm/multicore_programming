// Mykola Maslych. improved 8-threaded prime sieve. Feb 2023.
// revisiting because a friend is taking this course and mentioned this assignment

// Each thread will have a specific offset (threadnum) so no overlap
// in work occurs. Using the formula Prime^2 + Prime*ThreadNum to start,
// when a thread finds a prime, it will mark all composites of that prime
// **THAT IT IS RESPONSIBLE FOR**. This is done by incrementing by Prime*TotalThreadNum.
// So a thread only marks multiples of (prime*threadnum).

// Expected num: 5761455
// Expected sum: 279209790387276

/*
g++ faster_sieve.cpp -lpthread -std=c++11
./a.out
cat primes.txt
*/

#include <fstream>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

#define NUM_VALUE 100000000
#define THREADCOUNT 8
bool numbers[NUM_VALUE];

// Give each thread a specific offset (threadnum) so no overlap
void markSome(bool (&n)[NUM_VALUE], int threadnum)
{
    for (int i = 2; i * i < NUM_VALUE; i++)
    {
        if (n[i] == 1)
        {
            for (int j = i * i + i * threadnum; j <= NUM_VALUE; j += i * THREADCOUNT)
            {
                n[j] = 0;
            }
        }
    }
}

int main(void)
{
    int count = 0;
    int last10[10];
    long long int sum = 0;
    thread threads[THREADCOUNT + 1];
    ofstream myfile;

    for (int i = 0; i < NUM_VALUE; i++)
    {
        numbers[i] = 1;
    }
    numbers[0] = numbers[1] = 0;

    // Start counting time
    auto start = high_resolution_clock::now();

    for (int t = 0; t < THREADCOUNT; t++)
    {
        threads[t] = thread(markSome, ref(numbers), t);
    }

    for (int t = 0; t < THREADCOUNT; t++)
    {
        threads[t].join();
    }

    // Finish counting time
    auto stop = high_resolution_clock::now();

    // Count and sum primes in range
    for (int i = 0; i < NUM_VALUE; i++)
    {
        if (numbers[i] == 1)
        {
            count++;
            sum += i;
        }
    }

    // Save last 10 primes
    for (int i = NUM_VALUE - 1, c = 9; i >= 0 && c >= 0; i--)
    {
        if (numbers[i] == 1)
        {
            last10[c] = i;
            c--;
        }
    }

    auto duration = duration_cast<microseconds>(stop - start);

    myfile.open("primes.txt");

    myfile << "Time taken " << (double)duration.count() / 1000000 << " seconds" << endl;
    myfile << "Primes found: " << count << endl;
    myfile << "Sum of primes: " << sum << endl;

    for (int i = 0; i < 10; i++)
    {
        myfile << last10[i] << ' ';
    }

    myfile << endl;
}