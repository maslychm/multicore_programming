#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

bool isPrime(int n)
{
    // if (n == 1) return false;
    // if (n == 2) return true;
    // if (n % 2 == 0) return false;   
    for (int i = 3; i*i <= n; i += 2)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

void main(void) 
{
    int prime_count = 0;

    cout << "Count prime numbers between 1 and 10**8" << endl << endl;

    auto start = high_resolution_clock::now();

    // for (int i = 1; i < 100000000; i++)
    // {
    //     if (isPrime(i))
    //         prime_count++;
    // }

    prime_count++; // Skip 2 and it's multiples
    for (int i = 3; i < 100000000; i+=2)
    {
        if (isPrime(i))
            prime_count++;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    
    cout << prime_count << " primes" << endl;

    cout << "Time taken " << (double) duration.count() / 1000000 << " seconds" << endl;
}