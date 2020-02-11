// Mykola Maslych for CAP4520 Multicore Programming
// Testing of a Lock-Free LIFO with SIZE

/* ***************** FOR TESTING ***************** */
int t1popNum = 0;
int t2popNum = 0;
int t3popNum = 0;
int t4popNum = 0;
int t1pushNum = 0;
int t2pushNum = 0;
int t3pushNum = 0;
int t4pushNum = 0;

#include <atomic>
#include <thread>
#include <iostream>
#include <random>
#include "ConcurrentStackSize.cpp"

#define SIZE_PER_THREAD 150000

using namespace std;

template<class T>
struct node
{
    T data;
    node* next;
    node(const T& data) : data(data), next(nullptr) {}
};

template<class T>
class concurrentStack
{
private:
    std::atomic<node<T>*> head;
    std::atomic<int> numOps = 0;
    std::atomic<int> size = 0;

public:
    bool push(node<T> *new_node) {

        do
        {
            new_node->next = head.load();
        } while (!std::atomic_compare_exchange_weak(
            &head,
            &new_node->next,
            new_node));

        numOps++;
        size++;
        return true;
    }

    node<T>* pop()
    {
        node<T>* old_head = head.load();
        
        node<T>* new_head = nullptr;
        do 
        {
            if (old_head == nullptr)
            {
                numOps++;
                return nullptr;
            }
            new_head = old_head->next;
        } while (!std::atomic_compare_exchange_weak(
            &head,
            &old_head,
            new_head));

        numOps++;
        size--;
        return old_head;
    }

    int getSize()
    {
        numOps++;
        return size.load();
    }

    int getNumOps() {
        return numOps.load();
    }
};


// To spawn on multiple threads.
// if even -> push it onto stack
// if odd -> pop top value
// If this works, the stack will only have even values
void tFunc(concurrentStack<int> &st, node<int> *nodearray, int &pops, int &pushs) {
    for (int i = 0; i < SIZE_PER_THREAD; i++)
    {
        if (nodearray[i].data % 2 == 0)
        {
            st.push(&nodearray[i]);
            pushs++;
        }
        else
        {
            st.pop();
            pops++;
        }
    }
}

int main()
{   
    std::cout << "Lock Free Stack on 4 threads with SIZE" << std::endl;

    // Create lists for testing each thread and preallocation
    node<int> *list1 = (node<int> *) malloc(sizeof(node<int>) * SIZE_PER_THREAD);
    node<int> *list2 = (node<int> *) malloc(sizeof(node<int>) * SIZE_PER_THREAD);
    node<int> *list3 = (node<int> *) malloc(sizeof(node<int>) * SIZE_PER_THREAD);
    node<int> *list4 = (node<int> *) malloc(sizeof(node<int>) * SIZE_PER_THREAD);
    node<int> * prePopulate = (node<int> *) malloc(sizeof(node<int>) * 50000);

    for (int i = 0; i < SIZE_PER_THREAD; i++)
    {
        list1[i].data = rand();
        list2[i].data = rand();
        list3[i].data = rand();
        list4[i].data = rand();
    }

    for (int i = 0; i < 50000; i++)
    {
        prePopulate[i].data = rand();
    }
    
    // Make an instance of our stack
    concurrentStack<int> lfStack;

    //Pre-populate stack with our entries
    for (int i = 0; i < 50000; i++)
    {
        lfStack.push(&prePopulate[i]);
    }

    int targetNumOps = SIZE_PER_THREAD;
    // Spawn 4 threads pushing and popping
    std::thread threads[] = {
        std::thread(tFunc, std::ref(lfStack), std::ref(list1), ref(t1popNum), ref(t1pushNum)),
        std::thread(tFunc, std::ref(lfStack), std::ref(list2), ref(t2popNum), ref(t2pushNum)),
        std::thread(tFunc, std::ref(lfStack), std::ref(list3), ref(t3popNum), ref(t3pushNum)),
        std::thread(tFunc, std::ref(lfStack), std::ref(list4), ref(t4popNum), ref(t4pushNum))
    };

    threads[0].join();
    threads[1].join();
    threads[2].join();
    threads[3].join();

    // std::cout << "[";
    // for (int i = 0; i < 10; i++) {
    //     std::cout << lfStack.pop()->data << " ";
    // }
    // std::cout << "]" << std::endl;

    std::cout << lfStack.getSize() << std::endl;

    // Checking target and actual number of operations
    std::cout << "Target numOps: " << targetNumOps * 4 + 50000 << std::endl;
    std::cout << "Actual numOps: " << lfStack.getNumOps() << std::endl;

    cout << "T1 pushs: " << t1pushNum << "T1 pops: " << t1popNum << endl;
    cout << "T2 pushs: " << t2pushNum << "T2 pops: " << t2popNum << endl;
    cout << "T3 pushs: " << t3pushNum << "T3 pops: " << t3popNum << endl;
    cout << "T4 pushs: " << t4pushNum << "T4 pops: " << t4popNum << endl;

    int totPops = t1popNum + t2popNum + t3popNum + t4popNum;
    int totPush = t1pushNum + t2pushNum + t3pushNum + t4pushNum;

    
    cout << "Total pushs: " << totPush << endl;
    cout << "Total pops : " << totPops << endl;

    int totMustbe = 50000 - totPops + totPush;
    std::cout << "Total size: " << lfStack.getSize() << std::endl;
    cout << "Total must be: " << totMustbe << endl;  

    return 0;
}