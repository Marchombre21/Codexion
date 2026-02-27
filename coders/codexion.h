/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 11:34:13 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/27 20:23:19 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <limits.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_coder	t_coder;

typedef struct s_priority
{
	t_coder	*order[2];
	char	*scheduler;
}	t_priority;

typedef struct s_dongle
{
	pthread_mutex_t	lock;
	int				free;
	long long		released_at;
	t_priority		*priority;
}	t_dongle;

typedef struct s_shared_env
{
	int				nb_cod;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	long long		dongle_cd;
	long long		start;
	char			*scheduler;
	pthread_cond_t	cond_priority;
	pthread_cond_t	cond_free;
	pthread_mutex_t	lock_coder_data;
	pthread_t		*monitor;
	pthread_t		*threads;
	t_dongle		*dongles;
	pthread_mutex_t	lock_sim_state;
	int				simulation_state;
}	t_shared_env;

typedef struct s_coder
{
	long long		last_comp_time;
	long long		request_time;
	int				id;
	int				finish;
	t_dongle		*dongles[2];
	int				count_compile;
	t_shared_env	*shared_env;
	pthread_mutex_t	*lock_coder_data;
	pthread_mutex_t	lock_coder_finish;
	pthread_mutex_t	lock_coder_time;
	pthread_mutex_t	lock_coder_count;
	pthread_mutex_t	lock_coder_request;
	pthread_cond_t	*cond_priority;
	pthread_cond_t	cond_available;
	pthread_cond_t	*cond_free;
}	t_coder;

// Utils
void		display_message(t_coder *coder, char *message, int number);
int			stop_taking_dongles(t_coder *coder);
void		get_end_cooldown(long long waited_time, struct timespec *ts);
void		unlock_dongles(t_coder *coder);
// Routines
void		*coder_routine(void *coder);
void		*monitor_routine(void *coders);
// Priority
void		edf(t_coder *coder, int i, t_coder *temp);
void		fifo(t_coder *coder, int i, t_coder *temp);
int			check_priority(t_coder *coder);
// Parsing
void		*parsing(int argc, char **args);
// Get_set
int			get_count_comp(t_coder *coder);
void		set_count_comp(t_coder *coder);
long long	get_time_now(void);
long long	get_request(t_coder *coder);
void		set_sim_state(t_shared_env *shared_env, int i);
int			get_sim_state(t_shared_env *shared_env);
int			get_finish_stats(t_coder *coders, int nb);
void		set_finish(t_coder *coder);
int			get_finish(t_coder *coder);
void		set_request(t_coder *coder, long long time);
long long	get_comp_time(t_coder *coder);
void		set_comp_time(t_coder *coder, long long comp_time);
// Free
void		*free_all(void *ptr[], int number, int nb_priority_array, int end);
void		final(t_shared_env *shared_env, t_coder *coders);
// Create_utils
int			init_threads(t_shared_env *shared_env, t_coder *coders, int *i);
void		init_coders_stats(t_shared_env *shared_env, t_coder *coders, int i);
int			error_create_thread(t_shared_env *shared_env, t_coder *coders, int i);
void		init_dongles_priority(t_shared_env *shared_env, t_coder *coders,
				int i, int nb_max);
// Create_pointers
void		*create_coders(t_shared_env *shared_env);
int			create_threads(t_shared_env *shared_env, t_coder *coders);
int			create_monitor(t_shared_env *shared_env, t_coder *coders);
void		*create_dongles(t_shared_env *shared_env);
// Actions
void		start_refactoring(t_coder *coder);
void		start_debugging(t_coder *coder);
void		start_compile(t_coder *coder);
int			taking_dongles(t_coder *coder);
// Actions_utils
void		just_one(t_coder *coder);
long long	check_availability(t_coder *coder, long long cooldown);
void		check_res_available(long long available, t_coder *coder);
void		insert_priority(t_coder *coder);

#endif