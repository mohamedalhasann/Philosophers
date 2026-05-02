/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 12:17:18 by malhassa          #+#    #+#             */
/*   Updated: 2026/05/02 14:26:29 by malhassa         ###   ########.fr       */
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

void	lock_routine(long time, pthread_mutex_t *mutex, t_philosopher *philo,
		char *philo_action)
{
	pthread_mutex_lock(&philo->prog->stop_mutex);
	if (philo->prog->stop_flag != 1 || ft_strcmp(philo_action, "died") == 0)
	{
		pthread_mutex_lock(mutex);
		printf("%ld %d %s\n", time, philo->i, philo_action);
		pthread_mutex_unlock(mutex);
	}
	pthread_mutex_unlock(&philo->prog->stop_mutex);
}

void	*one_philo_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	pthread_mutex_lock(philo->left_fork);
	lock_routine(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "has taken a fork");
	ft_sleep(philo->prog->time_to_die, philo->prog);
	pthread_mutex_lock(&philo->prog->stop_mutex);
	philo->prog->stop_flag = 1;
	pthread_mutex_unlock(&philo->prog->stop_mutex);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}
