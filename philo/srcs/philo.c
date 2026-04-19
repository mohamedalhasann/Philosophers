/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:27:25 by malhassa          #+#    #+#             */
/*   Updated: 2026/04/19 18:00:28 by malhassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>


static void args_error(char *message)
{
    printf("%s",message);
    exit(1);
}
static int  time_check(t_program *program)
{
    if (program->time_to_die <= 0)
        return (0);
    if (program->time_to_eat <= 0)
        return (0);
    if (program->time_to_sleep <= 0)
        return (0);
    return (1);
}
static void args_check(t_program *program,int argc, char **argv)
{    
    program->n_of_philos = ft_atoi(argv[1]);
    if (program->n_of_philos < 1)
        args_error("invalid number of philosophres\n");
    program->time_to_die = ft_atoi(argv[2]);
    program->time_to_eat = ft_atoi(argv[3]);
    program->time_to_sleep = ft_atoi(argv[4]);
    if (!time_check(program))
        args_error("invalid time\n");
    if (argc == 6)
    {
        program->meals_must_eat = ft_atoi(argv[5]);
        if (program->meals_must_eat <= 0)
            args_error("invalid number of meals\n");   
    }
    else
        program->meals_must_eat = -1;
}
void	lock_routine(char *message, pthread_mutex_t *mutex,int print)
{
	pthread_mutex_lock(mutex);
	printf("%s %d",message,print);
	pthread_mutex_unlock(mutex);
}
static void	*philo_routine(void *arg)
{
	t_philosopher *philo;
	
	philo = (t_philosopher *)arg;
	lock_routine("THREAD ", &philo->prog->print_mutex,philo->i);
	if (philo -> i % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
	}
	lock_routine("is eating", &philo->prog->print_mutex, philo->i);
	usleep(philo->prog->time_to_eat);
	philo->last_meal = gettime();
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (NULL);
}
void	init_all_mutexes(t_program *program)
{
	// protection later
	int	i;

	i = 0;
	while (i < program->n_of_philos)
	{
		pthread_mutex_init(&program->forks[i],NULL);
		pthread_mutex_init(&program->philos[i].meal_mutex,NULL);
		i++;
	}
	pthread_mutex_init(&program->print_mutex,NULL);
	pthread_mutex_init(&program->stop_mutex,NULL);
	i = 0;
	while (i < program->n_of_philos)
	{
		program->philos[i].i = i+1;
		program->philos[i].left_fork = &program->forks[i];
		program->philos[i].right_fork = &program->forks[(i+1) % program->n_of_philos];
		program->philos[i].prog = program;
		program->philos[i].meals_count = 0;
		program->philos[i].last_meal = program->start_time;
		i++;
	}
}
void	create_join_threads(t_program *program)
{
	int	i;

	i = 0;
	init_all_mutexes(program);
	while (i < program->n_of_philos)
	{
		if (pthread_create(&program->philos[i].philo_thread, NULL,
				philo_routine, &program->philos[i]) != 0)
			args_error("pthread_create failed \n");
		i++;
	}
	i = 0;
	while (i < program->n_of_philos)
	{
		if (pthread_join(program->philos[i].philo_thread,  NULL) != 0)
			args_error(" pthread_join failed\n");
		i++;
	}
}

int	allocate_data(t_program *program)
{
	program->philos = malloc(program->n_of_philos * sizeof(t_philosopher));
	if (!program->philos)
		return (0);
	program->forks = malloc(program->n_of_philos * sizeof(pthread_mutex_t));
	if (!program->forks)
	{
		free(program->philos);
		program->philos = NULL;
		return (0);
	}
	program->start_time = gettime();
	program->stop_flag = 0;
	create_join_threads(program);
	return (1);
}

int	main(int argc, char **argv)
{
	t_program	program;

	if (argc != 5 && argc != 6)
	{
		printf("invalid number of arguments\n");
		exit(1);
	}
	args_check(&program, argc, argv);
	if (!allocate_data(&program))
		args_error("allocation failed\n");
	return (0);
}
