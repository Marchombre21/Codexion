/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 11:34:13 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/23 08:26:14 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <limits.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct	s_coder
{
	long long		last_comp_time;
	int				id;
	pthread_t		thread_id;
	pthread_mutex_t	*lock_dongle_1;
	pthread_mutex_t	*lock_dongle_2;
	int				count_compile;
	t_shared_env	*shared_env;
	pthread_mutex_t	lock_coder_data;
	pthread_cond_t	*cond_priority;
	pthread_cond_t	*cond_available;
	pthread_cond_t	*cond_free;
}	t_coder;

typedef struct	s_dongle
{
	pthread_mutex_t	lock;
	int				free;
	long long		released_at;
	int				*priority;
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
	char			*scheduler;
	t_dongle		*dongles;
	pthread_mutex_t	lock_output;
	pthread_mutex_t	lock_sim_state;
	int				simulation_state;
	
}	t_shared_env;

void	*parsing(int argc, char **args);
void	*free_all(void *ptr[], int number);
void	*create_coders(t_shared_env *shared_env);
void	create_dongles(t_shared_env *shared_env);

#endif