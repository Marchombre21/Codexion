/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 11:34:13 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/20 14:16:15 by bfitte           ###   ########lyon.fr   */
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
	pthread_mutex_t	lock_dongle;
	pthread_cond_t	cond;
	int				count_compile;
	int				second_index_dongle;
	t_shared_env	*shared_env;
	pthread_mutex_t	lock_coder_data;
}	t_coder;

typedef struct	s_dongle
{
	int		free;
	long long		released_at;
	int		*priority;
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

#endif