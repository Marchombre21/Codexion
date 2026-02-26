/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 08:13:06 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/26 07:45:31 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

int	num_args(int number)
{
	if (number < 9)
		printf("Not enough arguments");
	else if (number > 9)
		printf("Too much arguments");
	return (0);
}

void	*coder_routine(void *coder)
{
	t_coder		*new_coder;

	new_coder = (t_coder *)coder;
	while ((get_sim_state(new_coder->shared_env) == 1) &&
	new_coder->count_compile <
	new_coder->shared_env->number_of_compiles_required)
	{
		if (taking_dongles(new_coder) == 1)
		{
			start_compile(new_coder);
			start_debugging(new_coder);
			start_refactoring(new_coder);
		}
	}
	return NULL;
}

void	*monitor_routine(void *coders)
{
	t_coder		*new_coders;
	long long	limit;
	int			i;

	new_coders = (t_coder *)coders;
	limit = new_coders[0].shared_env->time_to_burnout;
	while (get_sim_state(new_coders[0].shared_env) == 1)
	{
		i = 0;
		while (i < new_coders[0].shared_env->nb_cod)
		{
			if((get_time_now() - get_comp_time(&new_coders[i])) >= limit)
			{
				pthread_mutex_unlock(&new_coders[i].lock_coder_time);
				set_sim_state(new_coders[0].shared_env, 0);
				printf("%lld %i burned out", get_time_now(), new_coders[i].id);
				break;
			}
			pthread_mutex_unlock(&new_coders[i].lock_coder_time);
			i++;
		}
	}
	return NULL;
}

void	final(t_shared_env *shared_env, t_coder *coders)
{
	if (get_sim_state(shared_env) == 0)
		usleep((shared_env->time_to_compile +
				shared_env->time_to_debug + shared_env->time_to_refactor) *
			1000);
	free_all((void *[]){shared_env->dongles, coders, shared_env}, 3,
			shared_env->nb_cod, 1);
}

int main(int argc, char **argv)
{
	t_coder			*coders;
	t_shared_env	*shared_env;
	void			*error;

	if (argc != 9)
		return num_args(argc);
	else
	{
		shared_env = parsing(argc - 1, argv + 1);
		if (!shared_env)
			return (0);
		error = create_dongles(shared_env);
		if(!error)
			return (1);
		coders = create_coders(shared_env);
		if(!coders)
			return (1);
		if (create_monitor(shared_env, coders) == 1)
			if (create_threads(shared_env, coders) == 1)
				final(shared_env, coders);
	}
	return (0);
}
