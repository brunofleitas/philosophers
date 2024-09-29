/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02-init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 19:21:43 by bruno             #+#    #+#             */
/*   Updated: 2024/09/29 21:58:56 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_table *init_table(int argc, char **argv)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL);
	init_table_data(table, argc, argv);
	if (!init_forks(table) || !init_philosophers(table))
	{
		if (table->forks)
			free(table->forks);
		if (table->philosophers)
			free(table->philosophers);
		free(table);
		return (NULL);
	}
	return (table);
}

void init_table_data(t_table *table, int argc, char **argv)
{
	struct timeval tv;

	table->philo_count = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]) * 1000;
	table->time_to_eat = ft_atoi(argv[3]) * 1000;
	table->time_to_sleep = ft_atoi(argv[4]) * 1000;
	if (argc == 6)
		table->meal_count = ft_atoi(argv[5]);
	else
		table->meal_count = -1;
	gettimeofday(&tv, NULL);
	table->start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	table->end_flag = 0;
	table->all_threads_created = 0;
	mutex_handler(&table->table_mutex, INIT);
	mutex_handler(&table->write_mutex, INIT);
}

int init_forks(t_table *table)
{
	int i;

	table->forks = malloc(sizeof(t_fork) * table->philo_count);
	if (!table->forks)
		return (0);
	i = 0;
	while (i < table->philo_count)
	{
		mutex_handler(table->forks[i].fork_mutex, INIT);
		table->forks[i].fork = i;
		i++;
	}
	return (1);
}

int init_philosophers(t_table *table)
{
	int i;
	struct timeval tv;

	table->philosophers = malloc(sizeof(t_philosopher) * table->philo_count);
	if (!table->philosophers)
		return (0);
	i = 0;
	while (i < table->philo_count)
	{
		table->philosophers[i].id = i;
		table->philosophers[i].meal_count = 0;
		table->philosophers[i].is_full = 0;
		gettimeofday(&tv, NULL);
		table->philosophers[i].last_meal = tv.tv_sec * 1000 + tv.tv_usec / 1000;
		mutex_handler(&table->philosophers[i].philo_mutex, INIT);
		assign_forks(&table->philosophers[i], &table->forks[i], &table->forks[(i + 1) % table->philo_count], i);
		i++;
	}
	return (1);
}

void assign_forks(t_philosopher *philosopher, t_fork *left_fork, t_fork *right_fork, int id)
{
	if (id % 2 == 0)
	{
		philosopher->left_fork = left_fork;
		philosopher->right_fork = right_fork;
	}
	else
	{
		philosopher->left_fork = right_fork;
		philosopher->right_fork = left_fork;
	}
}


// t_table *init_table(int argc, char **argv)
// {
// 	t_table	*table;
// 	int		i;

// 	table = malloc(sizeof(t_table));
// 	if (!table)
// 		return (NULL);
// 	table->philo_count = ft_atoi(argv[1]);
// 	table->time_to_die = ft_atoi(argv[2]) * 1000;
// 	table->time_to_eat = ft_atoi(argv[3]) * 1000;
// 	table->time_to_sleep = ft_atoi(argv[4]) * 1000;
// 	if (argc == 6)
// 		table->meal_count = ft_atoi(argv[5]);
// 	else
// 		table->meal_count = -1;
// 	table->start_time = get_time();
// 	table->end_flag = 0;
// 	table->all_threads_created = 0;
// 	mutex_handler(&table->table_mutex, INIT);
// 	mutex_handler(&table->write_mutex, INIT);
// 	table->forks = malloc(sizeof(t_fork) * table->philo_count);
// 	if (!table->forks)
// 	{
// 		free(table);
// 		return (NULL);
// 	}
// 	i = 0;
// 	while (i < table->philo_count)
// 	{
// 		mutex_handler(&table->forks[i].fork_mutex, INIT);
// 		table->forks[i].fork = i;
// 		i++;
// 	}
// 	table->philosophers = malloc(sizeof(t_philosopher) * table->philo_count);
// 	if (!table->philosophers)
// 	{
// 		free(table->forks);
// 		free(table);
// 		return (NULL);
// 	}
// 	i = 0;
// 	while (i < table->philo_count)
// 	{
// 		table->philosophers[i].id = i;
// 		table->philosophers[i].meal_count = 0;
// 		table->philosophers[i].is_full = 0;
// 		table->philosophers[i].last_meal = get_time();
// 		mutex_handler(&table->philosophers[i].philo_mutex, INIT);
// 		if (i % 2 == 0)
// 		{
// 			table->philosophers[i].left_fork = &table->forks[i];
// 			table->philosophers[i].right_fork = &table->forks[(i + 1) % table->philo_count];
// 		}
// 		else
// 		{
// 			table->philosophers[i].left_fork = &table->forks[(i + 1) % table->philo_count];
// 			table->philosophers[i].right_fork = &table->forks[i];
// 		}
// 		i++;
// 	}
// 	return (table);
// }
