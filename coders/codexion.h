/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 11:34:13 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/25 08:47:27 by bfitte           ###   ########lyon.fr   */
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

typedef struct s_coder t_coder;

typedef struct	s_priority
{
	t_coder	*order[2];
	char	*scheduler;
}	t_priority;

typedef struct	s_dongle
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
	long long		dongle_cooldown;
	long long		start;
	char			*scheduler;
	pthread_t		*threads;
	t_dongle		*dongles;
	pthread_mutex_t	lock_output;
	pthread_mutex_t	lock_sim_state;
	int				simulation_state;
	
}	t_shared_env;

typedef struct	s_coder
{
	long long		last_comp_time;
	long long		request_time;
	int				id;
	t_dongle		*dongles[2];
	int				count_compile;
	t_shared_env	*shared_env;
	pthread_mutex_t	*lock_coder_data;
	pthread_cond_t	*cond_priority;
	pthread_cond_t	cond_available;
	pthread_cond_t	*cond_free;
}	t_coder;

void		*parsing(int argc, char **args);
void		*free_all(void *ptr[], int number, int nb_priority_array);
void		*create_coders(t_shared_env *shared_env);
void		unlock_dongles(t_coder *coder);
void		initialize_priority(t_dongle *dongles, t_coder *coders);
void		display_message(t_coder *coder, char *message, int number);
void		get_end_cooldown(long long waited_time, struct timespec *ts);
long long	get_time_now();
int			create_threads(t_shared_env *shared_env, t_coder *coders);
void		stop_threads(pthread_t *threads, t_coder *coder);
long long	checking_available(t_coder *coder, long long cooldown);
void		check_available(long long available, t_coder *coder);
void		insert_priority(t_coder *coder);
void		*taking_dongles(void *coder);
void		start_compile(t_coder *coder);
void		*create_dongles(t_shared_env *shared_env);

#endif