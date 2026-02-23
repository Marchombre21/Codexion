/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 08:13:06 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/20 15:07:54 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	num_args(int number)
{
	if (number < 9)
		printf("Not enough arguments");
	else if (number > 9)
		printf("Too much arguments");
	return (0);
}

int main(int argc, char **argv)
{
	pthread_t		*threads;
	t_coder			*coders;
	t_shared_env	*shared_env;

	if (argc != 9)
		return num_args(argc);
	else
		shared_env = parsing(argc - 1, argv + 1);
		if (!shared_env)
			return (0);
		create_dongles(shared_env);
		coders = 
	return (0);
}
