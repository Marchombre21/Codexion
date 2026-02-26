/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pointers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:44:29 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/26 11:02:07 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	*create_dongles(t_shared_env *shared_env)
{
	t_priority		*array_priority;
	struct timeval	now;
	int				i;
	
	i = 0;
	// shared_env->dongles = NULL;
	shared_env->dongles = malloc(sizeof(t_dongle) * shared_env->nb_cod);
	if (!shared_env->dongles)
		return free_all((void*[]){shared_env}, 1, 0, 0);
	gettimeofday(&now, NULL);
	while (i < shared_env->nb_cod)
	{
		// array_priority = NULL;
		array_priority = malloc(sizeof(t_priority));
		if (!array_priority)
			//Vérifier le i - 1
			return free_all((void*[]){shared_env->dongles, shared_env}, 2, i - 1, 0);
		//Not sure if it does what I want
		memset(array_priority->order, 0, 2);
		pthread_mutex_init(&shared_env->dongles[i].lock, NULL);
		shared_env->dongles[i].free = 1;
		shared_env->dongles[i].released_at = (now.tv_sec * 1000LL) +
							(now.tv_usec / 1000) - shared_env->dongle_cooldown;
		shared_env->dongles[i].priority = array_priority;
		shared_env->dongles[i].priority->scheduler = shared_env->scheduler;
		i++;
	}
	return shared_env->dongles;
}

void	*create_coders(t_shared_env *shared_env)
{
	t_coder	*coders;
	int		i;
	int		nb_max;

	i = 0;
	nb_max = shared_env->nb_cod - 1;
	// coders = NULL;
	coders = malloc(sizeof(t_coder) * shared_env->nb_cod);
	if (!coders)
		return free_all((void*[]){shared_env->dongles, shared_env}, 2, nb_max, 0);
	while (i < shared_env->nb_cod)
	{
		coders[i].shared_env = shared_env;
		coders[i].last_comp_time = get_time_now();
		coders[i].id = i + 1;
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
		coders[i].count_compile = 0;
		coders[i].request_time = 0;
		coders[i].cond_free = &shared_env->cond_free;
		coders[i].cond_priority = &shared_env->cond_priority;
		coders[i].lock_coder_data = &shared_env->lock_coder_data;
		pthread_mutex_init(&coders[i].lock_coder_time, NULL);
		pthread_cond_init(&coders[i].cond_available, NULL);
		i++;
	}
	return coders;
}

int	create_threads(t_shared_env *shared_env, t_coder *coders)
{
	int	i;
	
	i = 0;
	// shared_env->threads = NULL;
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
			return error_create_thread(shared_env, i);
		i++;
	}
	i = 0;
	if (pthread_join(*shared_env->monitor, NULL) != 0)
	{
		set_sim_state(shared_env, 0);
		write(2, "An error occured with a pthread_join.\n", 38);
	}
	while (get_sim_state(shared_env) != 2 && i < shared_env->nb_cod)
			if (pthread_join(shared_env->threads[i++], NULL) != 0)
			{
				set_sim_state(shared_env, 0);
				write(2, "An error occured with a pthread_join.\n", 38);
			}
	return (1);
}

int	create_monitor(t_shared_env *shared_env, t_coder *coders)
{
	// shared_env->monitor = NULL;
	shared_env->monitor = malloc(sizeof(pthread_t));
	if (!shared_env->monitor)
	{
		free_all((void *[]){shared_env->dongles, coders, shared_env},
						3, shared_env->nb_cod - 1, 3);
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
