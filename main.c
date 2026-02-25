/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 08:13:06 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/25 12:05:53 by bfitte           ###   ########lyon.fr   */
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
	while (new_coder->count_compile < new_coder->shared_env->number_of_compiles_required)
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

int main(int argc, char **argv)
{
	t_coder			*coders;
	t_shared_env	*shared_env;

	if (argc != 9)
		return num_args(argc);
	else
	{
		shared_env = parsing(argc - 1, argv + 1);
		if (!shared_env)
			return (0);
		shared_env->dongles = create_dongles(shared_env);
		if(!shared_env->dongles)
			return (1);
		coders = create_coders(shared_env);
		if(!coders)
			return (1);
		// initialize_priority(shared_env->dongles, coders);
		if (create_threads(shared_env, coders) == 1)
			free_all((void *[]){shared_env->dongles, coders, shared_env}, 3,
					shared_env->nb_cod, 1);
	}
	return (0);
}
