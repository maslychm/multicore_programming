// Written by Mykola Maslych for CAP4520 with Damian Dechev in UCF Spring 2020
// Find primes in first 10^8 numbers using 8 threads concurrently

// REASONING: When using 8 threads, split workload by giving each 
// a unique starting point, such as 3, 5, 7, 9, 11, 13, 15, 17.
// Next index will be an increment of 16 (because 8{threads} * 2{to skip evens})
// Prime 2 is easily known, so I count it towards count and sum manually.

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

bool isPrime(int n)
{
    for (int i = 3; i*i <= n; i += 2)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

void checkForPrimesByMod(int startVal, int maxVal, int &acm, long long int &sum)
{
    acm = 0;
    sum = 0;
    for (int i = startVal; i <= maxVal; i += 16)
    {
        if (isPrime(i))
        {
            acm++;
            sum += i;
        }
    }
}

void main(void)
{
    auto start = high_resolution_clock::now();

    int prime_count;
    long long int prime_sum = 0;

    int MAX_VALUE = 100000000;

    cout << "Count prime numbers between 1 and 10**8" << endl << endl;

    int acm1 = 0; long long sum1 = 0;
    int acm2 = 0; long long sum2 = 0;
    int acm3 = 0; long long sum3 = 0;
    int acm4 = 0; long long sum4 = 0;
    int acm5 = 0; long long sum5 = 0;
    int acm6 = 0; long long sum6 = 0;
    int acm7 = 0; long long sum7 = 0;
    int acm8 = 0; long long sum8 = 0;

    std::thread t1(checkForPrimesByMod, 3, MAX_VALUE, std::ref(acm1), std::ref(sum1));
    std::thread t2(checkForPrimesByMod, 5, MAX_VALUE, std::ref(acm2), std::ref(sum2));
    std::thread t3(checkForPrimesByMod, 7, MAX_VALUE, std::ref(acm3), std::ref(sum3));
    std::thread t4(checkForPrimesByMod, 9, MAX_VALUE, std::ref(acm4), std::ref(sum4));
    std::thread t5(checkForPrimesByMod, 11, MAX_VALUE, std::ref(acm5), std::ref(sum5));
    std::thread t6(checkForPrimesByMod, 13, MAX_VALUE, std::ref(acm6), std::ref(sum6));
    std::thread t7(checkForPrimesByMod, 15, MAX_VALUE, std::ref(acm7), std::ref(sum7));
    std::thread t8(checkForPrimesByMod, 17, MAX_VALUE, std::ref(acm8), std::ref(sum8));

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();

    prime_count = acm1 + acm2 + acm3 + acm4 + acm5 + acm6 + acm7 + acm8 + 1;
    prime_sum   = sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8 + 2;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken " << (double) duration.count() / 1000000 << " seconds" << endl;
    cout << prime_count << " primes" << endl;
    cout << prime_sum << " sum" << endl;
    // Top ten maximum primes from bottom to top
}