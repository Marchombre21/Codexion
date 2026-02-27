/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 09:36:37 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/27 20:42:10 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	*no_coders(void)
{
	printf("At least one coder must be present\n");
	return (NULL);
}

static int	final_check_input(int i, char *nptr)
{
	long long	result;

	result = 0;
	if (!(nptr[i] >= 48 && nptr[i] <= 57))
		return (1);
	while (nptr[i])
	{
		if (!(nptr[i] <= 57 && nptr[i] >= 48))
			return (1);
		result = (result * 10 + (nptr[i] - 48));
		if (result > INT_MAX)
			return (1);
		i++;
	}
	return (0);
}

static int	check_input(char *nptr)
{
	int			i;

	i = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			return (1);
		i++;
	}
	return (final_check_input(i, nptr));
}

static void	*create_shared_env(int args_array[], char *scheduler)
{
	t_shared_env	*shared_env;

	if (args_array[0] == 0)
		return (no_coders());
	shared_env = malloc(sizeof(t_shared_env));
	if (!shared_env)
		return (NULL);
	shared_env->nb_cod = args_array[0];
	shared_env->start = get_time_now();
	shared_env->time_to_burnout = args_array[1];
	shared_env->simulation_state = 0;
	shared_env->threads = NULL;
	shared_env->time_to_compile = args_array[2];
	pthread_cond_init(&shared_env->cond_priority, NULL);
	pthread_cond_init(&shared_env->cond_free, NULL);
	pthread_mutex_init(&shared_env->lock_coder_data, NULL);
	pthread_mutex_init(&shared_env->lock_sim_state, NULL);
	shared_env->time_to_debug = args_array[3];
	shared_env->time_to_refactor = args_array[4];
	shared_env->number_of_compiles_required = args_array[5];
	shared_env->dongle_cd = args_array[6];
	shared_env->scheduler = scheduler;
	return (shared_env);
}

void	*parsing(int argc, char **args)
{
	int	i;
	int	args_array[8];

	i = 0;
	while (i < argc - 1)
	{
		if (check_input(args[i]) == 0)
			args_array[i] = atoi(args[i]);
		else
		{
			printf("All arguments (except the last one) must be valid and"
				" positive integers.\n");
			return (NULL);
		}
		i++;
	}
	if (strcmp(args[i], "fifo") == 0 || strcmp(args[i], "edf") == 0)
		return (create_shared_env(args_array, args[i]));
	else
	{
		printf("Last argument must be either 'fifo' or 'edf'.\n");
		return (NULL);
	}
}
