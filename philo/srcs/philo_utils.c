/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamed <mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:33:59 by malhassa          #+#    #+#             */
/*   Updated: 2026/04/19 22:04:52 by mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_atoi(const char *nptr)//non-digit rejection/overflow detection?
{
	int	sign;
	int	result;

	sign = 1;
	result = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			sign *= -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + (*nptr - '0');
		nptr++;
	}
	return (result * sign);
}

long	gettime(void)
{
	struct timeval tev;
	long	current_time;
	
	gettimeofday(&tev,NULL);
	current_time = (tev.tv_sec * 1000) + (tev.tv_usec / 1000);
	return (current_time);
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
