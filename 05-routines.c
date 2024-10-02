/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05-routines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfleitas <bfleitas@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 20:01:07 by bruno             #+#    #+#             */
/*   Updated: 2024/10/02 12:12:36 by bfleitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Routine executed by each philosopher thread.
 *
 * This function represents the life cycle of a philosopher in the dining
 * philosophers problem. Each philosopher will think, take forks, eat, and
 * sleep in a loop until the end condition is met.
 *
 * @param arg Pointer to the philosopher structure (t_philosopher).
 * @return NULL when the routine ends.
 *
 * The routine performs the following steps in a loop:
 * 1. Thinks and writes the THINKING status.
 * 2. Takes the left fork and writes the TAKE_LEFT_FORK status.
 * 3. Takes the right fork and writes the TAKE_RIGHT_FORK status.
 * 4. Eats and writes the EATING status.
 * 5. Updates the last meal time and increments the meal count.
 * 6. Releases the left and right forks.
 * 7. Sleeps and writes the SLEEPING status.
 *
 * The loop continues until the end_flag is set in the table structure.
 */
void* philosopher_routine(void* arg)
{
    t_philosopher	*philo;
	t_table			*table;
    
	philo = (t_philosopher *)arg;
	table = philo->table;
    // if (philo->id % 2 == 0)
    //     precise_usleep(100, table);
    while (!get_int_value(&table->table_mutex, &table->end_flag ))
	{
        write_status(philo, THINKING, table);
        // usleep(1000);
        mutex_handler(&philo->left_fork->fork_mutex, LOCK);  //DEBUGGING
        write_status(philo, TAKE_LEFT_FORK, table);          //DEBUGGING
        mutex_handler(&philo->right_fork->fork_mutex, LOCK); //DEBUGGING
        write_status(philo, TAKE_RIGHT_FORK, table);         //DEBUGGING
        write_status(philo, EATING, table);
        mutex_handler(&table->table_mutex, LOCK);
        philo->last_meal = get_time();
        mutex_handler(&table->table_mutex, UNLOCK);
        precise_usleep(table->time_to_eat, table);
        if (table->meal_count != -1)
        {
            philo->meal_count++;
            if (philo->meal_count >= table->meal_count)
                philo->is_full = 1;
        }
        mutex_handler(&philo->left_fork->fork_mutex, UNLOCK);
        mutex_handler(&philo->right_fork->fork_mutex, UNLOCK);
        write_status(philo, SLEEPING, table);
        precise_usleep(table->time_to_sleep, table);
    }
    return (NULL);
}

/**
 * @brief Monitor routine to check the status of philosophers.
 *
 * This function continuously monitors the status of each philosopher in the 
 * table. It checks if any philosopher has exceeded the allowed time without 
 * eating (time_to_die). If a philosopher has died, it updates the end_flag 
 * and writes the status.
 *
 * @param arg Pointer to the table structure (t_table).
 * @return NULL when the monitoring ends, either due to a philosopher's death 
 * or the end_flag being set.
 */
void* monitor_routine(void* arg)
{
    t_table *table;
    int i;
    long time_since_last_meal;

    table = (t_table *)arg;
    i = 0;
    while (!get_int_value(&table->table_mutex, &table->end_flag))
    {
        i = 0;
        while (i < table->philo_count)
        {
            mutex_handler(&table->table_mutex, LOCK);
            time_since_last_meal = get_time() - 
                                   table->philosophers[i].last_meal;
            if (time_since_last_meal >= table->time_to_die)
            {
                write_status(&table->philosophers[i], DIED, table);
                set_int_value(&table->set_value, &table->end_flag, 1);
                mutex_handler(&table->table_mutex, UNLOCK);
                return (0);
            }
            mutex_handler(&table->table_mutex, UNLOCK);
            i++;
        }
        precise_usleep(1000, table);
    }
    return (NULL);
}