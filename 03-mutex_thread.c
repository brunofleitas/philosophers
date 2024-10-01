/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03-mutex_thread.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfleitas <bfleitas@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:13:47 by bruno             #+#    #+#             */
/*   Updated: 2024/10/01 10:06:59 by bfleitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Handles errors related to mutex operations.
 *
 * This function checks the status of a mutex operation and prints an 
 * appropriate error message based on the type of operation that failed.
 *
 * @param status The status code returned by the mutex operation. A non-zero 
 * value indicates an error.
 * @param code The type of mutex operation that was attempted. This can be one 
 * of the following:
 *             - LOCK: Indicates a mutex lock operation.
 *             - UNLOCK: Indicates a mutex unlock operation.
 *             - INIT: Indicates a mutex initialization operation.
 *             - DESTROY: Indicates a mutex destruction operation.
 */
void handle_mutex_error(int status, e_mutex_code code)
{
    if (status != 0)
    {
        if (code == LOCK)
            printf("Error: mutex lock failed\n");
        else if (code == UNLOCK)
            printf("Error: mutex unlock failed\n");
        else if (code == INIT)
            printf("Error: mutex initialization failed\n");
        else if (code == DESTROY)
            printf("Error: mutex destruction failed\n");
    }
}

/**
 * @brief Utility functions for handling mutex operations.
 *
 * This file contains the implementation of the mutex_handler function,
 * which provides a unified interface for various mutex operations such as
 * locking, unlocking, initializing, and destroying a mutex.
 *
 * @param mutex A pointer to the pthread_mutex_t object to be manipulated.
 * @param code An enumeration value indicating the mutex operation to perform.
 *             - LOCK: Lock the mutex.
 *             - UNLOCK: Unlock the mutex.
 *             - INIT: Initialize the mutex.
 *             - DESTROY: Destroy the mutex.
 *
 * The function calls handle_mutex_error to handle any errors that occur
 * during the mutex operations.
 */
void mutex_handler(pthread_mutex_t *mutex, e_mutex_code code)
{
    if (code == LOCK)
        handle_mutex_error(pthread_mutex_lock(mutex), code);
    else if (code == UNLOCK)
        handle_mutex_error(pthread_mutex_unlock(mutex), code);
    else if (code == INIT)
        handle_mutex_error(pthread_mutex_init(mutex, NULL), code);
    else if (code == DESTROY)
        handle_mutex_error(pthread_mutex_destroy(mutex), code);
}

/**
 * @brief Handles thread-related errors based on the status and error code.
 *
 * This function checks the status of a thread operation and prints an
 * appropriate error message if the operation failed. The type of operation
 * is determined by the `code` parameter.
 *
 * @param status The status code returned by the thread operation.
 * @param code The type of thread operation that was attempted (CREATE, JOIN, DETACH).
 */
void handle_thread_error(int status, e_thread_code code)
{
    if (status != 0)
    {
        if (code == CREATE)
            printf("Error: thread creation failed\n");
        else if (code == JOIN)
            printf("Error: thread join failed\n");
        else if (code == DETACH)
            printf("Error: thread detach failed\n");
    }
}

/**
 * @brief Utility functions for handling threads with mutex in the philosophers 
 * project.
 *
 * This file contains the implementation of the thread_handler function, which
 * provides a unified interface for creating, joining, and detaching threads.
 * 
 * @param thread Pointer to the pthread_t structure representing the thread.
 * @param foo Function pointer to the thread's start routine.
 * @param data Pointer to the data to be passed to the thread's start routine.
 * @param code Enum value indicating the thread operation (CREATE, JOIN, DETACH).
 *
 * The function handles errors for each thread operation by calling the
 * handle_thread_error function with the appropriate error code.
 */
void thread_handler(pthread_t *thread, void *(*foo)(void *), void *data, e_thread_code code)
{
    if (code == CREATE)
        handle_thread_error(pthread_create(thread, NULL, foo, data), code);
    else if (code == JOIN)
        handle_thread_error(pthread_join(*thread, NULL), code);
    else if (code == DETACH)
        handle_thread_error(pthread_detach(*thread), code);
}