/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05-simulation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfleitas <bfleitas@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 20:01:07 by bruno             #+#    #+#             */
/*   Updated: 2024/09/29 23:51:23 by bfleitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

#include "philosophers.h"

long long current_time_in_ms() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

void* philosopher_routine(void* arg) {
    t_philosopher *philo = (t_philosopher *)arg;
    t_table *table = philo->table;

    while (!get_int_value(&table->table_mutex, &table->end_flag)) {
        write_status(philo, THINKING, table);

        // Tomar tenedores
        mutex_handler(philo->left_fork->fork_mutex, LOCK);
        write_status(philo, TAKE_LEFT_FORK, table);
        mutex_handler(philo->right_fork->fork_mutex, LOCK);
        write_status(philo, TAKE_RIGHT_FORK, table);

        // Comer
        write_status(philo, EATING, table);
        mutex_handler(&table->table_mutex, LOCK);
        philo->last_meal = current_time_in_ms();
        mutex_handler(&table->table_mutex, UNLOCK);
        precise_usleep(table->time_to_eat, table);
        philo->meal_count++;

        // Soltar tenedores
        mutex_handler(philo->left_fork->fork_mutex, UNLOCK);
        mutex_handler(philo->right_fork->fork_mutex, UNLOCK);

        // Dormir
        write_status(philo, SLEEPING, table);
        precise_usleep(table->time_to_sleep, table);
    }
    return NULL;
}

void* monitor_routine(void* arg) {
    t_table *table = (t_table *)arg;
    int i = 0;

    while (!get_int_value(&table->table_mutex, &table->end_flag)) {
        i = 0;
        while (i < table->philo_count) {
            mutex_handler(&table->table_mutex, LOCK);
            long time_since_last_meal = current_time_in_ms() - table->philosophers[i].last_meal;
            if (time_since_last_meal >= table->time_to_die) {
                write_status(&table->philosophers[i], DIED, table);
                set_int_value(&table->table_mutex, &table->end_flag, 1);
                mutex_handler(&table->table_mutex, UNLOCK);
                return NULL;
            }
            mutex_handler(&table->table_mutex, UNLOCK);
            i++;
        }
        usleep(1000);  // Verificación cada 1ms
    }
    return NULL;
}
