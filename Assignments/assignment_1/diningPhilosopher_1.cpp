#include <iostream>
#include <thread>
#include <array>
#include <mutex>
#include <random>
#include <atomic>

using namespace std;

struct chopstick
{
    mutex mutex;
};

atomic<bool> keepRunning = true;

void philosopherThread(chopstick &l, chopstick &r, int pID)
{
    cout << "STARTED " << pID << " thread" << endl;

    while (keepRunning)
    {
        // Sleep for random time before getting hungry
        this_thread::sleep_for(chrono::milliseconds(1000 * ((rand() % 5) + 1)));
        cout << "[" << pID << "]" << " is now hungry" << endl;

        // Lock the resource as left to right for all threads presents a deadlock
        l.mutex.lock();
        r.mutex.lock();

        cout << "[" << pID << "]" << " is now eating" << endl;

        // Unlock resources after random sleep
        this_thread::sleep_for(chrono::milliseconds(1000 * ((rand() % 5) + 1)));
        l.mutex.unlock();
        r.mutex.unlock();
        cout << "[" << pID << "]" << " is now thinking" << endl;
    }
}

void main(void)
{
    cout << "at least starts..." << endl << endl;
    array<chopstick, 5> chopsticks;
    char c = 'x';

    thread philosophers[] = 
    {
        thread(philosopherThread, ref(chopsticks[0]), ref(chopsticks[1]), 1),
        thread(philosopherThread, ref(chopsticks[1]), ref(chopsticks[2]), 2),
        thread(philosopherThread, ref(chopsticks[2]), ref(chopsticks[3]), 3),
        thread(philosopherThread, ref(chopsticks[3]), ref(chopsticks[4]), 4),
        thread(philosopherThread, ref(chopsticks[4]), ref(chopsticks[0]), 5),
    };

    while (c != 'n') {
        cin >> c;
        keepRunning = false;
    }

    cout << endl;
    for (int i = 0; i < 5; i++)
    {
        philosophers[i].join();
        cout << "ENDED thread " << i << endl;
    }

}
