/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 17:39:30 by malhassa          #+#    #+#             */
/*   Updated: 2026/04/30 17:40:02 by malhassa         ###   ########.fr       */
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

int	ft_atoi(const char *nptr)
{
	long	result;

	if (!nptr)
		return (-1);
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '-')
		return (-1);
	if (*nptr == '+')
		nptr++;
	if (*nptr < '0' || *nptr > '9')
		return (-1);
	result = 0;
	while (*nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + (*nptr - '0');
		if (result > INT_MAX)
			return (-1);
		nptr++;
	}
	if (*nptr != '\0')
		return (-1);
	return ((int)result);
}

long	gettime(void)
{
	struct timeval	tev;
	long			current_time;

	gettimeofday(&tev, NULL);
	current_time = (tev.tv_sec * 1000) + (tev.tv_usec / 1000);
	return (current_time);
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

int	ft_strcmp(char *str, char *str2)
{
	int	i;

	i = 0;
	while (str[i] && str2[i] && str[i] == str2[i])
		i++;
	return ((unsigned char)str[i] - (unsigned char)str2[i]);
}
