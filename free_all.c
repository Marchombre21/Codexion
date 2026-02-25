/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 14:18:09 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/25 11:42:53 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	destroy(t_shared_env *new_env, t_coder *new_coders)
{
	int	i;

	i = 0;
	while (i < new_env->nb_cod)
		{
			pthread_cond_destroy(&new_coders[i].cond_available);
			pthread_mutex_destroy(&new_env->dongles[i++].lock);
		}
		pthread_mutex_destroy(&new_env->lock_coder_data);
		pthread_cond_destroy(&new_env->cond_free);
		pthread_cond_destroy(&new_env->cond_priority);
		free(new_env->threads);
}

void	*free_all(void *ptr[], int number, int nb_priority_array, int end)
{
	int				i;
	int				j;
	t_shared_env	*new_env;
	t_coder			*new_coders;

	i = 0;
	j = 0;
	if (end)
	{
		new_env = (t_shared_env *)ptr[number - 1];
		new_coders = (t_coder *)ptr[number - 2];
		destroy(new_env, new_coders);
	}
	while (j < nb_priority_array)
		free(((t_dongle *)ptr[i])[j++].priority);
	while (i < number)
		free(ptr[i++]);
	return NULL;
}
