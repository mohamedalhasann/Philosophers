#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>   
#include <bits/pthreadtypes.h>
#include <time.h>

typedef struct s_program t_program;
// died or meals must eat
typedef struct s_philosopher
{
    int i;
    pthread_t philo_thread;
    long last_meal;
    int meals_count;
    pthread_mutex_t meal_mutex;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_program *prog;
}               t_philosopher;
// meal mutex -> monitoring -> meals count  last meal died or not | philosopher -> meals count eating get time in ms
typedef struct s_program
{
    t_philosopher *philos;
    int n_of_philos;
    long start_time;
    int stop_flag;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    int meals_must_eat;
    pthread_mutex_t *forks;
    pthread_mutex_t stop_mutex;
    pthread_mutex_t print_mutex;
    pthread_t monitor;
}           t_program;

long	gettime(void);
int	ft_atoi(const char *nptr);
void args_error(char *message);
int  time_check(t_program *program);
void args_check(t_program *program,int argc, char **argv);
void	init_all_mutexes(t_program *program);
#endif