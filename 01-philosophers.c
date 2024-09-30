/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01-philosophers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfleitas <bfleitas@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:14:01 by bruno             #+#    #+#             */
/*   Updated: 2024/09/29 23:42:58 by bfleitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

/* int	main(int argc, char **argv)
{
	// t_philosopher	*philo;
	t_table			*table;

	if (check_args(argc, argv))
	{
		printf("Error: invalid arguments\n");\
		printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_meals]\n");
		return (1);
	}
	table = init_table(argc, argv);
	if (!table)
	{
		printf("Error: table initialization failed\n");
		return (1);
	}
	// if (start_simulation(philo))
	// {
	// 	printf("Error: simulation failed\n");
	// 	return (1);
	// }
	// destroy_table(table);
	return (0);
} */

int main(int argc, char **argv) {
    t_table *table;
    int i = 0;

    if (check_args(argc, argv)) {
        printf("Error: invalid arguments\n");
        printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_meals]\n");
        return (1);
    }

    table = init_table(argc, argv);
    if (!table)
        return (1);

    // Crear filósofos y hilos de monitor
    i = 0;
    while (i < table->philo_count) {
        thread_handler(&table->philosophers[i].thread, philosopher_routine, &table->philosophers[i], CREATE);
        i++;
    }
    thread_handler(table->monitor, monitor_routine, table, CREATE);

    // Esperar a que los hilos terminen
    i = 0;
    while (i < table->philo_count) {
        thread_handler(&table->philosophers[i].thread, NULL, NULL, JOIN);
        i++;
    }
    thread_handler(table->monitor, NULL, NULL, JOIN);

    // Liberar recursos
    // free_table(table);
    return (0);
}
