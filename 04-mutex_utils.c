/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04-mutex_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:13:51 by bruno             #+#    #+#             */
/*   Updated: 2024/09/29 22:09:46 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void set_long_value(t_mtx *mutex, long *dest, long value)
{
    mutex_handler(mutex, LOCK);
    *dest = value;
    mutex_handler(mutex, UNLOCK);
}

long get_long_value(t_mtx *mutex, long *dest)
{
    long value;

    mutex_handler(mutex, LOCK);
    value = *dest;
    mutex_handler(mutex, UNLOCK);
    return (value);
}

void set_int_value(t_mtx *mutex, int *dest, int value)
{
    mutex_handler(mutex, LOCK);
    *dest = value;
    mutex_handler(mutex, UNLOCK);
}

int get_int_value(t_mtx *mutex, int *dest)
{
    int value;

    mutex_handler(mutex, LOCK);
    value = *dest;
    mutex_handler(mutex, UNLOCK);
    return (value);
}

int simulation_finished(t_table *table)
{
    return (get_int_value(&table->table_mutex, &table->end_flag));
}

long get_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

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
        usleep(100);
    }
}

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
    // else if ((state == TAKE_LEFT_FORK) && !simulation_finished(table))
    //     printf("%ld %d %s\n", time, philosopher->id + 1, "has taken a left fork");
    // else if ((state == TAKE_RIGHT_FORK) && !simulation_finished(table))
    //     printf("%ld %d %s\n", time, philosopher->id + 1, "has taken a right fork");
    mutex_handler(&table->write_mutex, UNLOCK);
}