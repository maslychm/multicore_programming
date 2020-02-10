#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

typedef struct Node {
    int value;
    Node * next;
    Node(int _value) {
        value = _value;
    }
} Node;

class LockedStack
{
private:
    mutex * mutex;
    Node * head;
    int numOps;

public:
    LockedStack() : head(nullptr), numOps(0) {
        mutex = new std::mutex();
    }

    bool push(int x) {
        mutex->lock();
        Node *n = new Node(x);
        n->next = head;
        head = n;
        numOps++;
        mutex->unlock();
        return true;
    }

    int pop() {
        mutex->lock();
        if (head == nullptr) {
            mutex->unlock();
            // Must return NULL in final version
            return -1;
        }
        Node *n = head;
        head = n->next;
        numOps++;
        mutex->unlock();
        return n->value;
    }

    int getNumOps() {
        return numOps;
    }
};

void tFunc(LockedStack &L) {
    
    for (int i = 0; i < 10; i++) {
        L.push(i);
        // this_thread::sleep_for(chrono::milliseconds(10));
    }
}

int main()
{
    cout << "Hello lemme display a lock Based Stack" << endl;

    LockedStack loSt = LockedStack();

    thread threads[] = {
        thread(tFunc, ref(loSt)),
        thread(tFunc, ref(loSt)),
        thread(tFunc, ref(loSt)),
        thread(tFunc, ref(loSt))
    };

    threads[0].join();
    threads[1].join();
    threads[2].join();
    threads[3].join();

    int retVal = 0;
    while (retVal != -1) {
        retVal = loSt.pop();
        cout << retVal <<  endl;
    }

    return 0;
}