// My attempt on making a starvatio - free version of the program
// Does not work at the moment, because priorities[] isn't atomic

// This DOES NOT work properly

#include <iostream>
#include <thread>
#include <array>
#include <mutex>
#include <random>
#include <atomic>
#include <queue>

using namespace std;

atomic<bool> keepRunning = true;

struct chopstick
{
    mutex mutex;
};

class Philosopher
{
private:
    string const name;
    int iD;
    chopstick &left;
    chopstick &right;
    thread t;

public:
    Philosopher(string n, chopstick &l, chopstick &r, int id)
        : name(n), left(l), right(r), t(&Philosopher::Start, this), iD(id)
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
    cout << "Main Thread ID " << this_thread::get_id() << endl << endl;
    
    deque<chopstick> chopsticks;
    vector<Philosopher> philosophers;
    char c = 'x';
    int num = 0;

    cout << "Enter the number of Philosophers" << endl;
    cin >> num;

    for (int i = 0; i < num; i++)
    {
        chopstick c;
        chopsticks.push_back(c);
    }

    for (int i = 0; i < num - 1; i++)
    {
        philosophers.push_back(Philosopher("Name" + num, chopsticks[i], chopsticks[i+1], i));
    }
    philosophers.push_back(Philosopher("Name" + num, chopsticks[num-1], chopsticks[0], num));

    while (c != 'n') {
        cin >> c;
        keepRunning = false;
    }

    cout << "The end is near" << endl;
}