/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 08:13:06 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/23 11:59:05 by bfitte           ###   ########lyon.fr   */
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
long long	checking_available(t_coder coder, long long cooldown)
{
	struct timeval 	tv;
	long long		waited_time_1;
	long long		waited_time_2;

	if (coder.dongles[0].free && coder.dongles[1].free)
	{
		if (gettimeofday(&tv, NULL) == -1)
			return (-1);
		waited_time_1 = ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) - coder.dongles[0].released_at;
		waited_time_2 = ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) - coder.dongles[1].released_at;
		if ((waited_time_1 >= cooldown) && (waited_time_2 >= cooldown))
			return (1);
		else
		{
			if (waited_time_1 < waited_time_2)
				return (cooldown - waited_time_1);
			else
				return (cooldown - waited_time_2);
		}
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

void	check_available(long long available, t_coder coder)
{
	struct timespec	ts;

	if (available == -1)
			return NULL;
		else if (available == 0)
		{
			pthread_cond_wait(coder.cond_free, &coder.dongles[0].lock);
			pthread_cond_wait(coder.cond_free, &coder.dongles[1].lock);
		}
		else
		{
			get_end_cooldown(available, &ts);
			pthread_cond_timedwait(coder.cond_available, &coder.dongles[0].lock, &ts);
			pthread_cond_timedwait(coder.cond_available, &coder.dongles[1].lock, &ts);
		}
}

long long	get_time_now()
{
	struct timeval ts;

	gettimeofday(&ts, NULL);
	return (ts.tv_sec * 1000LL) + (ts.tv_usec / 1000);
}

void	unlock_dongles(t_coder coder)
{
	int	temp;
	int	i;

	i = 0;
	while (i < 2)
	{
		pthread_mutex_unlock(&coder.dongles[i].lock);
		coder.dongles[i].free = 1;
		temp = coder.dongles[i].priority[1];
		coder.dongles[i].priority[1] = coder.id;
		coder.dongles[i].priority[0] = temp;
		coder.dongles[i].released_at = get_time_now();
		pthread_mutex_unlock(&coder.dongles[i].lock);
		pthread_cond_broadcast(coder.cond_free);
		pthread_cond_broadcast(coder.cond_priority);
		i++;
	}
}

void display_message(t_coder coder, char *message)
{
	pthread_mutex_lock(coder.lock_coder_data);
	fprintf
}

void	start_compile(t_coder coder)
{
	usleep(coder.shared_env->time_to_compile);
	unlock_dongles(coder);
}

void	*taking_dongles(t_coder coder)
{
	long long	available;

	pthread_mutex_lock(&coder.dongles[0].lock);
	pthread_mutex_lock(&coder.dongles[1].lock);
	available = checking_available(coder, coder.shared_env->dongle_cooldown);
	while (available != 1)
	{
		check_available(available, coder);
		if ((coder.id == coder.dongles[0].priority[0]) &&
		(coder.id == coder.dongles[1].priority[0]))
		{
			coder.dongles[0].free = 0;
			coder.dongles[1].free = 0;
			start_compile(coder);
		}
		else
			pthread_cond_wait(&coder.cond_priority, &coder.dongles[0].lock);
			pthread_cond_wait(&coder.cond_priority, &coder.dongles[1].lock);
		available = checking_available(coder, coder.shared_env->dongle_cooldown);
	}
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
		coders = create_coders(shared_env);
		initialize_priority(shared_env->dongles, coders);
	return (0);
}
