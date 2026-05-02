/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 12:17:11 by malhassa          #+#    #+#             */
/*   Updated: 2026/05/02 14:26:13 by malhassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

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

int	even_pick_forks(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_fork);
	lock_routine(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "has taken a fork");
	pthread_mutex_lock(&philo->prog->stop_mutex);
	if (philo->prog->stop_flag == 1)
	{
		pthread_mutex_unlock(&philo->prog->stop_mutex);
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	pthread_mutex_unlock(&philo->prog->stop_mutex);
	pthread_mutex_lock(philo->right_fork);
	lock_routine(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "has taken a fork");
	return (1);
}

int	odd_pick_forks(t_philosopher *philo)
{
	pthread_mutex_lock(philo->right_fork);
	lock_routine(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "has taken a fork");
	pthread_mutex_lock(&philo->prog->stop_mutex);
	if (philo->prog->stop_flag == 1)
	{
		pthread_mutex_unlock(&philo->prog->stop_mutex);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	pthread_mutex_unlock(&philo->prog->stop_mutex);
	pthread_mutex_lock(philo->left_fork);
	lock_routine(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "has taken a fork");
	return (1);
}

void	eating_process(t_philosopher *philo)
{
	if (philo->i % 2 == 0)
	{
		if (!even_pick_forks(philo))
			return ;
	}
	else
	{
		if (!odd_pick_forks(philo))
			return ;
	}
	lock_routine(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "is eating");
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

void	sleep_process(t_philosopher *philo)
{
	lock_routine(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "is sleeping");
	ft_sleep(philo->prog->time_to_sleep, philo->prog);
	lock_routine(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "is thinking");
	if (philo->prog->n_of_philos % 2 == 1)
			ft_sleep(1, philo->prog);
}
