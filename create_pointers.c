/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pointers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:44:29 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/20 15:28:34 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_dongles(t_shared_env *shared_env)
{
	int			*array_priority;
	int			i;
	
	i = 0;
	shared_env->dongles = malloc(sizeof(t_dongle) * shared_env->num_cod);
	if (!shared_env->dongles)
		return free_all((void*[]){shared_env}, 1);
	array_priority = malloc(sizeof(int) * shared_env->num_cod);
	if (!array_priority)
		return free_all((void*[]){shared_env->dongles, shared_env}, 2);
	while (i < shared_env->num_cod)
	{
		pthread_mutex_init(&shared_env->dongles[i].lock, NULL);
		shared_env->dongles[i].available = 0;
		shared_env->dongles[i].priority = array_priority;
		i++;
	}
}

void	*create_coders(t_shared_env *shared_env)
{
	t_coder	*coders;
	int		i;

	i = 0;
	coders = malloc(sizeof(t_coder) * shared_env->num_cod);
	if (!coders)
		return free_all((void*[]){shared_env->dongles->priority,
						shared_env->dongles, shared_env}, 3);
	while (i < shared_env->num_cod)
	{
		coders->last_comp_time = 0;
		coders->id = i + 1;
		coders->lock_dongle_1 = &shared_env->dongles[i].lock;
		coders->lock_dongle_2 = &shared_env->dongles[i - 1].lock;
		coders->last_comp_time = 0;
		coders->last_comp_time = 0;
		
	}
}
