/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06-simulation_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfleitas <bfleitas@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 09:25:58 by bfleitas          #+#    #+#             */
/*   Updated: 2024/10/01 09:40:44 by bfleitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Checks if the simulation has finished.
 *
 * This function checks the end_flag of the simulation table to determine if the
 * simulation has finished. It uses a mutex to ensure thread-safe access to the
 * end_flag variable.
 *
 * @param table A pointer to the simulation table structure.
 * @return An integer value indicating whether the simulation has finished.
 */
int simulation_finished(t_table *table)
{
    return (get_int_value(&table->table_mutex, &table->end_flag));
}

/**
 * @brief Get the current time in milliseconds.
 *
 * This function retrieves the current time using the `gettimeofday` function
 * and returns it in milliseconds.
 *
 * @return The current time in milliseconds.
 */
long get_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/**
 * precise_usleep - Sleeps for a precise amount of time in milliseconds.
 * 
 * This function uses a combination of gettimeofday and usleep to achieve
 * a more accurate sleep duration compared to using usleep alone. It also
 * checks periodically if the simulation has finished and breaks out of the
 * sleep loop if it has.
 * 
 * @param time: The amount of time to sleep in milliseconds.
 * @param table: A pointer to the simulation table structure, used to check
 *               if the simulation has finished.
 */
void precise_usleep(long time, t_table *table)
{
    long start_time;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    while (get_time() - start_time < time)
    {
        if (simulation_finished(table))
            break;
        usleep(50);
    }
}

/**
 * @brief Writes the status of a philosopher to the console.
 *
 * This function prints the current status of a philosopher (eating, 
 * sleeping, thinking, or died) along with the time elapsed since the start 
 * of the simulation. It ensures thread-safe access to the console output 
 * by using a mutex.
 *
 * @param philosopher A pointer to the philosopher whose status is being written
 * @param state The current state of the philosopher (e.g., EATING, SLEEPING, 
 * THINKING, DIED).
 * @param table A pointer to the table structure containing simulation data 
 * and synchronization primitives.
 */
void write_status(t_philosopher *philosopher, e_philo_state state, t_table *table)
{
    long time;

    time = get_time() - table->start_time;
    mutex_handler(&table->write_mutex, LOCK);
    if (state == DIED)
        printf("%ld %d %s\n", time, philosopher->id + 1, "died");
    else if ((state == EATING) && !simulation_finished(table))
        printf("%ld %d %s\n", time, philosopher->id + 1, "is eating");
    else if ((state == SLEEPING) && !simulation_finished(table))
        printf("%ld %d %s\n", time, philosopher->id + 1, "is sleeping");
    else if ((state == THINKING) && !simulation_finished(table))
        printf("%ld %d %s\n", time, philosopher->id + 1, "is thinking");
    else if ((state == TAKE_LEFT_FORK) && !simulation_finished(table))
        printf("%ld %d %s\n", time, philosopher->id + 1, "has taken left fork");   //DEBUGGING
    else if ((state == TAKE_RIGHT_FORK) && !simulation_finished(table))            //DEBUGGING
        printf("%ld %d %s\n", time, philosopher->id + 1, "has taken right fork");  //DEBUGGING
    mutex_handler(&table->write_mutex, UNLOCK);                                    //DEBUGGING
}
