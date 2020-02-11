// Mykola Maslych for CAP4520 Multicore Programming
// Testing of a Lock-Free LIFO with SIZE

#include <atomic>
#include <thread>
#include <iostream>
#include <random>

#define SIZE_PER_THREAD 150000

// Template for node class
template<class T>
struct node
{
    T data;
    node* next;
    node(const T& data) : data(data), next(nullptr) {}
};

// Template for Stack class
// ++ and -- operators are called atomically if a variable
// is initialized as an atomic
template<class T>
class concurrentStackWithSize
{
private:
    std::atomic<node<T>*> head;
    std::atomic<int> numOps = 0;
    std::atomic<int> size = 0;

public:
    // Push operation takes a pre-allocated node and places
    // it into the stack.
    // Get current head of stack, then, at the end, if the
    // head wasn't changed, save place an object into stack
    // Otherwise get the current head and try again
    bool push(node<T> * new_node) {
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

    // Pop operation removes a node from the stack
    // Get current head of stack, and go to it's next
    // If by the time head is reassigned to the head->next
    // it's the same object, proceed. Else get the latest
    // head and try again
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
void tFunc(concurrentStackWithSize<int> &st, node<int> *nodearray) {
    for (int i = 0; i < SIZE_PER_THREAD; i++)
    {
        if (nodearray[i].data % 2 == 0)
        {
            st.push(&nodearray[i]);
        }
        else
        {
            st.pop();
        }
    }
}

int main()
{   
    // Make an instance of our stack
    concurrentStackWithSize<int> lfStack;

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

    //Pre-populate stack with our entries
    for (int i = 0; i < 50000; i++)
    {
        lfStack.push(&prePopulate[i]);
    }

    // Spawn 4 threads pushing and popping
    std::thread threads[] = {
        std::thread(tFunc, std::ref(lfStack), std::ref(list1)),
        std::thread(tFunc, std::ref(lfStack), std::ref(list2)),
        std::thread(tFunc, std::ref(lfStack), std::ref(list3)),
        std::thread(tFunc, std::ref(lfStack), std::ref(list4))
    };

    threads[0].join();
    threads[1].join();
    threads[2].join();
    threads[3].join();

    std::cout << "Final stack size: " << lfStack.getSize() << std::endl;

    return 0;
}