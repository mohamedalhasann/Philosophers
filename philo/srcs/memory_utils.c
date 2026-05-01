
#include "../philosophers.h"

void	destroy_all_mutexes(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->n_of_philos)
	{
		pthread_mutex_destroy(&program->forks[i]);
		pthread_mutex_destroy(&program->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&program->print_mutex);
	pthread_mutex_destroy(&program->stop_mutex);
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

void	init_all_mutexes(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->n_of_philos)
	{
		pthread_mutex_init(&program->forks[i], NULL);
		pthread_mutex_init(&program->philos[i].meal_mutex, NULL);
		i++;
	}
	pthread_mutex_init(&program->print_mutex, NULL);
	pthread_mutex_init(&program->stop_mutex, NULL);
	i = 0;
	while (i < program->n_of_philos)
	{
		program->philos[i].i = i + 1;
		program->philos[i].left_fork = &program->forks[i];
		program->philos[i].right_fork = &program->forks[(i + 1)
			% program->n_of_philos];
		program->philos[i].prog = program;
		program->philos[i].meals_count = 0;
		program->philos[i].last_meal = program->start_time;
		i++;
	}
}

int	allocate_data(t_program *program)
{
	int	create_result;
	int	join_result;

	program->philos = malloc(program->n_of_philos * sizeof(t_philosopher));
	if (!program->philos)
		return (0);
	program->forks = malloc(program->n_of_philos * sizeof(pthread_mutex_t));
	if (!program->forks)
	{
		free(program->philos);
		return (0);
	}
	program->start_time = gettime();
	program->stop_flag = 0;
	create_result = create_threads(program);
	join_result = join_threads(program);
	if (!create_result)
		write(2, "pthread_create failed\n", 23);
	else if (!join_result)
		write(2, "pthread_join failed\n", 21);
	return (create_result && join_result);
}

void	args_error(char *message)
{
	write(2, message, ft_strlen(message));
	exit(1);
}
