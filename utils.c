/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 12:46:49 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/26 12:54:47 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

long long	get_time_now()
{
	struct timeval ts;

	gettimeofday(&ts, NULL);
	return (ts.tv_sec * 1000LL) + (ts.tv_usec / 1000);
}

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

int	priority_ok(t_coder *coder)
{
	coder->dongles[0]->free = 0;
	coder->dongles[1]->free = 0;
	return (1);
}

void	priority_ko(t_coder *coder)
{
	pthread_mutex_unlock(&coder->dongles[1]->lock);
	pthread_cond_wait(coder->cond_priority, &coder->dongles[0]->lock);
	pthread_mutex_lock(&coder->dongles[1]->lock);
}

void	check_res_available(long long available, t_coder *coder)
{
	struct timespec	ts;

	if (available == 0)
		{
			// printf("no free : %lu\n", pthread_self());
			pthread_mutex_unlock(&coder->dongles[1]->lock);
			pthread_cond_wait(coder->cond_free, &coder->dongles[0]->lock);
			pthread_mutex_lock(&coder->dongles[1]->lock);
		}
	else if (available != 1)
		{
			// printf("no available : %lu\n", pthread_self());
			get_end_cooldown(available, &ts);
			pthread_mutex_unlock(&coder->dongles[1]->lock);
			pthread_cond_timedwait(&coder->cond_available, &coder->dongles[0]->lock, &ts);
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
		waited_time_1 = ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) - coder->dongles[0]->released_at;
		waited_time_2 = ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) - coder->dongles[1]->released_at;
		// printf("Check available : %lu\n", pthread_self());
		// printf("Le 1 : %lld\n", waited_time_1);
		// printf("Le 2 : %lld\n", waited_time_2);
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
	// t_coder	*temp;
	int	i;

	i = 0;
	coder->request_time = 0;
	while (i < 2)
	{
		coder->dongles[i]->free = 1;
		// printf("Unlock id 1 usb 1 : %d\n", coder->dongles[i]->priority->order[0]->id);
		// printf("Unlock id 2 usb 1 : %d\n", coder->dongles[i]->priority->order[1]->id);
		// temp = coder->dongles[i]->priority->order[1];
		// coder->dongles[i]->priority->order[1] = coder;
		// coder->dongles[i]->priority->order[0] = temp;
		// printf("Unlock id 1 usb 1 : %d\n", coder->dongles[i]->priority->order[0]->id);
		// printf("Unlock id 2 usb 1 : %d\n", coder->dongles[i]->priority->order[1]->id);
		coder->dongles[i]->released_at = get_time_now();
		pthread_mutex_unlock(&coder->dongles[i]->lock);
		i++;
	}
	pthread_cond_broadcast(coder->cond_free);
	pthread_cond_broadcast(coder->cond_priority);
}

void	set_sim_state(t_shared_env *shared_env, int i)
{
	pthread_mutex_lock(&shared_env->lock_sim_state);
	shared_env->simulation_state = i;
	pthread_mutex_unlock(&shared_env->lock_sim_state);
}

int	get_sim_state(t_shared_env *shared_env)
{
	pthread_mutex_lock(&shared_env->lock_sim_state);
	if (shared_env->simulation_state == 1)
	{
		pthread_mutex_unlock(&shared_env->lock_sim_state);
		return (1);
	}
	else
	{
		pthread_mutex_unlock(&shared_env->lock_sim_state);
		return (0);
	}
}

void	set_comp_time(t_coder *coder, long long comp_time)
{
	pthread_mutex_lock(&coder->lock_coder_time);
	coder->last_comp_time = comp_time;
	pthread_mutex_unlock(&coder->lock_coder_time);
}

long long	get_comp_time(t_coder *coder)
{
	long long	time;

	pthread_mutex_lock(&coder->lock_coder_time);
	time = coder->last_comp_time;
	pthread_mutex_unlock(&coder->lock_coder_time);
	return time;
}