/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malhassa <malhassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 12:17:27 by malhassa          #+#    #+#             */
/*   Updated: 2026/05/02 12:49:34 by malhassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

typedef struct s_program	t_program;
typedef struct s_philosopher
{
	int						i;
	pthread_t				philo_thread;
	long					last_meal;
	int						meals_count;
	pthread_mutex_t			meal_mutex;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
	t_program				*prog;
}							t_philosopher;
typedef struct s_program
{
	t_philosopher			*philos;
	int						n_of_philos;
	long					start_time;
	int						stop_flag;
	long					time_to_die;
	long					time_to_eat;
	long					time_to_sleep;
	int						meals_must_eat;
	pthread_mutex_t			*forks;
	pthread_mutex_t			stop_mutex;
	pthread_mutex_t			print_mutex;
	pthread_t				monitor;
}							t_program;

long						gettime(void);
void						memory_cleanup(t_program *program);
void						destroy_all_mutexes(t_program *program);
int							ft_atoi(const char *nptr);
int							time_check(t_program *program);
int							args_check(t_program *program, int argc,
								char **argv);
void						sleep_process(t_philosopher *philo);
void						lock_routine(long time, pthread_mutex_t *mutex,
								t_philosopher *philo, char *philo_action);
void						eating_process(t_philosopher *philo);
void						*philo_routine(void *arg);
int							stop_simulation_check(t_program *prog, int i,
								int *count);
void						ft_sleep(long time, t_program *program);
void						*one_philo_routine(void *arg);
int							create_threads(t_program *program);
int							join_threads(t_program *program);
int							ft_strlen(char *s);
void						*monitor_routine(void *arg);
void						init_all_mutexes(t_program *program);
int							allocate_data(t_program *program);
int							ft_strcmp(char *str, char *str2);
int							args_error(char *message);
#endif
