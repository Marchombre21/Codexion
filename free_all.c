/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 14:18:09 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/27 13:53:59 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	final(t_shared_env *shared_env, t_coder *coders)
{
	int	i;

	i = shared_env->simulation_state;
	free_all((void *[]){shared_env->dongles, coders, shared_env}, 3,
		shared_env->nb_cod - 1, 1);
	if (i)
		printf("All compilations are completed, nobody died.\n");
}

static void	destroy(t_shared_env *new_env, t_coder *new_coders, int end)
{
	int	i;

	i = 0;
	while (i < new_env->nb_cod)
	{
		pthread_cond_destroy(&new_coders[i].cond_available);
		pthread_mutex_destroy(&new_coders[i].lock_coder_time);
		pthread_mutex_destroy(&new_coders[i].lock_coder_finish);
		pthread_mutex_destroy(&new_coders[i].lock_coder_request);
		pthread_mutex_destroy(&new_env->dongles[i++].lock);
	}
	pthread_mutex_destroy(&new_env->lock_coder_data);
	pthread_mutex_destroy(&new_env->lock_sim_state);
	pthread_cond_destroy(&new_env->cond_free);
	pthread_cond_destroy(&new_env->cond_priority);
	if (end != 3)
	{
		free(new_env->threads);
		free(new_env->monitor);
	}
}

void	*free_all(void *ptr[], int number, int nb_priority_array, int end)
{
	int				i;
	int				j;
	t_shared_env	*new_env;
	t_coder			*new_coders;

	i = 0;
	j = 0;
	if (end == 1 || end == 2 || end == 3)
	{
		new_env = (t_shared_env *)ptr[number - 1];
		new_coders = (t_coder *)ptr[number - 2];
		set_sim_state(new_env, 0);
		if (end == 2)
			pthread_join(*new_env->monitor, NULL);
		destroy(new_env, new_coders, end);
	}
	while (j <= nb_priority_array)
		free(((t_dongle *)ptr[i])[j++].priority);
	while (i < number)
		free(ptr[i++]);
	return (NULL);
}
