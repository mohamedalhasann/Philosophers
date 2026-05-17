/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamed <mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 12:16:59 by malhassa          #+#    #+#             */
/*   Updated: 2026/05/17 23:14:09 by mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (i);
	while (s[i])
		i++;
	return (i);
}

int	ft_atoi(const char *nptr)
{
	long	result;

	if (!nptr)
		return (-1);
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '-')
		return (-1);
	if (*nptr == '+')
		nptr++;
	if (*nptr < '0' || *nptr > '9')
		return (-1);
	result = 0;
	while (*nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + (*nptr - '0');
		if (result > INT_MAX)
			return (-1);
		nptr++;
	}
	if (*nptr != '\0')
		return (-1);
	return ((int)result);
}

long	gettime(void)
{
	struct timeval	tev;
	long			current_time;

	gettimeofday(&tev, NULL);
	current_time = (tev.tv_sec * 1000) + (tev.tv_usec / 1000);
	return (current_time);
}

int	ft_strcmp(char *str, char *str2)
{
	int	i;

	i = 0;
	while (str[i] && str2[i] && str[i] == str2[i])
		i++;
	return ((unsigned char)str[i] - (unsigned char)str2[i]);
}

int	ft_sleep(long time, t_program *program)
{
	long	start;

	start = gettime();
	while ((gettime() - start) < time)
	{
		pthread_mutex_lock(&program->stop_mutex);
		if (program->stop_flag)
		{
			pthread_mutex_unlock(&program->stop_mutex);
			return (0);
		}
		pthread_mutex_unlock(&program->stop_mutex);
		usleep(500);
	}
	return (1);
}
