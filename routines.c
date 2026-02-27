/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 11:53:36 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/27 20:26:50 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	*burning(t_coder *coder)
{
	set_sim_state(coder->shared_env, 0);
	printf("%lld %i burned out\n", get_time_now()
		- coder->shared_env->start, coder->id);
	pthread_cond_broadcast(&coder->shared_env->cond_free);
	pthread_cond_broadcast(&coder->shared_env->cond_priority);
	return (NULL);
}

void	*coder_routine(void *coder)
{
	t_coder		*new_coder;

	new_coder = (t_coder *)coder;
	while (get_sim_state(new_coder->shared_env) != 1)
		usleep(100);
	if (get_sim_state(new_coder->shared_env) == 2)
		return (NULL);
	while ((get_sim_state(new_coder->shared_env) == 1)
		&& get_count_comp(new_coder)
		< new_coder->shared_env->number_of_compiles_required)
	{
		if (taking_dongles(new_coder) == 1)
		{
			set_request(new_coder, 0);
			insert_priority(new_coder);
			start_compile(new_coder);
			start_debugging(new_coder);
			start_refactoring(new_coder);
		}
	}
	return (NULL);
}

void	*monitor_routine(void *coders)
{
	t_coder		*new_coders;
	long long	limit;
	int			i;
	
	new_coders = (t_coder *)coders;
	while (get_sim_state(new_coders[0].shared_env) != 1)
		usleep(100);
	if (get_sim_state(new_coders[0].shared_env) == 2)
		return (NULL);
	limit = new_coders[0].shared_env->time_to_burnout;
	while (get_sim_state(new_coders[0].shared_env) == 1
		&& !get_finish_stats(new_coders, new_coders[0].shared_env->nb_cod))
	{
		i = 0;
		while (i < new_coders[0].shared_env->nb_cod)
		{
			if ((get_time_now() - get_comp_time(&new_coders[i])) >= limit
				&& !get_finish(&new_coders[i]))
				return (burning(&new_coders[i]));
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
