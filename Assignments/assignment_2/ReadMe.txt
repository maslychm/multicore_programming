ConcurrentStack.cpp Readme

HEAD of stack and NUMOPS need to be completed in one cycles,
so are declared as atomic variables. They are shared by multiple
threads so my choice is making them atomic to be modified in one
cycle. 

push() operation:
1) create NEW NODE
2) save OLD HEAD
3) ATOMICALLY check if head was changed
    if NO, replace with NEW NODE
    if YES, go back to step 2

pop() operation:
1) save OLD HEAD
2) check if OLD HEAD = NULL
    if YES return NULL
3) take NEW HEAD from OLD_HEAD->NEXT
4) ATOMICALLY check if head was changed
    if NO, replace with NEW NODE
    if YES, go back to step

atomics:
    ptr * head
    int numOps
    int size

    operators ++ and -- act as atomics if a variable is defined
    as an atomic variable

Both Push and Pop operations will only succeed with adding/removing
an element IF in the time they are doing it, there was no change of
the head. This guarantees correcness.

```````````````````````````````````````````````````````````````````
Compile on Windows using Microsoft Visual Studio's compiler

: cl ConcurrentStack.cpp
: ConcurrentStack