/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 13:35:46 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/24 06:50:10 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	*taking_dongles(void *coder)
{
	long long	available;
	t_coder		*new_coder;

	new_coder = (t_coder *)coder;
	pthread_mutex_lock(&new_coder->dongles[0].lock);
	pthread_mutex_lock(&new_coder->dongles[1].lock);
	available = checking_available(new_coder, new_coder->shared_env->dongle_cooldown);
	while (1)
	{
		if (available == -1)
		{
			stop_threads(new_coder->shared_env->threads, new_coder);
			return NULL;
		}
		check_available(available, new_coder);
		if (available == 1)
		{
			if ((new_coder->id == new_coder->dongles[0].priority[0]) &&
			(new_coder->id == new_coder->dongles[1].priority[0]))
			{
				new_coder->dongles[0].free = 0;
				new_coder->dongles[1].free = 0;
				start_compile(new_coder);
				return NULL;
			}
			else
			{
				pthread_mutex_unlock(&new_coder->dongles[1].lock);
				pthread_cond_wait(new_coder->cond_priority, &new_coder->dongles[0].lock);
				pthread_mutex_lock(&new_coder->dongles[1].lock);
			}
		}
		available = checking_available(new_coder, new_coder->shared_env->dongle_cooldown);
	}
	return NULL;
}

void	start_refactoring(t_coder *coder)
{
	display_message(coder, "is refactoring", 1);
	usleep(coder->shared_env->time_to_refactor);
	taking_dongles(coder);
}

void	start_debugging(t_coder *coder)
{
	display_message(coder, "is debugging", 1);
	usleep(coder->shared_env->time_to_debug);
	start_refactoring(coder);
}

/// @brief Display messages, wait for the time_to_compile time and unlock
/// dongles
/// @param coder The coder who execute the action.
void	start_compile(t_coder *coder)
{
	display_message(coder, "has taken a dongle", 2);
	display_message(coder, "is compiling", 1);
	usleep(coder->shared_env->time_to_compile);
	unlock_dongles(coder);
	coder->count_compile += 1;
	if (coder->count_compile >= coder->shared_env->number_of_compiles_required)
		stop_threads(coder->shared_env->threads, coder);
	coder->last_comp_time = get_time_now();
	start_debugging(coder);
}


