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

typedef struct	s_coder
{
	long long		last_comp_time;
	int				id;
	pthread_t		thread_id;
	pthread_mutex_t	*lock_dongle;
	int				count_compile;
	void			*shared_env_struct;
	pthread_mutex_t	lock_coder_data;
}	t_coder;

typedef struct	s_dongle
{
	pthread_mutex_t	lock;
	long long		available;
	int				*priority;
}	t_dongle;

typedef struct s_shared_env
{
	int				numbers_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	char			*scheduler;
	void			*dongles_array;
	pthread_mutex_t	lock_output;
	pthread_mutex_t	lock_sim_state;
	int				simulation_state;
	
}	t_shared_env;

void	*parsing(int argc, char **args);

#endif