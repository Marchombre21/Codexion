/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 14:18:09 by bfitte            #+#    #+#             */
/*   Updated: 2026/03/04 07:38:53 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	final(t_shared_env *shared_env, t_coder *coders)
{
	int	i;

	i = shared_env->simulation_state;
	free_all((void *[]){shared_env->dongles, coders, shared_env}, 3,
		shared_env->nb_cod - 1, 1);
	if (i == 1)
		printf("All requested compilations are completed, nobody died.\n");
}

static void	destroy(t_shared_env *new_env, t_coder *new_coders)
{
	int	i;

	i = 0;
	while (i < new_env->nb_cod)
	{
		pthread_cond_destroy(&new_coders[i].cond_available);
		pthread_mutex_destroy(&new_coders[i].lock_coder_time);
		pthread_mutex_destroy(&new_coders[i].lock_coder_count);
		pthread_mutex_destroy(&new_coders[i].lock_coder_finish);
		pthread_mutex_destroy(&new_coders[i].lock_coder_request);
		pthread_mutex_destroy(&new_env->dongles[i].lock);
		pthread_mutex_destroy(&new_env->dongles[i].lock_priority);
		pthread_mutex_destroy(&new_env->dongles[i++].lock_free);
	}
	pthread_mutex_destroy(&new_env->lock_coder_data);
	pthread_mutex_destroy(&new_env->lock_sim_state);
	pthread_cond_destroy(&new_env->cond_free);
	pthread_cond_destroy(&new_env->cond_priority);
	free(new_env->threads);
	free(new_env->monitor);
}

void	*free_all(void *ptr[], int number, int nb_priority_array, int end)
{
	int				i;
	int				j;
	t_shared_env	*new_env;
	t_coder			*new_coders;

	i = 0;
	j = 0;
	if (end == 1 || end == 2)
	{
		new_env = (t_shared_env *)ptr[number - 1];
		new_coders = (t_coder *)ptr[number - 2];
		if (end == 1)
			set_sim_state(new_env, 0);
		if (end == 2)
			if (pthread_join(*new_env->monitor, NULL) != 0)
				write(2, "An error occured with pthread_join.\n", 38);
		destroy(new_env, new_coders);
	}
	while (j <= nb_priority_array)
		free(((t_dongle *)ptr[i])[j++].priority);
	while (i < number)
		free(ptr[i++]);
	return (NULL);
}
