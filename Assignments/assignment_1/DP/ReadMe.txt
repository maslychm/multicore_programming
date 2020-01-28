Mykola Maslych

Dining Philosopher 1 -> Philosophers classes that can think() getHungry() and eat()
for randomized amounts of time. This version is presents a Deadlock if unlucky, or
if set a wait() time to ~10ms after locking the left chopstick

Dining Philosopher 2 -> Built upon DP1, this removes the deadlock problem, but
starvation is still present

Dining Philosopher 3 -> Attempter to create a custom prioritizing mechanizm for 
the threads, but couldn't overcome some problems with it

```
To run, for each file:
Using Microsoft Visual Studio Compiler
compile with: `cl diningPhilosopher_*.cpp`
run with    : `diningPhilosopher_*`
