/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 13:59:27 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/26 14:01:34 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_sim_state(t_shared_env *shared_env, int i)
{
	pthread_mutex_lock(&shared_env->lock_sim_state);
	shared_env->simulation_state = i;
	pthread_mutex_unlock(&shared_env->lock_sim_state);
}

int	get_sim_state(t_shared_env *shared_env)
{
	pthread_mutex_lock(&shared_env->lock_sim_state);
	if (shared_env->simulation_state == 1)
	{
		pthread_mutex_unlock(&shared_env->lock_sim_state);
		return (1);
	}
	else
	{
		pthread_mutex_unlock(&shared_env->lock_sim_state);
		return (0);
	}
}

void	set_comp_time(t_coder *coder, long long comp_time)
{
	pthread_mutex_lock(&coder->lock_coder_time);
	coder->last_comp_time = comp_time;
	pthread_mutex_unlock(&coder->lock_coder_time);
}

long long	get_comp_time(t_coder *coder)
{
	long long	time;

	pthread_mutex_lock(&coder->lock_coder_time);
	time = coder->last_comp_time;
	pthread_mutex_unlock(&coder->lock_coder_time);
	return time;
}

long long	get_time_now()
{
	struct timeval ts;

	gettimeofday(&ts, NULL);
	return (ts.tv_sec * 1000LL) + (ts.tv_usec / 1000);
}
