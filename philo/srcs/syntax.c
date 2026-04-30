/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 17:40:07 by malhassa          #+#    #+#             */
/*   Updated: 2026/04/30 17:40:17 by malhassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	args_error(char *message, t_program *prog)
{
	if (ft_strcmp(message, "pthread_create failed") == 0 || ft_strcmp(message,
			"invalid time\n") == 0 || ft_strcmp(message,
			"invalid number of philosophres\n") == 0 || ft_strcmp(message,
			"invalid number of meals\n") == 0 || ft_strcmp(message,
			"pthread_join failed\n") == 0)
	{
		if (prog->philos)
			free(prog->philos);
		free(prog);
	}
	printf("%s", message);
	exit(1);
}

int	time_check(t_program *program)
{
	if (program->time_to_die <= 0)
		return (0);
	if (program->time_to_eat <= 0)
		return (0);
	if (program->time_to_sleep <= 0)
		return (0);
	return (1);
}

void	args_check(t_program *program, int argc, char **argv)
{
	program->n_of_philos = ft_atoi(argv[1]);
	if (program->n_of_philos < 1)
		args_error("invalid number of philosophres\n", program);
	program->time_to_die = ft_atoi(argv[2]);
	program->time_to_eat = ft_atoi(argv[3]);
	program->time_to_sleep = ft_atoi(argv[4]);
	if (!time_check(program))
		args_error("invalid time\n", program);
	if (argc == 6)
	{
		program->meals_must_eat = ft_atoi(argv[5]);
		if (program->meals_must_eat <= 0)
			args_error("invalid number of meals\n", program);
	}
	else
		program->meals_must_eat = -1;
}

void	memory_cleanup(t_program *program)
{
	if (!program)
		return ;
	destroy_all_mutexes(program);
	if (program->philos)
		free(program->philos);
	if (program->forks)
		free(program->forks);
}
