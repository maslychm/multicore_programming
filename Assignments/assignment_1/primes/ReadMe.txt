Mykola Maslych

Implemented Sieve of Eratosthenes (https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes)
using C++ and multithreading.

To split the work among threads evenly, I direct all spawned threads to mark off the composites
of every prime in a way where there is no overlaps. To do this, I spawn 8 threads for each prime
and wait for them to join() before moving to marking the next prime

To do this effectively, I need a universal loop that will make sure no overlaps are happening
Given a prime p (2, 3, 5, 7, ...) that we'll be marking off, and a thread number t_Num (from 0 to 7)
we can build a formula that will ensure no overlaps. Essentially for each prime number, each
thread will be running the following loop, marking off composites

    for (int j = p*p + p*t_Num; j <= 100000000; j += p*THREADCOUNT)
    {
        n[j] = 0;
    }

This way, given a prime 2, each thread marks off +=16 off it's starting point,
for 3, each thread marks off += 27 off it's starting point etc

```
To run on Windows with Microsoft's Visul Studio compiler installed:
1) Open: Developer Command Prompt for VS 2019/2017 as ADMINISTRATOR
2) Run: `cl primeSieveThread.cpp`
3) Run: `primeSieveThread`
4) Open: "primes.txt"

```
To run on linux with g++ compiler:
1) Navigate to directory of the file
2) Compile: `g++ primeSieveThread.cpp -pthread`
3) Run: `./a.out`
4) Open: "primes.txt"