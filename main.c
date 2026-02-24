/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 08:13:06 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/24 07:30:55 by bfitte           ###   ########lyon.fr   */
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

// long long	get_waited_time(long long waited_time_1, long long waited_time_2,
// 					long long cooldown)
// {
// 	long long	result;

// 	if (waited_time_1 < waited_time_2)
// 	result = cooldown - waited_time_1;
// 	else
// 	result = cooldown - waited_time_2;
// 	return result;
// }

// long long	checking_available(t_dongle *dongles, int cod_id, int nb_cod,
// 						long long cooldown)
// {
// 	struct timeval 	tv;
// 	t_dongle		dongle_1;
// 	t_dongle		dongle_2;
// 	long long		waited_time_1;
// 	long long		waited_time_2;

// 	dongle_1 = dongles[cod_id];
// 	if (cod_id > 0)
// 		dongle_2 = dongles[cod_id - 1];
// 	else
// 		dongle_2 = dongles[nb_cod - 1];
// 	if (dongle_1.free && dongle_2.free)
// 	{
// 		if (gettimeofday(&tv, NULL) == -1)
// 			return (-1);
// 		waited_time_1 = ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) - dongle_1.released_at;
// 		waited_time_2 = ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) - dongle_2.released_at;
// 		if ((waited_time_1 >= cooldown) && (waited_time_2 >= cooldown))
// 			return (1);
// 		else
// 			return get_waited_time(waited_time_1, waited_time_2, cooldown);
// 	}
// 	else
// 		return (0);
// }

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
					shared_env->nb_cod);
	}
	return (0);
}
