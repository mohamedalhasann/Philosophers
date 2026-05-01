/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamed <mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 17:18:10 by malhassa          #+#    #+#             */
/*   Updated: 2026/05/01 20:34:22 by mohamed          ###   ########.fr       */
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

void	even_pick_forks(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_fork);
	lock_routine(gettime() - philo->prog->start_time,
		&philo->prog->print_mutex, philo, "has taken a fork");
	pthread_mutex_lock(&philo->prog->stop_mutex);
	if (philo->prog->stop_flag == 1)
	{
		pthread_mutex_unlock(&philo->prog->stop_mutex);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	pthread_mutex_unlock(&philo->prog->stop_mutex);
	pthread_mutex_lock(philo->right_fork);
	lock_routine(gettime() - philo->prog->start_time,
		&philo->prog->print_mutex, philo, "has taken a fork");
}
void	odd_pick_forks(t_philosopher *philo)
{
	pthread_mutex_lock(philo->right_fork);
	lock_routine(gettime() - philo->prog->start_time,
		&philo->prog->print_mutex, philo, "has taken a fork");
	pthread_mutex_lock(&philo->prog->stop_mutex);
	if (philo->prog->stop_flag == 1)
	{
		pthread_mutex_unlock(&philo->prog->stop_mutex);
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	pthread_mutex_unlock(&philo->prog->stop_mutex);
	pthread_mutex_lock(philo->left_fork);
	lock_routine(gettime() - philo->prog->start_time,
		&philo->prog->print_mutex, philo, "has taken a fork");
}
void	eating_process(t_philosopher *philo)
{
	if (philo->i%2 == 0)
		even_pick_forks(philo);
	else
		odd_pick_forks(philo);
	lock_routine(gettime() - philo->prog->start_time,
			&philo->prog->print_mutex, philo, "is eating");
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
	lock_routine(gettime() - philo->prog->start_time,
			&philo->prog->print_mutex, philo, "is sleeping");
	ft_sleep(philo->prog->time_to_sleep, philo->prog);
	lock_routine(gettime() - philo->prog->start_time,
			&philo->prog->print_mutex, philo, "is thinking");
}