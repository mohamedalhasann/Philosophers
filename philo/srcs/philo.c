/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:27:25 by malhassa          #+#    #+#             */
/*   Updated: 2026/04/30 17:40:45 by malhassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
#include <pthread.h>
#include <stddef.h>

int	stop_simulation_check(t_program *prog, int i, int *count)
{
	pthread_mutex_lock(&prog->philos[i].meal_mutex);
	if (gettime() - prog->philos[i].last_meal >= prog->time_to_die)
	{
		lock_routine(gettime() - prog->start_time, prog->philos[i].i, "died",
			&prog->print_mutex, prog);
		pthread_mutex_unlock(&prog->philos[i].meal_mutex);
		pthread_mutex_lock(&prog->stop_mutex);
		prog->stop_flag = 1;
		pthread_mutex_unlock(&prog->stop_mutex);
		return (1);
	}
	if (prog->meals_must_eat != -1
		&& prog->philos[i].meals_count >= prog->meals_must_eat)
		(*count)++;
	pthread_mutex_unlock(&prog->philos[i].meal_mutex);
	if (*count == prog->n_of_philos)
	{
		pthread_mutex_lock(&prog->stop_mutex);
		prog->stop_flag = 1;
		pthread_mutex_unlock(&prog->stop_mutex);
		return (1);
	}
	return (0);
}

void	create_threads(t_program *program)
{
	int	i;

	i = 0;
	init_all_mutexes(program);
	if (program->n_of_philos == 1)
	{
		if (pthread_create(&program->philos[i].philo_thread, NULL,
				one_philo_routine, &program->philos[i]) != 0)
			args_error("pthread_create failed\n", program);
	}
	else
	{
		while (i < program->n_of_philos)
		{
			if (pthread_create(&program->philos[i].philo_thread, NULL,
					philo_routine, &program->philos[i]) != 0)
				args_error("pthread_create failed \n", program);
			i++;
		}
	}
	if (pthread_create(&program->monitor, NULL, monitor_routine, program) != 0)
		args_error("pthread create failed\n", program);
}

void	join_threads(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->n_of_philos)
	{
		if (pthread_join(program->philos[i].philo_thread, NULL) != 0)
			args_error(" pthread_join failed\n", program);
		i++;
	}
	if (pthread_join(program->monitor, NULL) != 0)
		args_error("pthread_join failed\n", program);
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
	create_threads(program);
	join_threads(program);
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
		args_error("allocation failed\n", &program);
	memory_cleanup(&program);
	return (0);
}
