/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamed <mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 12:17:11 by malhassa          #+#    #+#             */
/*   Updated: 2026/05/18 00:19:17 by mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	take_forks(t_philosopher *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	
	if (philo->i % 2 == 0)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(first_fork);
	print_action(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "has taken a fork");
	if (check_stop_flag(philo->prog))
	{
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	pthread_mutex_lock(second_fork);
	print_action(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "has taken a fork");
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
	print_action(now - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "is eating");
	return (1);
}

int	eat_cycle(t_philosopher *philo)
{
	if(!take_forks(philo))
		return (0);
	if (check_stop_flag(philo->prog))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	if (!try_start_eating(philo))
		return (0);
	if (!ft_sleep(philo->prog->time_to_eat, philo->prog))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_count++;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (1);
}

int	sleep_cycle(t_philosopher *philo)
{
	print_action(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "is sleeping");
	if (!ft_sleep(philo->prog->time_to_sleep, philo->prog))
		return (0);
	print_action(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "is thinking");
	return (1);
}
