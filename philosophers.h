/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfleitas <bfleitas@student.42luxembourg    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:14:22 by bruno             #+#    #+#             */
/*   Updated: 2024/10/01 10:26:10 by bfleitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
} e_mutex_code;

typedef enum
{
	CREATE,
	JOIN,
	DETACH,
} e_thread_code;

typedef enum
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_LEFT_FORK,
	TAKE_RIGHT_FORK,
	DIED,
}	e_philo_state;

typedef struct s_table	t_table;

typedef struct s_fork
{
	pthread_mutex_t	fork_mutex;
	int				fork;
} t_fork;

typedef struct s_philosopher
{
	int				id;
	long			meal_count;
	int				is_full;
	int				last_meal;
	t_fork			*left_fork;
	t_fork			*right_fork;
	pthread_mutex_t	philo_mutex;
	pthread_t		thread;
	t_table			*table;
}					t_philosopher;

typedef struct s_table
{
	long			philo_count;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			meal_count;
	long 			start_time;
	int 			end_flag; //  1 = all are full or one died
	int 			all_threads_created;
	pthread_mutex_t	table_mutex;
	pthread_mutex_t	write_mutex;
	t_fork			*forks;
	pthread_t		monitor;
	t_philosopher	*philosophers;
}					t_table;

int		ft_atoi(const char *str);
int		check_args(int argc, char **argv);

t_table	*init_table(int argc, char **argv);
void	init_table_data(t_table *table, int argc, char **argv);
int		init_forks(t_table *table);
int		init_philosophers(t_table *table);
void	assign_forks(t_philosopher *philosopher, t_fork *left_fork, t_fork *right_fork, int id);

void	handle_mutex_error(int status, e_mutex_code code);
void	mutex_handler(pthread_mutex_t *mutex, e_mutex_code code);
void	handle_thread_error(int status, e_thread_code code);
void	thread_handler(pthread_t *thread, void *(*foo)(void *), void *data, e_thread_code code);

void	set_long_value(pthread_mutex_t *mutex, long *dest, long value);
long	get_long_value(pthread_mutex_t *mutex, long *dest);
void	set_int_value(pthread_mutex_t *mutex, int *dest, int value);
int		get_int_value(pthread_mutex_t *mutex, int *dest);

void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);

int 	simulation_finished(t_table *table);
long	get_time(void);
void	precise_usleep(long time, t_table *table);
void	write_status(t_philosopher *philosopher, e_philo_state state, t_table *table);

#endif
