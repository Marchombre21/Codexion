/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pointers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:44:29 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/28 16:37:00 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*create_dongles(t_shared_env *shared_env)
{
	t_priority		*array_priority;
	struct timeval	now;
	int				i;

	i = 0;
	shared_env->dongles = malloc(sizeof(t_dongle) * shared_env->nb_cod);
	if (!shared_env->dongles)
		return (free_all((void *[]){shared_env}, 1, 0, 0));
	gettimeofday(&now, NULL);
	while (i < shared_env->nb_cod)
	{
		array_priority = malloc(sizeof(t_priority));
		if (!array_priority)
			return (free_all((void *[]){shared_env->dongles, shared_env}, 2,
				i - 1, 0));
		pthread_mutex_init(&shared_env->dongles[i].lock, NULL);
		shared_env->dongles[i].free = 1;
		shared_env->dongles[i].released_at = (now.tv_sec * 1000LL)
			+ (now.tv_usec / 1000) - shared_env->dongle_cd;
		shared_env->dongles[i].priority = array_priority;
		shared_env->dongles[i].priority->scheduler = shared_env->scheduler;
		i++;
	}
	return (shared_env->dongles);
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
		return (free_all((void *[]){shared_env->dongles, shared_env}, 2,
			nb_max, 0));
	while (i < shared_env->nb_cod)
	{
		init_dongles_priority(shared_env, coders, i, nb_max);
		init_coders_stats(shared_env, coders, i);
		pthread_mutex_init(&coders[i].lock_coder_time, NULL);
		pthread_mutex_init(&coders[i].lock_coder_count, NULL);
		pthread_mutex_init(&coders[i].lock_coder_request, NULL);
		pthread_mutex_init(&coders[i].lock_coder_finish, NULL);
		pthread_cond_init(&coders[i].cond_available, NULL);
		i++;
	}
	return (coders);
}

int	create_threads(t_shared_env *shared_env, t_coder *coders)
{
	int	i;

	i = 0;
	if (init_threads(shared_env, coders, &i) == 1)
	{
		set_sim_state(shared_env, 1);
		i = 0;
		while (get_sim_state(shared_env) == 1 && i < shared_env->nb_cod)
		{
			if (pthread_join(shared_env->threads[i++], NULL) != 0)
			{
				set_sim_state(shared_env, 0);
				write(2, "An error occured with a pthread_join.\n", 38);
			}
		}
		if (pthread_join(*shared_env->monitor, NULL) != 0)
		{
			set_sim_state(shared_env, 0);
			write(2, "An error occured with a pthread_join.\n", 38);
		}
	}
	else
		return (error_create_thread(shared_env, coders, i));
	return (1);
}

int	create_monitor(t_shared_env *shared_env, t_coder *coders)
{
	shared_env->monitor = malloc(sizeof(pthread_t));
	if (!shared_env->monitor)
	{
		free_all((void *[]){shared_env->dongles, coders, shared_env},
			3, shared_env->nb_cod - 1, 1);
		return (0);
	}
	if (pthread_create(shared_env->monitor, NULL, monitor_routine, coders) != 0)
	{
		write(2, "The monitor can't be create.\n", 29);
		free_all((void *[]){shared_env->dongles, coders, shared_env},
			3, shared_env->nb_cod - 1, 1);
		return (0);
	}
	else
		return (1);
}
