
#include "../philosophers.h"
#include <pthread.h>
#include <stddef.h>

int	stop_simulation_check(t_program *prog, int i, int *count)
{
	pthread_mutex_lock(&prog->philos[i].meal_mutex);
	if (gettime() - prog->philos[i].last_meal >= prog->time_to_die)
	{
		lock_routine(gettime() - prog->start_time, &prog->print_mutex,
			&prog->philos[i], "died");
		pthread_mutex_unlock(&prog->philos[i].meal_mutex);
		pthread_mutex_lock(&prog->stop_mutex);
		prog->stop_flag = 1;
		pthread_mutex_unlock(&prog->stop_mutex);
		return (1);
	}
	if (prog->meals_must_eat != -1
		&& prog->philos[i].meals_count >= prog->meals_must_eat)
		(*count)++;
	pthread_mutex_unlock(&prog->philos[i].meal_mutex);
	if (*count == prog->n_of_philos)
	{
		pthread_mutex_lock(&prog->stop_mutex);
		prog->stop_flag = 1;
		pthread_mutex_unlock(&prog->stop_mutex);
		return (1);
	}
	return (0);
}

int	create_threads(t_program *program)
{
	int	i;

	i = 0;
	init_all_mutexes(program);
	if (program->n_of_philos == 1)
	{
		if (pthread_create(&program->philos[i].philo_thread, NULL,
				one_philo_routine, &program->philos[i]) != 0)
			return (0);
	}
	else
	{
		while (i < program->n_of_philos)
		{
			if (pthread_create(&program->philos[i].philo_thread, NULL,
					philo_routine, &program->philos[i]) != 0)
				return (0);
			i++;
		}
	}
	if (pthread_create(&program->monitor, NULL, monitor_routine, program) != 0)
		return (0);
	return (1);
}

int	join_threads(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->n_of_philos)
	{
		if (pthread_join(program->philos[i].philo_thread, NULL) != 0)
			return (0);
		i++;
	}
	if (pthread_join(program->monitor, NULL) != 0)
		return (0);
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
	memory_cleanup(&program);
	return (0);
}
