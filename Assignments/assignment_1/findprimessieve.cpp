#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;

#define NUM_VALUE 100000000

void markSome(vector<char> &n, int i, int threadnum)
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
    vector<char> numbers;

    for (int i = 0; i < NUM_VALUE; i++)
    {
        numbers.push_back(1);
    }

    numbers[0] = numbers[1] = 0;

    for (int i = 2; i*i <= NUM_VALUE; i++)
    {
        if (numbers[i] == 1)
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
        if (numbers[i] == 1)
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