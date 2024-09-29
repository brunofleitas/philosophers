/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03-mutex_thread_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:13:47 by bruno             #+#    #+#             */
/*   Updated: 2024/09/29 22:06:36 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void thread_handler(pthread_t *thread, void *(*foo)(void *), void *data, e_thread_code code)
{
    if (code == CREATE)
        handle_thread_error(pthread_create(thread, NULL, foo, data), code);
    else if (code == JOIN)
        handle_thread_error(pthread_join(*thread, NULL), code);
    else if (code == DETACH)
        handle_thread_error(pthread_detach(*thread), code);
}