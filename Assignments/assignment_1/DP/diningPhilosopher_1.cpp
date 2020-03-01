#include <iostream>
#include <thread>
#include <array>
#include <mutex>
#include <random>
#include <atomic>

using namespace std;

struct chopstick
{
    std::mutex mutex;
};

class Philosopher
{
private:
    string const name;
    chopstick &left;
    chopstick &right;
    thread t;
    atomic_bool &kR;

public:
    Philosopher(string n, chopstick &l, chopstick &r, atomic_bool& ab)
        : name(n), left(l), right(r), kR(ab), t(&Philosopher::Start, this)
    {}

    void End()
    {
        t.join();
    }

    void Start() 
    {
        cout << name << " Thread ID: " << this_thread::get_id() << endl;
        
        while (kR.load())
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
        left.mutex.lock();
        right.mutex.lock();
        cout << "[" << name << "]" << " is now eating" << endl;

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

int main(void)
{
    atomic<bool> keepRunning;
    keepRunning.store(true);
    array<chopstick, 5> chopsticks;
    char c = 'x';

    Philosopher ps[] = 
    {
        Philosopher("Dijkstra", chopsticks[0], chopsticks[1],std::ref(keepRunning)),
        Philosopher("Peterson", chopsticks[1], chopsticks[2],std::ref(keepRunning)),
        Philosopher("Zizek", chopsticks[2], chopsticks[3],std::ref(keepRunning)),
        Philosopher("Jung", chopsticks[3], chopsticks[4],std::ref(keepRunning)),
        Philosopher("Dostoevsky", chopsticks[4], chopsticks[0],std::ref(keepRunning)),
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