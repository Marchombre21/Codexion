/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creates_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 13:29:59 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/27 08:41:31 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_dongles_priority(t_shared_env *shared_env, t_coder *coders, int i,
								int nb_max)
{
	if (i % 2 == 0)
	{
		coders[i].dongles[0] = &shared_env->dongles[i];
		if (i == 0)
			coders[i].dongles[1] = &shared_env->dongles[nb_max];
		else
			coders[i].dongles[1] = &shared_env->dongles[i - 1];
		coders[i].dongles[0]->priority->order[0] = &coders[i];
		coders[i].dongles[1]->priority->order[1] = &coders[i];
	}
	else
	{
		coders[i].dongles[0] = &shared_env->dongles[i - 1];
		coders[i].dongles[1] = &shared_env->dongles[i];
		coders[i].dongles[0]->priority->order[1] = &coders[i];
		coders[i].dongles[1]->priority->order[0] = &coders[i];
	}
}

void	init_coders_stats(t_shared_env *shared_env, t_coder *coders, int i)
{
	coders[i].shared_env = shared_env;
	coders[i].last_comp_time = get_time_now();
	coders[i].id = i + 1;
	coders[i].finish = 0;
	coders[i].count_compile = 0;
	coders[i].request_time = 0;
	coders[i].cond_free = &shared_env->cond_free;
	coders[i].cond_priority = &shared_env->cond_priority;
	coders[i].lock_coder_data = &shared_env->lock_coder_data;
}

int	init_threads(t_shared_env *shared_env, t_coder *coders, int i)
{
	shared_env->threads = malloc(sizeof(pthread_t) * shared_env->nb_cod);
	if (!shared_env->threads)
	{
		free_all((void *[]){shared_env->dongles, coders, shared_env},
			3, shared_env->nb_cod - 1, 2);
		return (0);
	}
	while (i < shared_env->nb_cod)
	{
		if (pthread_create(&shared_env->threads[i], NULL, coder_routine,
				&coders[i]) != 0)
			return (error_create_thread(shared_env, i));
		i++;
	}
	return (1);
}

int	error_create_thread(t_shared_env *shared_env, int i)
{
	int	j;

	j = 0;
	set_sim_state(shared_env, 2);
	while (j < i)
		if (pthread_join(shared_env->threads[j++], NULL) != 0)
			write(2, "An error occured with pthread_join.\n", 38);
	return (1);
}
