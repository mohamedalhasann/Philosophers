/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamed <mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:27:25 by malhassa          #+#    #+#             */
/*   Updated: 2026/04/26 22:53:52 by mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	lock_routine(long time,int i,char *philo_action, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	printf("%ld Philo %d %s\n",time,i,philo_action);
	pthread_mutex_unlock(mutex);
}
// static void critical_section(pthread_mutex_t *mutex, char *action)
// {
// 	if (strcmp(action,"last meal"))
// 	{
// 		pthread_mutex_lock(&mutex);
		
// 	}
// }
static void	eating_process(t_philosopher *philo)
{
	if (philo -> i % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		lock_routine(gettime() - philo->prog->start_time, philo->i,"has taken a fork",&philo->prog->print_mutex);
		pthread_mutex_lock(philo->right_fork);
		lock_routine(gettime() - philo->prog->start_time, philo->i,"has taken a fork",&philo->prog->print_mutex);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		lock_routine(gettime() - philo->prog->start_time, philo->i,"has taken a fork",&philo->prog->print_mutex);
		pthread_mutex_lock(philo->left_fork);
		lock_routine(gettime() - philo->prog->start_time, philo->i,"has taken a fork",&philo->prog->print_mutex);
	}
	lock_routine(gettime() - philo->prog->start_time, philo->i, "is eating", &philo->prog->print_mutex);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = gettime();
	pthread_mutex_unlock(&philo->meal_mutex);
	usleep(philo->prog->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_count++;
	pthread_mutex_unlock(&philo->meal_mutex);
}
static void	*philo_routine(void *arg)
{
	t_philosopher *philo;
	
	philo = (t_philosopher *)arg; // eat -> sleep -> think -> repeat
	while (1)
	{
		pthread_mutex_lock(&philo->prog->stop_mutex);
		if (philo->prog->stop_flag == 1)
		{
			pthread_mutex_unlock(&philo->prog->stop_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->prog->stop_mutex);
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->prog->meals_must_eat != -1 && philo->meals_count >= philo->prog->meals_must_eat)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		eating_process(philo);
		lock_routine(gettime() - philo->prog->start_time, philo->i, "is sleeping", &philo->prog->print_mutex);
		usleep(philo->prog->time_to_sleep * 1000);
		lock_routine(gettime() - philo->prog->start_time, philo->i, "is thinking", &philo->prog->print_mutex);
	}
	return (NULL);
}

void	create_join_threads(t_program *program)
{	
	int	i;

	i = 0;
	init_all_mutexes(program);
	while (i < program->n_of_philos)
	{
		if (pthread_create(&program->philos[i].philo_thread, NULL,
				philo_routine, &program->philos[i]) != 0)
			args_error("pthread_create failed \n");
		i++;
	}
	i = 0;
	while (i < program->n_of_philos)
	{
		if (pthread_join(program->philos[i].philo_thread,  NULL) != 0)
			args_error(" pthread_join failed\n");
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
	program->stop_flag = 0;
	create_join_threads(program);
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
