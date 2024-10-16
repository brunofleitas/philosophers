/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02-init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 19:21:43 by bruno             #+#    #+#             */
/*   Updated: 2024/10/16 13:50:53 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Initializes the table structure.
 *
 * Allocates memory for the table structure and initializes its data, forks,
	and philosophers.
 * If any initialization fails, frees allocated memory and returns NULL.
 *
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return A pointer to the initialized table structure, or NULL on failure.
 */
t_table	*init_table(int argc, char **argv)
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

/**
 * @brief Initializes the table data.
 *
 * Sets the number of philosophers, time to die, time to eat, time to sleep,
	and meal count
 * based on the command-line arguments. Initializes the start time, end flag,
	and mutexes.
 *
 * @param table A pointer to the table structure.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 */
void	init_table_data(t_table *table, int argc, char **argv)
{
	table->philo_count = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		table->meal_count = ft_atoi(argv[5]) + 1;
	else
		table->meal_count = -1;
	table->start_time = get_time();
	table->end_flag = 0;
	table->all_threads_created = 0;
	mutex_handler(&table->table_mutex, INIT);
	mutex_handler(&table->write_mutex, INIT);
	mutex_handler(&table->sim, INIT);
	mutex_handler(&table->set_value, INIT);
}

/**
 * @brief Initializes the forks.
 *
 * Allocates memory for the forks and initializes their mutexes and IDs.
 *
 * @param table A pointer to the table structure.
 * @return 1 on success, 0 on failure.
 */
int	init_forks(t_table *table)
{
	int	i;

	table->forks = malloc(sizeof(t_fork) * table->philo_count);
	if (!table->forks)
		return (0);
	i = 0;
	while (i < table->philo_count)
	{
		mutex_handler(&table->forks[i].fork_mutex, INIT);
		table->forks[i].fork = i;
		i++;
	}
	return (1);
}

/**
 * @brief Initializes the philosophers.
 *
 * Allocates memory for the philosophers and initializes their IDs, meal counts,
	fullness status,
 * last meal times, and mutexes. Assigns forks to each philosopher.
 *
 * @param table A pointer to the table structure.
 * @return 1 on success, 0 on failure.
 */
int	init_philosophers(t_table *table)
{
	int	i;

	table->philosophers = malloc(sizeof(t_philosopher) * table->philo_count);
	if (!table->philosophers)
		return (0);
	i = 0;
	while (i < table->philo_count)
	{
		table->philosophers[i].id = i;
		table->philosophers[i].meal_count = 0;
		table->philosophers[i].is_full = 0;
		table->philosophers[i].last_meal = get_time();
		mutex_handler(&table->philosophers[i].philo_mutex, INIT);
		assign_forks(&table->philosophers[i], &table->forks[i], &table->forks[(i
				+ 1) % table->philo_count], i);
		table->philosophers[i].table = table;
		i++;
	}
	return (1);
}

/**
 * @brief Assigns forks to a philosopher.
 *
 * Assigns the left and right forks to the philosopher based on their ID.
 *
 * @param philosopher A pointer to the philosopher structure.
 * @param left_fork A pointer to the left fork.
 * @param right_fork A pointer to the right fork.
 * @param id The ID of the philosopher.
 */
void	assign_forks(t_philosopher *philosopher, t_fork *left_fork,
		t_fork *right_fork, int id)
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
