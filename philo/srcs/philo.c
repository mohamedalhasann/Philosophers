/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamed <mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 12:17:07 by malhassa          #+#    #+#             */
/*   Updated: 2026/05/18 00:19:17 by mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	set_stop_flag(t_program *prog)
{
	pthread_mutex_lock(&prog->stop_mutex);
	prog->stop_flag = 1;
	pthread_mutex_unlock(&prog->stop_mutex);
}

int	stop_simulation_check(t_program *prog, int i, int *count)
{
	pthread_mutex_lock(&prog->philos[i].meal_mutex);
	if (prog->meals_must_eat != -1
		&& prog->philos[i].meals_count >= prog->meals_must_eat)
	{
		(*count)++;
		pthread_mutex_unlock(&prog->philos[i].meal_mutex);
		if (*count == prog->n_of_philos)
		{
			set_stop_flag(prog);
			return (1);
		}
		return (0);
	}
	if (gettime() - prog->philos[i].last_meal > prog->time_to_die)
	{
		set_stop_flag(prog);
		pthread_mutex_unlock(&prog->philos[i].meal_mutex);
		print_action(gettime() - prog->start_time, &prog->print_mutex,
			&prog->philos[i], "died");
		return (1);
	}
	pthread_mutex_unlock(&prog->philos[i].meal_mutex);
	return (0);
}

int	main(int argc, char **argv)
{
	t_program	program;

	if (argc != 5 && argc != 6)
	{
		printf("invalid number of arguments\n");
		return (1);
	}
	if (!args_check(&program, argc, argv))
		return (1);
	if (!allocate_data(&program))
	{
		write(2, "allocation failed\n", 19);
		return (1);
	}
	memory_cleanup(&program);
	return (0);
}
