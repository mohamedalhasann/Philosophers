/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:27:25 by malhassa          #+#    #+#             */
/*   Updated: 2026/04/30 15:47:33 by malhassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <string.h>

void	lock_routine(long time, int i, char *philo_action,
		pthread_mutex_t *mutex,t_program *program)
{
	pthread_mutex_lock(&program->stop_mutex);
	if (program->stop_flag != 1 || strcmp(philo_action,"died") == 0)
	{
		pthread_mutex_lock(mutex);
		printf("%ld %d %s\n", time, i, philo_action);
		pthread_mutex_unlock(mutex);
	}
	pthread_mutex_unlock(&program->stop_mutex);
}

void	ft_sleep(long time, t_program *program);

static void	eating_process(t_philosopher *philo)
{
	if (philo -> i % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		lock_routine(gettime() - philo->prog->start_time, philo->i,
			"has taken a fork", &philo->prog->print_mutex,philo->prog);
		pthread_mutex_lock(philo->right_fork);
		lock_routine(gettime() - philo->prog->start_time, philo->i,
			"has taken a fork", &philo->prog->print_mutex,philo->prog);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		lock_routine(gettime() - philo->prog->start_time, philo->i,
			"has taken a fork", &philo->prog->print_mutex,philo->prog);
		pthread_mutex_lock(philo->left_fork);
		lock_routine(gettime() - philo->prog->start_time, philo->i,
			"has taken a fork", &philo->prog->print_mutex,philo->prog);
	}
	lock_routine(gettime() - philo->prog->start_time, philo->i,
			"is eating", &philo->prog->print_mutex,philo->prog);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = gettime();
	pthread_mutex_unlock(&philo->meal_mutex);
	ft_sleep(philo->prog->time_to_eat, philo->prog);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_count++;
	pthread_mutex_unlock(&philo->meal_mutex);
}
void	ft_sleep(long time, t_program *program)
{
	long	slice;
	long	remaining;

	remaining = time;
	while (remaining > 0)
	{
		pthread_mutex_lock(&program->stop_mutex);
		if (program->stop_flag == 1)
		{
			pthread_mutex_unlock(&program->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&program->stop_mutex);
		if (remaining > 1)
			slice = 1000;
		else
			slice = remaining * 1000;
		usleep(slice);
		remaining -= (slice / 1000);
	}
}
static void	*philo_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg; // eat -> sleep -> think -> repeat 
	while (1)
	{
		pthread_mutex_lock(&philo->prog->stop_mutex);
		if (philo->prog->stop_flag == 1)
		{
			pthread_mutex_unlock(&philo->prog->stop_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->prog->stop_mutex);
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->prog->meals_must_eat != -1
			&& philo->meals_count >= philo->prog->meals_must_eat)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		eating_process(philo);
		lock_routine(gettime() - philo->prog->start_time, philo->i,
			"is sleeping", &philo->prog->print_mutex,philo->prog);
		ft_sleep(philo->prog->time_to_sleep, philo->prog);
		lock_routine(gettime() - philo->prog->start_time, philo->i,
			"is thinking", &philo->prog->print_mutex,philo->prog);
	}
	return (NULL);
}
static void	*one_philo_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	pthread_mutex_lock(philo->left_fork);
	lock_routine(gettime() - philo->prog->start_time, philo->i,
			"has taken a fork", &philo->prog->print_mutex,philo->prog);
	ft_sleep(philo->prog->time_to_die, philo->prog);
	lock_routine(gettime() - philo->prog->start_time, philo->i,
			"died", &philo->prog->print_mutex,philo->prog);
	pthread_mutex_lock(&philo->prog->stop_mutex);
	philo->prog->stop_flag = 1;
	pthread_mutex_unlock(&philo->prog->stop_mutex);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}
static void *monitor_routine(void *arg)
{
	t_program	*prog;
	int			i;
	int	count;
	
	prog = (t_program *)arg;
	pthread_mutex_lock(&prog->stop_mutex);
	if (prog->stop_flag == 1)
	{
		pthread_mutex_unlock(&prog->stop_mutex);
		return NULL;
	}
	pthread_mutex_unlock(&prog->stop_mutex);
	while (1)
	{
		count = 0;
		i = 0;
		while (i < prog->n_of_philos)
		{
			pthread_mutex_lock(&prog->philos[i].meal_mutex);
			if (gettime() - prog->philos[i].last_meal >= prog->time_to_die)
			{
				lock_routine(gettime() - prog->start_time,
					prog->philos[i].i, "died", &prog->print_mutex,prog);
				pthread_mutex_unlock(&prog->philos[i].meal_mutex);
				pthread_mutex_lock(&prog->stop_mutex);
				prog->stop_flag = 1;
				pthread_mutex_unlock(&prog->stop_mutex);
				return NULL;
			}
			else if (prog->meals_must_eat != -1 && prog->philos[i].meals_count >= prog->meals_must_eat)
				count++;
			if (count == prog->n_of_philos)
			{
				pthread_mutex_lock(&prog->stop_mutex);
				prog->stop_flag = 1;
				pthread_mutex_unlock(&prog->stop_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&prog->philos[i].meal_mutex);
			i++;
		}
		ft_sleep(1, prog);
	}
}
void	create_join_threads(t_program *program)
{
	int	i;

	i = 0;
	init_all_mutexes(program);
	if (program->n_of_philos == 1)
	{
		if (pthread_create(&program->philos[i].philo_thread, NULL,
				one_philo_routine, &program->philos[i]) != 0)
			args_error("pthread_create failed\n");
	}
	else
	{
		while (i < program->n_of_philos)
		{
			if (pthread_create(&program->philos[i].philo_thread, NULL,
					philo_routine, &program->philos[i]) != 0)
				args_error("pthread_create failed \n");
			i++;
		}
	}
	if (pthread_create(&program->monitor, NULL, monitor_routine, program) != 0)
		args_error("pthread create failed\n");
	i = 0;
	while (i < program->n_of_philos)
	{
		if (pthread_join(program->philos[i].philo_thread, NULL) != 0)
			args_error(" pthread_join failed\n");
		i++;
	}
	if (pthread_join(program->monitor,NULL) != 0)
		args_error("pthread_join failed\n");
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
