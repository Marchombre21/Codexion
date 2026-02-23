/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pointers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:44:29 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/23 08:26:22 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_dongles(t_shared_env *shared_env)
{
	int				*array_priority;
	struct timeval	now;
	int				i;
	
	i = 0;
	shared_env->dongles = malloc(sizeof(t_dongle) * shared_env->nb_cod);
	if (!shared_env->dongles)
		return free_all((void*[]){shared_env}, 1);
	array_priority = malloc(sizeof(int) * shared_env->nb_cod);
	if (!array_priority)
		return free_all((void*[]){shared_env->dongles, shared_env}, 2);
	gettimeofday(&now, NULL);
	while (i < shared_env->nb_cod)
	{
		pthread_mutex_init(&shared_env->dongles[i].lock, NULL);
		shared_env->dongles[i].free = 1;
		shared_env->dongles[i].released_at = (now.tv_sec * 1000LL) +
							(now.tv_usec / 1000) + shared_env->dongle_cooldown;
		shared_env->dongles[i].priority = array_priority;
		i++;
	}
}

void	*create_coders(t_shared_env *shared_env)
{
	t_coder	*coders;
	int		i;
	int		nb_max;

	i = 0;
	nb_max = shared_env->nb_cod - 1;
	coders = malloc(sizeof(t_coder) * shared_env->nb_cod);
	if (!coders)
		return free_all((void*[]){shared_env->dongles->priority,
						shared_env->dongles, shared_env}, 3);
	while (i < shared_env->nb_cod)
	{
		coders->last_comp_time = 0;
		coders->id = i + 1;
		coders->lock_dongle_1 = &shared_env->dongles[i].lock;
		if (i > 0)
			coders->lock_dongle_2 = &shared_env->dongles[i - 1].lock;
		else
			coders->lock_dongle_2 = &shared_env->dongles[nb_max].lock;
		coders->count_compile = 0;
		pthread_mutex_init(&coders->lock_coder_data, NULL);
		pthread_cond_init(coders[i].cond_priority, NULL);
		pthread_cond_init(coders[i].cond_available, NULL);
		pthread_cond_init(coders[i].cond_free, NULL);
	}
}
