/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pointers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:44:29 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/23 17:34:07 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	*create_dongles(t_shared_env *shared_env)
{
	int				*array_priority;
	t_dongle		*dongles;
	struct timeval	now;
	int				i;
	
	i = 0;
	dongles = malloc(sizeof(t_dongle) * shared_env->nb_cod);
	if (!dongles)
		return free_all((void*[]){shared_env}, 1, 0);
	gettimeofday(&now, NULL);
	while (i < shared_env->nb_cod)
	{
		array_priority = malloc(sizeof(int) * 2);
		if (!array_priority)
			return free_all((void*[]){dongles, shared_env}, 2, i);
		pthread_mutex_init(&dongles[i].lock, NULL);
		dongles[i].free = 1;
		dongles[i].released_at = (now.tv_sec * 1000LL) +
							(now.tv_usec / 1000) + shared_env->dongle_cooldown;
		dongles[i].priority = array_priority;
		i++;
	}
	return dongles;
}

void	initialize_priority(t_dongle *dongles, t_coder *coders)
{
	int i;
	
	i = 0;
	while (i < coders[0].shared_env->nb_cod)
	{
		dongles[i].priority[0] = coders[i].id;
		if (i == coders[0].shared_env->nb_cod - 1)
			dongles[i].priority[1] = coders[0].id;
		else
			dongles[i].priority[1] = coders[i + 1].id;
		i++;
	}
}

void	*create_coders(t_shared_env *shared_env)
{
	t_coder	*coders;
	pthread_cond_t	cond_priority;
	pthread_cond_t	cond_free;
	pthread_mutex_t	lock_coder_data;
	int		i;
	int		nb_max;

	i = 0;
	pthread_cond_init(&cond_priority, NULL);
	pthread_cond_init(&cond_free, NULL);
	pthread_mutex_init(&lock_coder_data, NULL);
	nb_max = shared_env->nb_cod - 1;
	coders = malloc(sizeof(t_coder) * shared_env->nb_cod);
	if (!coders)
		return free_all((void*[]){shared_env->dongles, shared_env}, 3, nb_max);
	while (i < shared_env->nb_cod)
	{
		coders[i].shared_env = shared_env;
		coders[i].last_comp_time = get_time_now();
		coders[i].id = i + 1;
		coders[i].dongles[0] = shared_env->dongles[i];
		if (i > 0)
			coders[i].dongles[1] = shared_env->dongles[i - 1];
		else
			coders[i].dongles[1] = shared_env->dongles[nb_max];
		coders[i].count_compile = 0;
		coders[i].cond_free = &cond_free;
		coders[i].cond_priority = &cond_priority;
		coders[i].lock_coder_data = &lock_coder_data;
		pthread_cond_init(&coders[i].cond_available, NULL);
		i++;
	}
	return coders;
}

int	create_threads(t_shared_env *shared_env, t_coder *coders)
{
	int	i;

	i = 0;
	shared_env->threads = malloc(sizeof(pthread_t) * shared_env->nb_cod);
		if (!shared_env->threads)
		{
			free_all((void *[]){shared_env->dongles, coders, shared_env},
							3, shared_env->nb_cod);
			return (0);
		}
	while (i < shared_env->nb_cod)
	{
		//Rajouter des gestions d'erreurs ici.
		pthread_create(&shared_env->threads[i], NULL, taking_dongles,
						&coders[i]);
		i++;
	}
	i = 0;
	while (i < shared_env->nb_cod)
	//Et ici.
			pthread_join(shared_env->threads[i++], NULL);
	return (1);
}
