/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 13:35:46 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/26 13:06:44 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

static void	insert_priority(t_coder *coder)
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
/// @brief Locks the mutexes and checks if dongles are availables
/// If they aren't run the cond_wait or the timed_wait. If they are availables,
/// check the priority queue and adapt its behavior.
/// @param coder The coder who try to use the dongle.
/// @return 1 if he can, 0 if not.
int	taking_dongles(t_coder *coder)
{
	long long	available;
	pthread_mutex_lock(&coder->dongles[0]->lock);
	pthread_mutex_lock(&coder->dongles[1]->lock);
	available = check_availability(coder, coder->shared_env->dongle_cd);
	coder->request_time = get_time_now();
	while (1)
	{
		insert_priority(coder);
		if (available == -1)
		{
			write(2, "An error occurs with timestamp retrieval.", 41);
			return (0);
		}
		check_res_available(available, coder);
		if (available == 1)
		{
			if ((coder->id == coder->dongles[0]->priority->order[0]->id) &&
			(coder->id == coder->dongles[1]->priority->order[0]->id))
				return (priority_ok(coder));
			else
				priority_ko(coder);
		}
		available = check_availability(coder, coder->shared_env->dongle_cd);
	}
	return (0);
}

void	start_refactoring(t_coder *coder)
{
	display_message(coder, "is refactoring", 1);
	usleep(coder->shared_env->time_to_refactor * 1000);
}

void	start_debugging(t_coder *coder)
{
	display_message(coder, "is debugging", 1);
	usleep(coder->shared_env->time_to_debug * 1000);
}

/// @brief Display messages, wait for the time_to_compile time and unlock
/// dongles
/// @param coder The coder who execute the action.
void	start_compile(t_coder *coder)
{
	display_message(coder, "has taken a dongle", 2);
	display_message(coder, "is compiling", 1);
	set_comp_time(coder, get_time_now());
	usleep(coder->shared_env->time_to_compile * 1000);
	unlock_dongles(coder);
	coder->count_compile += 1;
}
