/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 08:13:06 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/20 13:42:04 by bfitte           ###   ########lyon.fr   */
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

long long	get_waited_time(long long waited_time_1, long long waited_time_2,
					long long cooldown)
{
	long long	result;

	if (waited_time_1 < waited_time_2)
	result = cooldown - waited_time_1;
	else
	result = cooldown - waited_time_2;
	return result;
}

long long	checking_available(t_dongle *dongles, int cod_id, int nb_cod,
						long long cooldown)
{
	struct timeval 	tv;
	t_dongle		dongle_1;
	t_dongle		dongle_2;
	long long		waited_time_1;
	long long		waited_time_2;

	dongle_1 = dongles[cod_id];
	if (cod_id > 0)
		dongle_2 = dongles[cod_id - 1];
	else
		dongle_2 = dongles[nb_cod - 1];
	if (dongle_1.free && dongle_2.free)
	{
		if (gettimeofday(&tv, NULL) == -1)
			return (-1);
		waited_time_1 = ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) - dongle_1.released_at;
		waited_time_2 = ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) - dongle_2.released_at;
		if ((waited_time_1 >= cooldown) && (waited_time_2 >= cooldown))
			return (1);
		else
			return get_waited_time(waited_time_1, waited_time_2, cooldown);
	}
	else
		return (0);
}

void	get_end_cooldown(long long waited_time, struct timespec *ts)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	waited_time += (now.tv_sec * 1000LL) + (now.tv_usec / 1000);
	ts->tv_sec = waited_time / 1000;
	ts->tv_nsec = (waited_time % 1000) * 1000000;
}

void	*taking_dongles(t_coder coder)
{
	struct timespec	ts;
	long long	available;

	// Faire un lock pour chaque clef comme prévu.
	pthread_mutex_lock(&coder.lock_dongle);
	available = checking_available(coder.shared_env->dongles, coder.id,
									coder.shared_env->nb_cod,
									coder.shared_env->dongle_cooldown);
	while (available != 1)
	{
		if (available == -1)
			return NULL;
		else if (available == 0)
			pthread_cond_wait(&coder.cond, &coder.lock_dongle);
		else
		{
			get_end_cooldown(available, &ts);
			pthread_cond_timedwait(&coder.cond, &coder.lock_dongle, &ts);
		}
	}
}

int main(int argc, char **argv)
{
	pthread_t		*threads;
	t_dongle		*dongles;
	t_coder			*coders;
	t_shared_env	*shared_env;

	if (argc != 9)
		return num_args(argc);
	else
		shared_env = parsing(argc - 1, argv + 1);
		if (!shared_env)
			return (0);
	return (0);
}
