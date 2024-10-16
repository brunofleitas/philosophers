/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04-set_values.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 09:19:29 by bfleitas          #+#    #+#             */
/*   Updated: 2024/10/16 13:51:43 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Sets a long value in a thread-safe manner.
 *
 * This function locks the provided mutex, sets the destination long variable
 * to the specified value, and then unlocks the mutex.
 *
 * @param mutex A pointer to the mutex to be used for locking and unlocking.
 * @param dest A pointer to the long variable where the value will be set.
 * @param value The long value to be set.
 */
void	set_long_value(pthread_mutex_t *mutex, long *dest, long value)
{
	mutex_handler(mutex, LOCK);
	*dest = value;
	mutex_handler(mutex, UNLOCK);
}

/**
 * @brief Retrieves a long integer value in a thread-safe manner.
 *
 * This function locks the provided mutex, copies the value from the
 * destination pointer, and then unlocks the mutex before returning the value.
 *
 * @param mutex A pointer to the mutex to be used for locking and unlocking.
 * @param dest A pointer to the long integer value to be retrieved.
 * @return The long integer value stored at the destination pointer.
 */
long	get_long_value(pthread_mutex_t *mutex, long *dest)
{
	long	value;

	mutex_handler(mutex, LOCK);
	value = *dest;
	mutex_handler(mutex, UNLOCK);
	return (value);
}

/**
 * @brief Sets an integer value in a thread-safe manner.
 *
 * This function locks the provided mutex, sets the destination integer
 * to the specified value, and then unlocks the mutex.
 *
 * @param mutex A pointer to the mutex to be used for locking and unlocking.
 * @param dest A pointer to the integer variable where the value will be set.
 * @param value The integer value to set.
 */
void	set_int_value(pthread_mutex_t *mutex, int *dest, int value)
{
	mutex_handler(mutex, LOCK);
	*dest = value;
	mutex_handler(mutex, UNLOCK);
}

/**
 * @brief Retrieves an integer value in a thread-safe manner.
 *
 * This function locks the provided mutex, copies the value from the
 * destination pointer, and then unlocks the mutex before returning the value.
 *
 * @param mutex A pointer to the mutex to be used for locking and unlocking.
 * @param dest A pointer to the integer value to be retrieved.
 * @return The integer value retrieved from the destination pointer.
 */
int	get_int_value(pthread_mutex_t *mutex, int *dest)
{
	int	value;

	mutex_handler(mutex, LOCK);
	value = *dest;
	mutex_handler(mutex, UNLOCK);
	return (value);
}
