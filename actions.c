/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 13:35:46 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/27 08:34:54 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

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
	while (1)
	{
		if (get_sim_state(coder->shared_env) != 1)
			return (stop_taking_dongles(coder));
		set_request(coder, get_time_now());
		insert_priority(coder);
		if (available == -1)
		{
			write(2, "An error occurs with timestamp retrieval.\n", 42);
			return (0);
		}
		check_res_available(available, coder);
		if (available == -2)
			if (check_priority(coder) == 1)
				return (1);
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
	display_message(coder, "is debugging", 1);
	unlock_dongles(coder);
	coder->count_compile += 1;
	if (coder->count_compile >= coder->shared_env->number_of_compiles_required)
		set_finish(coder);
}
