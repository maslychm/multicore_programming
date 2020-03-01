/*
    Peterson lock as described in "The Art of Multiprocessor Programming"

    Use on 2 threads with 1 shared resource.

    When lock() is called, if lock is available, thread acquires it
    if not available, spins until it becomes available
*/

#include <cassert>

class PetersonLock
{
private:
    bool _flag[2];
    int _victim;

public:
    PetersonLock() 
    {
        _victim = 0;
        _flag[0] = false;
        _flag[1] = false;
    }

    void lock(int threadID)
    {
        assert(threadID == 0 || threadID == 1);
        int me = threadID;
        int notme = 1 - me;
        _flag[me] = true; // I want some CPU time
        _victim = me; // But you go first
        while(_flag[notme] && _victim == me) {}; // wait (spin)
    }

    void unlock(int threadID)
    {
        int me = threadID;
        _flag[me] = false;
    }
};