/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 12:17:11 by malhassa          #+#    #+#             */
/*   Updated: 2026/05/16 17:51:21 by malhassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	take_forks_even(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_fork);
	lock_routine(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "has taken a fork");
	if (check_stop_flag(philo->prog))
	{
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	pthread_mutex_lock(philo->right_fork);
	lock_routine(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "has taken a fork");
	if (check_stop_flag(philo->prog))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	return (1);
}

int	take_forks_odd(t_philosopher *philo)
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
	pthread_mutex_lock(&philo->prog->stop_mutex);
	if (philo->prog->stop_flag == 1)
	{
		pthread_mutex_unlock(&philo->prog->stop_mutex);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	pthread_mutex_unlock(&philo->prog->stop_mutex);
	return (1);
}

int	try_start_eating(t_philosopher *philo)
{
	long	now;

	pthread_mutex_lock(&philo->meal_mutex);
	now = gettime();
	if (now - philo->last_meal > philo->prog->time_to_die)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	philo->last_meal = now;
	pthread_mutex_unlock(&philo->meal_mutex);
	lock_routine(now - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "is eating");
	return (1);
}

void	eat_cycle(t_philosopher *philo)
{
	if (philo->i % 2 == 0)
	{
		if (!take_forks_even(philo))
			return ;
	}
	else
	{
		if (!take_forks_odd(philo))
			return ;
	}
	if (!try_start_eating(philo))
		return ;
	ft_sleep(philo->prog->time_to_eat, philo->prog);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_count++;
	pthread_mutex_unlock(&philo->meal_mutex);
}

void	sleep_cycle(t_philosopher *philo)
{
	lock_routine(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "is sleeping");
	ft_sleep(philo->prog->time_to_sleep, philo->prog);
	lock_routine(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "is thinking");
}
