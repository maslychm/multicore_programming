// Mykola Maslych for CAP4520 Multicore Programming
// Implementation of a Lock-Free LIFO

#include <atomic>
#include <thread>

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
    std::atomic<node<T>*> head = nullptr;
    std::atomic<int> numOps = 0;

public:
    bool push(const T& data) {
        node<T>* new_node = new node<T>(data);

        do 
        {
            new_node->next = head.load(std::memory_order_relaxed);
        } while (!std::atomic_compare_exchange_weak_explicit(
            &head,
            &new_node->next,
            new_node,
            std::memory_order_release,
            std::memory_order_relaxed));

        numOps++;
        return true;
    }

    node<T>* pop()
    {
        node<T>* old_head = head.load(std::memory_order_acquire);
        node<T>* new_head;

        do 
        {
            if (old_head == nullptr)
            {
                numOps++;
                return nullptr;
            }
            new_head = old_head->next;
        } while(!head.compare_exchange_weak(
            old_head,
            new_head,
            std::memory_order_acquire));

        numOps++;
        return old_head;
    }

    int getNumOps() {
        return numOps.load();
    }
};