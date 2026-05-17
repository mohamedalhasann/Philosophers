/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamed <mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 12:16:50 by malhassa          #+#    #+#             */
/*   Updated: 2026/05/18 00:16:51 by mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	destroy_all_mutexes(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->n_of_philos)
	{
		pthread_mutex_destroy(&program->forks[i]);
		pthread_mutex_destroy(&program->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&program->print_mutex);
	pthread_mutex_destroy(&program->stop_mutex);
}

void	memory_cleanup(t_program *program)
{
	if (!program)
		return ;
	destroy_all_mutexes(program);
	if (program->philos)
		free(program->philos);
	if (program->forks)
		free(program->forks);
}

void	init_all_mutexes(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->n_of_philos)
	{
		pthread_mutex_init(&program->forks[i], NULL);
		pthread_mutex_init(&program->philos[i].meal_mutex, NULL);
		i++;
	}
	pthread_mutex_init(&program->print_mutex, NULL);
	pthread_mutex_init(&program->stop_mutex, NULL);
	program->start_time = gettime();
	program->stop_flag = 0;
	i = 0;
	while (i < program->n_of_philos)
	{
		program->philos[i].i = i + 1;
		program->philos[i].left_fork = &program->forks[i];
		program->philos[i].right_fork = &program->forks[(i + 1)
			% program->n_of_philos];
		program->philos[i].prog = program;
		program->philos[i].meals_count = 0;
		program->philos[i].last_meal = program->start_time;
		i++;
	}
}

int	allocate_data(t_program *program)
{
	int	create_result;

	create_result = 1;
	program->philos = malloc(program->n_of_philos * sizeof(t_philosopher));
	if (!program->philos)
		return (0);
	program->forks = malloc(program->n_of_philos * sizeof(pthread_mutex_t));
	if (!program->forks)
	{
		free(program->philos);
		return (0);
	}
	init_all_mutexes(program);
	if (program->n_of_philos == 1)
	{
		if (pthread_create(&program->philos[0].philo_thread, NULL,
				one_philo_routine, &program->philos[0]) != 0)
			create_result = 0;
		if (create_result)
			create_result = pthread_join(program->philos[0].philo_thread, NULL) == 0;
	}
	else
		create_result = create_threads(program);
	return (create_result);
}

int	create_threads(t_program *program)
{
	int	i;
	int	monitor_created;
	
	monitor_created = 0;
	i = 0;
	while (i < program->n_of_philos)
	{
		if (pthread_create(&program->philos[i].philo_thread, NULL,
				philo_routine, &program->philos[i]) != 0)
				break;
		i++;	
	}
	monitor_created = (i == program->n_of_philos);
	if (monitor_created && pthread_create(&program->monitor, NULL, monitor_routine, program) != 0)
		monitor_created = 0;
	while (--i >= 0)
		pthread_join(program->philos[i].philo_thread, NULL);
	if (monitor_created)
		pthread_join(program->monitor, NULL);
	return (monitor_created);
}
