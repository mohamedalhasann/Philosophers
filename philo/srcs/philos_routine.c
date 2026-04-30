/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 17:21:01 by malhassa          #+#    #+#             */
/*   Updated: 2026/04/30 17:26:37 by malhassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	*monitor_routine(void *arg)
{
	t_program	*prog;
	int			i;
	int			count;

	prog = (t_program *)arg;
	pthread_mutex_lock(&prog->stop_mutex);
	if (prog->stop_flag == 1)
	{
		pthread_mutex_unlock(&prog->stop_mutex);
		return (NULL);
	}
	pthread_mutex_unlock(&prog->stop_mutex);
	while (1)
	{
		count = 0;
		i = 0;
		while (i < prog->n_of_philos)
		{
			if (stop_simulation_check(prog, i, &count))
				return (NULL);
			i++;
		}
		ft_sleep(1, prog);
	}
}

void	*philo_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->prog->stop_mutex);
		if (philo->prog->stop_flag == 1)
		{
			pthread_mutex_unlock(&philo->prog->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->prog->stop_mutex);
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->prog->meals_must_eat != -1
			&& philo->meals_count >= philo->prog->meals_must_eat)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		eating_process(philo);
		sleep_process(philo);
	}
	return (NULL);
}
void	*one_philo_routine(void *arg)
{
	t_philosopher *philo;

	philo = (t_philosopher *)arg;
	pthread_mutex_lock(philo->left_fork);
	lock_routine(gettime() - philo->prog->start_time, philo->i,
			"has taken a fork", &philo->prog->print_mutex, philo->prog);
	ft_sleep(philo->prog->time_to_die, philo->prog);
	lock_routine(gettime() - philo->prog->start_time, philo->i, "died",
			&philo->prog->print_mutex, philo->prog);
	pthread_mutex_lock(&philo->prog->stop_mutex);
	philo->prog->stop_flag = 1;
	pthread_mutex_unlock(&philo->prog->stop_mutex);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}