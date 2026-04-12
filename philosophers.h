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
}               t_philosopher;

typedef struct s_program{
    t_philosopher *philo;
    int n_of_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int meals_must_eat;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t stop_mutex;
}           t_program;

long	gettime(void);
int	ft_atoi(const char *nptr);