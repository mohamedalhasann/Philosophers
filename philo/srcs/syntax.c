/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 12:17:22 by malhassa          #+#    #+#             */
/*   Updated: 2026/05/16 15:34:13 by malhassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

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

int	args_check(t_program *program, int argc, char **argv)
{
	program->n_of_philos = ft_atoi(argv[1]);
	if (program->n_of_philos < 1)
		return (args_error("invalid number of philosophres\n"));
	program->time_to_die = ft_atoi(argv[2]);
	program->time_to_eat = ft_atoi(argv[3]);
	program->time_to_sleep = ft_atoi(argv[4]);
	if (!time_check(program))
		return (args_error("invalid time\n"));
	if (argc == 6)
	{
		program->meals_must_eat = ft_atoi(argv[5]);
		if (program->meals_must_eat <= 0)
			return (args_error("invalid number of meals\n"));
	}
	else
		program->meals_must_eat = -1;
	return (1);
}
