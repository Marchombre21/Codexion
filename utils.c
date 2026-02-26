/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 12:46:49 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/26 15:26:37 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

/// @brief Display the message number times
/// @param coder The coder who execute the action.
/// @param message The message to display.
/// @param number The number of time to display.
void	display_message(t_coder *coder, char *message, int number)
{
	pthread_mutex_lock(coder->lock_coder_data);
	long long	current_timestamp;
	int			i;

	i = 0;
	if (get_sim_state(coder->shared_env) == 0)
	{
		pthread_mutex_unlock(coder->lock_coder_data);
		return ;
	}
	while(i < number)
	{
		current_timestamp = get_time_now() - coder->shared_env->start;
		printf("%lld %i %s\n", current_timestamp, coder->id, message);
		i++;
	}
	pthread_mutex_unlock(coder->lock_coder_data);
}

void	get_end_cooldown(long long waited_time, struct timespec *ts)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	waited_time += (now.tv_sec * 1000LL) + (now.tv_usec / 1000);
	ts->tv_sec = waited_time / 1000;
	ts->tv_nsec = (waited_time % 1000) * 1000000;
}

// int	priority_ok(t_coder *coder)
// {
// 	coder->dongles[0]->free = 0;
// 	coder->dongles[1]->free = 0;
// 	return (1);
// }

// void	priority_ko(t_coder *coder)
// {
// 	pthread_mutex_unlock(&coder->dongles[1]->lock);
// 	pthread_cond_wait(coder->cond_priority, &coder->dongles[0]->lock);
// 	pthread_mutex_lock(&coder->dongles[1]->lock);
// }

int	stop_taking_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->dongles[1]->lock);
	pthread_mutex_unlock(&coder->dongles[0]->lock);
	return (0);
}

void	check_res_available(long long available, t_coder *coder)
{
	struct timespec	ts;

	if (available == 0)
		{
			pthread_mutex_unlock(&coder->dongles[1]->lock);
			pthread_cond_wait(coder->cond_free, &coder->dongles[0]->lock);
			pthread_mutex_lock(&coder->dongles[1]->lock);
		}
	else if (available != 1)
		{
			get_end_cooldown(available, &ts);
			pthread_mutex_unlock(&coder->dongles[1]->lock);
			pthread_cond_timedwait(&coder->cond_available,
									&coder->dongles[0]->lock, &ts);
			pthread_mutex_lock(&coder->dongles[1]->lock);
		}
}

long long	check_availability(t_coder *coder, long long cooldown)
{
	struct timeval 	tv;
	long long		waited_time_1;
	long long		waited_time_2;

	if (coder->dongles[0]->free && coder->dongles[1]->free)
	{
		if (gettimeofday(&tv, NULL) == -1)
			return (-1);
		waited_time_1 = ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) -
							coder->dongles[0]->released_at;
		waited_time_2 = ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) -
							coder->dongles[1]->released_at;
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


void	unlock_dongles(t_coder *coder)
{
	int	i;

	i = 0;
	set_request(coder, 0);
	while (i < 2)
	{
		coder->dongles[i]->free = 1;
		coder->dongles[i]->released_at = get_time_now();
		pthread_mutex_unlock(&coder->dongles[i]->lock);
		i++;
	}
	pthread_cond_broadcast(coder->cond_free);
	pthread_cond_broadcast(coder->cond_priority);
}
