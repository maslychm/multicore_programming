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
            new_node->next = head.load();
        } while (!std::atomic_compare_exchange_weak(
            &head,
            &new_node->next,
            new_node));

        numOps++;
        return true;
    }

    node<T>* pop()
    {
        node<T>* old_head = head.load();
        node<T>* new_head;

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
        return old_head;
    }

    int getNumOps() {
        return numOps.load();
    }
};