/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamed <mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 12:17:18 by malhassa          #+#    #+#             */
/*   Updated: 2026/05/18 00:20:44 by mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	print_action(long time, pthread_mutex_t *mutex, t_philosopher *philo,
		char *philo_action)
{
	pthread_mutex_lock(mutex);
	printf("%ld %d %s\n", time, philo->i, philo_action);
	pthread_mutex_unlock(mutex);
}
int	check_stop_flag(t_program *prog)
{
	pthread_mutex_lock(&prog->stop_mutex);
	if (prog->stop_flag == 1)
	{
		pthread_mutex_unlock(&prog->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&prog->stop_mutex);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_program	*prog;
	int			i;
	int			count;

	prog = (t_program *)arg;
	if (check_stop_flag(prog))
		return (NULL);
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
		usleep(1000);
	}
}

void	*philo_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (1)
	{
		if (check_stop_flag(philo->prog))
			break ;
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->prog->meals_must_eat != -1
			&& philo->meals_count >= philo->prog->meals_must_eat)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		if (!eat_cycle(philo))
			break ;
		if (!sleep_cycle(philo))
			break ;
	}
	return (NULL);
}

void	*one_philo_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	pthread_mutex_lock(philo->left_fork);
	print_action(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "has taken a fork");
	if (!ft_sleep(philo->prog->time_to_die, philo->prog))
	{
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	set_stop_flag(philo->prog);
	print_action(gettime() - philo->prog->start_time, &philo->prog->print_mutex,
		philo, "died");
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}
