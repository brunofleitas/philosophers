/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01-philosophers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfleitas <bfleitas@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:14:01 by bruno             #+#    #+#             */
/*   Updated: 2024/10/02 11:57:57 by bfleitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Converts a string to an integer.
 *
 * This function, `ft_atoi`, takes a string as input and converts it to 
 * an integer.
 * It handles optional leading whitespace characters, an optional sign 
 * character, and a sequence of numeric digits. If the resulting integer
 * exceeds the range
 * of `int`, the function returns 0.
 *
 * @param nptr The string to be converted to an integer.
 * @return The converted integer value, or 0 if the value is out of range.
 */
int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	long	result;

	i = 0;
	result = 0;
	sign = 1;
	while (nptr[i] != 0 && ((nptr[i] == ' ') || (nptr[i] >= 9
				&& nptr[i] <= 13)))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] != 0 && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		result = (nptr[i] - 48) + (result * 10);
		i++;
	}
	if (result > INT_MAX || result < INT_MIN)
		return (0);
	else
		return (sign * result);
}

/**
 * @brief Checks the validity of command-line arguments.
 *
 * This function verifies that the number of arguments is either 5 or 6.
 * It also ensures that each argument, when converted to an integer, is 
 * greater than 0.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return Returns 0 if the arguments are valid, otherwise returns 1.
 */
int	check_args(int argc, char **argv)
{
	int	i;

	if (argc > 6 || argc < 5)
		return (1);
	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) <= 0)
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Starts the simulation of philosophers dining.
 *
 * This function initializes and starts threads for each philosopher and a monitor thread.
 * It then waits for all philosopher threads and the monitor thread to complete.
 *
 * @param table A pointer to the table structure containing philosopher and monitor information.
 *
 * @return Always returns 0.
 */
int start_simulation(t_table *table)
{
	int i;

	i = 0;
    while (i < table->philo_count)
	{
        thread_handler(&table->philosophers[i].thread, philosopher_routine, &table->philosophers[i], CREATE);
        i++;
    }
    thread_handler(&table->monitor, monitor_routine, table, CREATE);
    i = 0;
    while (i < table->philo_count)
	{
        thread_handler(&table->philosophers[i].thread, NULL, NULL, JOIN);
        i++;
    }
    thread_handler(&table->monitor, NULL, NULL, JOIN);
	return (0);
}

/**
 * @brief Frees the resources allocated for the dining philosophers simulation.
 *
 * This function performs the following steps:
 * 1. Destroys the mutexes associated with each philosopher.
 * 2. Destroys the mutexes associated with each fork.
 * 3. Destroys the table mutex and write mutex.
 * 4. Detaches the monitor thread.
 * 5. Detaches each philosopher's thread.
 * 6. Frees the memory allocated for philosophers, forks, and the table itself.
 *
 * @param table Pointer to the table structure containing all resources.
 */
void free_table(t_table *table)
{
	int i;

	i = -1;
	while (++i < table->philo_count)
		mutex_handler(&table->philosophers[i].philo_mutex, DESTROY);
	i = -1;
	while (++i < table->philo_count)
		mutex_handler(&table->forks[i].fork_mutex, DESTROY);
	pthread_mutex_destroy(&table->table_mutex);
	pthread_mutex_destroy(&table->write_mutex);
	thread_handler(&table->monitor, NULL, NULL, DETACH);
	i = -1;
	while (++i < table->philo_count)
		thread_handler(&table->philosophers[i].thread, NULL, NULL, DETACH);
	free(table->philosophers);
	free(table->forks);
	free(table);
}

/**
 * @file 01-philosophers.c
 * @brief Entry point for the philosophers simulation program.
 *
 * This file contains the main function which initializes the simulation
 * environment and starts the philosophers simulation.
 *
 * Usage:
 * ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_meals]
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int Returns 0 on successful execution, 1 on error.
 */
int main(int argc, char **argv)
{
    t_table *table;
	
	// printf("time: %ld\n", get_time());
    if (check_args(argc, argv))
	{
        printf("Error: invalid arguments\n");
        printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_meals]\n");
        return (1);
    }
    table = init_table(argc, argv);
    if (!table)
	{
        return (1);
	}
	if (start_simulation(table))
	{
		printf("Error: simulation failed\n");
		return (1);
	}
    // free_table(table);
    return (0);
}
