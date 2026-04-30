/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 17:18:10 by malhassa          #+#    #+#             */
/*   Updated: 2026/04/30 17:42:15 by malhassa         ###   ########.fr       */
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

void	lock_routine(long time, int i, char *philo_action,
		pthread_mutex_t *mutex, t_program *program)
{
	pthread_mutex_lock(&program->stop_mutex);
	if (program->stop_flag != 1 || ft_strcmp(philo_action, "died") == 0)
	{
		pthread_mutex_lock(mutex);
		printf("%ld %d %s\n", time, i, philo_action);
		pthread_mutex_unlock(mutex);
	}
	pthread_mutex_unlock(&program->stop_mutex);
}

void	pick_forks(t_philosopher *philo)
{
	if (philo->i % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		lock_routine(gettime() - philo->prog->start_time, philo->i,
				"has taken a fork", &philo->prog->print_mutex, philo->prog);
		pthread_mutex_lock(philo->right_fork);
		lock_routine(gettime() - philo->prog->start_time, philo->i,
				"has taken a fork", &philo->prog->print_mutex, philo->prog);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		lock_routine(gettime() - philo->prog->start_time, philo->i,
				"has taken a fork", &philo->prog->print_mutex, philo->prog);
		pthread_mutex_lock(philo->left_fork);
		lock_routine(gettime() - philo->prog->start_time, philo->i,
				"has taken a fork", &philo->prog->print_mutex, philo->prog);
	}
}
void	eating_process(t_philosopher *philo)
{
	pick_forks(philo);
	lock_routine(gettime() - philo->prog->start_time, philo->i, "is eating",
			&philo->prog->print_mutex, philo->prog);
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
	lock_routine(gettime() - philo->prog->start_time, philo->i, "is sleeping",
			&philo->prog->print_mutex, philo->prog);
	ft_sleep(philo->prog->time_to_sleep, philo->prog);
	lock_routine(gettime() - philo->prog->start_time, philo->i, "is thinking",
			&philo->prog->print_mutex, philo->prog);
}