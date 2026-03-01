/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 08:13:06 by bfitte            #+#    #+#             */
/*   Updated: 2026/03/01 17:04:51 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	num_args(int number)
{
	if (number < 9)
		printf("Not enough arguments");
	else if (number > 9)
		printf("Too much arguments");
	return (0);
}

int	main(int argc, char **argv)
{
	t_coder			*coders;
	t_shared_env	*shared_env;
	void			*error;

	if (argc != 9)
		return (num_args(argc));
	else
	{
		shared_env = parsing(argc - 1, argv + 1);
		if (!shared_env)
			return (0);
		error = create_dongles(shared_env);
		if (!error)
			return (1);
		coders = create_coders(shared_env);
		if (!coders)
			return (1);
		if (create_monitor(shared_env, coders) == 1)
			if (create_threads(shared_env, coders) == 1)
				final(shared_env, coders);
	}
	return (0);
}
