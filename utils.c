/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 12:46:49 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/27 18:19:55 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/// @brief Display the message number times
/// @param coder The coder who execute the action.
/// @param message The message to display.
/// @param number The number of time to display.
void	display_message(t_coder *coder, char *message, int number)
{
	long long	current_timestamp;
	int			i;

	pthread_mutex_lock(coder->lock_coder_data);
	i = 0;
	if (get_sim_state(coder->shared_env) == 0)
	{
		pthread_mutex_unlock(coder->lock_coder_data);
		return ;
	}
	while (i < number)
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

int	stop_taking_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->dongles[1]->lock);
	pthread_mutex_unlock(&coder->dongles[0]->lock);
	return (0);
}

void	unlock_dongles(t_coder *coder)
{
	int	i;

	i = 0;
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
