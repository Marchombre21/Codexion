/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 16:50:47 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/27 11:00:23 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	insert_priority(t_coder *coder)
{
	int			i;
	t_coder		*temp;

	i = 0;
	temp = NULL;
	while (i < 2)
	{
		if (strcmp(coder->dongles[0]->priority->scheduler, "fifo") == 0)
			fifo(coder, i, temp);
		else
			edf(coder, i, temp);
		i++;
	}
}

void	check_res_available(long long available, t_coder *coder)
{
	struct timespec	ts;

	if (available == 0)
	{
		set_request(coder, 0);
		insert_priority(coder);
		pthread_mutex_unlock(&coder->dongles[1]->lock);
		pthread_cond_wait(coder->cond_free, &coder->dongles[0]->lock);
		pthread_mutex_lock(&coder->dongles[1]->lock);
	}
	else if (available != -2)
	{
		set_request(coder, get_time_now() + available);
		insert_priority(coder);
		get_end_cooldown(available, &ts);
		pthread_mutex_unlock(&coder->dongles[1]->lock);
		pthread_cond_timedwait(&coder->cond_available,
			&coder->dongles[0]->lock, &ts);
		pthread_mutex_lock(&coder->dongles[1]->lock);
	}
}

/// @brief Check if the coder's dongles are free and if their cooldown ends
/// @param coder The coder who attempt to take the dongles.
/// @param cooldown The dongles's cooldown
/// @return -1 if gettimeofday fails, -2 if all is good, a long long equal to
/// the time to wait.
long long	check_availability(t_coder *coder, long long cooldown)
{
	struct timeval	tv;
	long long		waited_time_1;
	long long		waited_time_2;

	if (coder->dongles[0]->free && coder->dongles[1]->free)
	{
		if (gettimeofday(&tv, NULL) == -1)
			return (-1);
		waited_time_1 = ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000))
			- coder->dongles[0]->released_at;
		waited_time_2 = ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000))
			- coder->dongles[1]->released_at;
		if ((waited_time_1 >= cooldown) && (waited_time_2 >= cooldown))
			return (-2);
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
