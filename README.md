*This project has been created as part of the 42 curriculum by malhassa.*

# Philosophers

## Description

Philo is my take on the Dining Philosophers problem, written in C with POSIX threads.
The project simulates a group of philosophers who think, eat, and sleep while sharing forks,
and it focuses on keeping that interaction safe and synchronized.

In practice, the program handles thread creation, fork locking, starvation detection, and an
optional meal limit that ends the simulation once every philosopher has eaten enough times.


## Instructions

### Compilation

Build the project from the project directory:

```bash
cd philo
make
```

This produces the executable named `philo`

### Cleaning

```bash
make clean
make fclean
make re
```

### Execution

Run the program with the following arguments:

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [meals_must_eat]
```

Example:

```bash
./philo 5 800 200 200
./philo 5 800 200 200 7
```

Argument summary:

- `number_of_philosophers`: number of philosophers and forks
- `time_to_die`: time in milliseconds before a philosopher dies without eating
- `time_to_eat`: time in milliseconds spent eating
- `time_to_sleep`: time in milliseconds spent sleeping
- `meals_must_eat` (optional): stop when every philosopher has eaten this many times

## Resources

Classic references related to the project topic:

- Dining philosophers problem: https://en.wikipedia.org/wiki/Dining_philosophers_problem
- thread vs process : https://www.geeksforgeeks.org/operating-systems/difference-between-process-and-thread/
- introduction to threads : https://youtu.be/ldJ8WGZVXZk?si=oauAwEJdHFAFrvOE
- Race Conditions, Deadlocks, Starvation : https://medium.com/@qingedaig/race-conditions-vs-deadlocks-vs-resource-starvation-32e26b039cc2
- POSIX Threads overview: https://man7.org/linux/man-pages/man7/pthreads.7.html
- pthreads functions in c : https://www.geeksforgeeks.org/c/thread-functions-in-c-c/
- `usleep(3)` : https://man7.org/linux/man-pages/man3/usleep.3.html

AI usage:
- AI was used for testing edge cases and helping identify possible deadlocks and data races.
