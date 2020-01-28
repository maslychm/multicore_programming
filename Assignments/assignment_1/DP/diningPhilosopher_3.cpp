// My attempt on making a starvatio - free version of the program
// Does not work at the moment, because priorities[] isn't atomic

#include <iostream>
#include <thread>
#include <array>
#include <mutex>
#include <random>
#include <atomic>
#include <queue>

using namespace std;

atomic<bool> keepRunning = true;
atomic<int> goIndex = -1;

struct chopstick
{
    mutex mutex;
};

class PriorityManager
{
public:
    int priorities[5];
    
    PriorityManager() 
    {
        goIndex.store(0);
        
        for (int i = 0; i < 5; i++)
        {
            priorities[i] = 0;
        }
    };

    void increasePriority(int index)
    {
        priorities[index] += 1;

        // High integer in priority means a lot of use -> find lowest and prioritize
        int lowVal = INT_MAX;
        int lowInd = 0;

        for (int i = 0; i < 5; i++)
        {
            if (priorities[i] < lowVal)
            {
                lowVal = priorities[i];
                lowInd = i;
            }
        }
        goIndex.store(lowInd);
    }
};

class Philosopher
{
private:
    string const name;
    int iD;
    chopstick &left;
    chopstick &right;
    thread t;
    PriorityManager pM;

public:
    Philosopher(string n, chopstick &l, chopstick &r, PriorityManager &pmm, int id)
        : name(n), left(l), right(r), t(&Philosopher::Start, this), pM(pmm), iD(id)
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
        // If lowest priority, eat otherwise sleep more
        bool done = false;
        while (!done)
        {
            if (goIndex.load() == iD)
            {
            
                left.mutex.lock();
                right.mutex.lock();
                cout << "[" << name << "]" << " is now eating" << endl;
                pM.increasePriority(iD);

                // Unlock resources after random sleep time
                this_thread::sleep_for(chrono::milliseconds(300 * ((rand() % 5) + 1)));
                left.mutex.unlock();
                right.mutex.unlock();
                done = true;
            }
            else
            {
                // If not highest prio, wait for 300 ms and try again
                this_thread::sleep_for(chrono::milliseconds(300));
            }
        }
    }

    void think()
    {
        cout << "[" << name << "]" << " is now thinking" << endl;
    }
};

void main(void)
{
    cout << "Main Thread ID " << this_thread::get_id() << endl << endl;
    
    array<chopstick, 5> chopsticks;
    PriorityManager priorityManager;
    char c = 'x';

    Philosopher ps[] = 
    {
        Philosopher("Dijkstra", chopsticks[0], chopsticks[1], ref(priorityManager), 0),
        Philosopher("Peterson", chopsticks[1], chopsticks[2], ref(priorityManager), 1),
        Philosopher("Dostoevsky", chopsticks[2], chopsticks[3], ref(priorityManager), 2),
        Philosopher("Zizek", chopsticks[3], chopsticks[4], ref(priorityManager), 3),
        Philosopher("Jung", chopsticks[4], chopsticks[0], ref(priorityManager), 4),
    };

    while (c != 'n') {
        cin >> c;
        keepRunning = false;
    }

    cout << "The end is near" << endl;
}