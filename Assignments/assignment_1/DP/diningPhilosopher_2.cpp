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
        this_thread::sleep_for(chrono::milliseconds(300 * ((rand() % 5) + 1)));
        cout << "[" << name << "]" << " is now hungry" << endl;

        eat();
    }

    void eat()
    {
        left.mutex.lock();
        right.mutex.lock();
        cout << "[" << name << "]" << " is now eating" << endl;

        // Unlock resources after random sleep time
        this_thread::sleep_for(chrono::milliseconds(300 * ((rand() % 5) + 1)));
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

    // For the last philosopher, switch the order of picking un chopsticks.
    // This ensures no deadlock in the program, as now P1 and P5 are trying
    // to pick up the same chopstick first
    Philosopher ps[] = 
    {
        Philosopher("Dijkstra", chopsticks[0], chopsticks[1]),
        Philosopher("Peterson", chopsticks[1], chopsticks[2]),
        Philosopher("Zizek", chopsticks[2], chopsticks[3]),
        Philosopher("Jung", chopsticks[3], chopsticks[4]),
        Philosopher("Dostoevsky", chopsticks[0], chopsticks[4]),
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