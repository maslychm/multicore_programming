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

class Philosopher
{
private:
    string const name;
    chopstick &left;
    chopstick &right;
    thread t;

public:
    Philosopher(string n, chopstick &l, chopstick &r)
        : name(n), left(l), right(r), t(&Philosopher::Start, this)
    {}

    void End()
    {
        t.join();
    }

    void Start() 
    {
        cout << name << " Thread ID: " << this_thread::get_id() << endl;
        
        while (keepRunning)
        {
            getHungry();
            think();
        }
    }

    void getHungry()
    {
        this_thread::sleep_for(chrono::milliseconds(200 * ((rand() % 5) + 1)));
        cout << "[" << name << "]" << " is now hungry" << endl;

        eat();
    }

    void eat()
    {
        // Lock the resource as left to right for all threads presents a deadlock
        cout << "[" << name << "]" << " is now eating" << endl;
        left.mutex.lock();
        right.mutex.lock();

        // Unlock resources after random sleep time
        this_thread::sleep_for(chrono::milliseconds(200 * ((rand() % 5) + 1)));
        left.mutex.unlock();
        right.mutex.unlock();
    }

    void think()
    {
        cout << "[" << name << "]" << " is now thinking" << endl;
    }
};

void main(void)
{
    array<chopstick, 5> chopsticks;
    char c = 'x';

    Philosopher ps[] = 
    {
        Philosopher("Dijkstra", chopsticks[0], chopsticks[1]),
        Philosopher("Peterson", chopsticks[1], chopsticks[2]),
        Philosopher("Zizek", chopsticks[2], chopsticks[3]),
        Philosopher("Jung", chopsticks[3], chopsticks[4]),
        Philosopher("Dostoevsky", chopsticks[4], chopsticks[0]),
    };

    while (c != 'n') {
        cin >> c;
        keepRunning = false;
    }

    for (int i = 0; i < 5; i++)
    {
        ps[i].End();
    }

    cout << "Ended threads Correctly " << endl;
}