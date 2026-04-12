/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamed <mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:27:25 by malhassa          #+#    #+#             */
/*   Updated: 2026/04/12 19:07:21 by mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
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
    {
        printf("invalid number of philosophres\n");
        exit(1);
    }
    program->time_to_die = ft_atoi(argv[2]);
    program->time_to_eat = ft_atoi(argv[3]);
    program->time_to_sleep = ft_atoi(argv[4]);
    if (!time_check(program))
    {
        printf("invalid time\n");
        exit(1);
    }
    if (argc == 6)
    {
        program->meals_must_eat = ft_atoi(argv[5]);
        if (program->meals_must_eat <= 0)
        {
            printf("invalid number of meals\n");
            exit(1);
        }    
    }
    else
        program->meals_must_eat = -1;
}
int main(int argc , char **argv)
{
    t_program   program;
    
    if (argc != 5 && argc != 6)
    {
        printf("invalid number of arguments\n");
        exit(1);
    }
    args_check(&program,argc,argv);
}
