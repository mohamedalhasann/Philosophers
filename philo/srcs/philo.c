/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:27:25 by malhassa          #+#    #+#             */
/*   Updated: 2026/04/16 16:28:13 by malhassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"


static void args_error(char *message)
{
    printf("%s",message);
    exit(1);
}
static int  time_check(t_program *program)
{
    if (program->time_to_die <= 0)
        return (0);
    if (program->time_to_eat <= 0)
        return (0);
    if (program->time_to_sleep <= 0)
        return (0);
    return (1);
}
static void args_check(t_program *program,int argc, char **argv)
{    
    program->n_of_philos = ft_atoi(argv[1]);
    if (program->n_of_philos < 1)
        args_error("invalid number of philosophres\n");
    program->time_to_die = ft_atoi(argv[2]);
    program->time_to_eat = ft_atoi(argv[3]);
    program->time_to_sleep = ft_atoi(argv[4]);
    if (!time_check(program))
        args_error("invalid time\n");
    if (argc == 6)
    {
        program->meals_must_eat = ft_atoi(argv[5]);
        if (program->meals_must_eat <= 0)
            args_error("invalid number of meals\n");   
    }
    else
        program->meals_must_eat = -1;
}

static void	*philo_routine(void *arg)
{
	(void)arg;
	return (NULL);
}

void	create_threads(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->n_of_philos)
	{
		if (pthread_create(&program->philos[i].philo_thread, NULL,
				philo_routine, &program->philos[i]) != 0)
			args_error("pthread_create failed\n");
		i++;
	}
}

int	allocate_data(t_program *program)
{
	program->philos = malloc(program->n_of_philos * sizeof(t_philosopher));
	if (!program->philos)
		return (0);
	program->forks = malloc(program->n_of_philos * sizeof(pthread_mutex_t));
	if (!program->forks)
	{
		free(program->philos);
		program->philos = NULL;
		return (0);
	}
	program->start_time = gettime();
	create_threads(program);
	return (1);
}

int	main(int argc, char **argv)
{
	t_program	program;

	if (argc != 5 && argc != 6)
	{
		printf("invalid number of arguments\n");
		exit(1);
	}
	args_check(&program, argc, argv);
	if (!allocate_data(&program))
		args_error("allocation failed\n");
	return (0);
}
