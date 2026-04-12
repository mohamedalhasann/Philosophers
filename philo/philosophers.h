#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>   
#include <time.h>

typedef struct s_philosopher
{
    int id;
    pthread_t philo_thread;
    long last_meal;
    int meals_count;
    int left;
    int right;
}               t_philosopher;

typedef struct s_program
{
    long start_time;
    t_philosopher *philo;
    int n_of_philos;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    int meals_must_eat;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t stop_mutex;
    int stop_flag;
}           t_program;

long	gettime(void);
int	ft_atoi(const char *nptr);
#endif