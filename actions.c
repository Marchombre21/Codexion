/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 13:35:46 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/26 09:36:48 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

int	taking_dongles(t_coder *coder)
{
	long long	available;
	pthread_mutex_lock(&coder->dongles[0]->lock);
	pthread_mutex_lock(&coder->dongles[1]->lock);
	available = checking_available(coder, coder->shared_env->dongle_cooldown);
	coder->request_time = get_time_now();
	while (1)
	{
		insert_priority(coder);
		if (available == -1)
		{
			write(2, "An error occurs with timestamp retrieval.", 41);
			return (0);
		}
		check_available(available, coder);
		if (available == 1)
		{
			if ((coder->id == coder->dongles[0]->priority->order[0]->id) &&
			(coder->id == coder->dongles[1]->priority->order[0]->id))
			{
				// printf("Enter : %lu\n", pthread_self());
				coder->dongles[0]->free = 0;
				coder->dongles[1]->free = 0;
				return (1);
			}
			else
			{
				// printf("no priority : %lu\n", pthread_self());
				pthread_mutex_unlock(&coder->dongles[1]->lock);
				pthread_cond_wait(coder->cond_priority, &coder->dongles[0]->lock);
				pthread_mutex_lock(&coder->dongles[1]->lock);
			}
		}
		available = checking_available(coder, coder->shared_env->dongle_cooldown);
	}
	return (0);
}

void	insert_priority(t_coder *coder)
{
	int			i;
	// long long	result;
	t_coder		*temp;

	i = 0;
	// printf("Check priority : %lu\n", pthread_self());
	// printf("id 1 usb 1 : %d\n", coder->dongles[0]->priority->order[0]->id);
	// printf("id 2 usb 1 : %d\n", coder->dongles[0]->priority->order[1]->id);
	// printf("id 1 usb 2 : %d\n", coder->dongles[1]->priority->order[0]->id);
	// printf("id 2 usb 2 : %d\n", coder->dongles[1]->priority->order[1]->id);
	while (i < 2)
	{
		if (strcmp(coder->dongles[0]->priority->scheduler, "fifo") == 0)
		{
			if (coder->dongles[i]->priority->order[0]->request_time == 0 ||
				((coder->dongles[i]->priority->order[0]->request_time >
				coder->dongles[i]->priority->order[1]->request_time) &&
				coder->dongles[i]->priority->order[1]->request_time != 0))
			{
				temp = coder->dongles[i]->priority->order[0];
				coder->dongles[i]->priority->order[0] = coder->dongles[i]->priority->order[1];
				coder->dongles[i]->priority->order[1] = temp;
			}
		}
		else
		{
			// result = get_time_now() - coder->dongles[i]->priority->order[0]->last_comp_time;
			// printf("%lld\n", coder->dongles[i]->priority->order[0]->last_comp_time);
			// printf("%lld\n", coder->dongles[i]->priority->order[1]->last_comp_time);
			if (get_comp_time(coder->dongles[i]->priority->order[0]) >
			get_comp_time(coder->dongles[i]->priority->order[1]))
			// if (result < (get_time_now() - coder->last_comp_time))
			{
				temp = coder->dongles[i]->priority->order[0];
				coder->dongles[i]->priority->order[0] = coder->dongles[i]->priority->order[1];
				coder->dongles[i]->priority->order[1] = temp;
			}
			else if (coder->dongles[i]->priority->order[0]->last_comp_time ==
			coder->dongles[i]->priority->order[1]->last_comp_time)
				if(coder->dongles[i]->priority->order[0]->id % 2 != 0)
				{
					temp = coder->dongles[i]->priority->order[0];
					coder->dongles[i]->priority->order[0] = coder->dongles[i]->priority->order[1];
					coder->dongles[i]->priority->order[1] = temp;
				}
		}
		i++;
	}
	// printf("id 1 usb 1 : %d\n", coder->dongles[0]->priority->order[0]->id);
	// printf("id 2 usb 1 : %d\n", coder->dongles[0]->priority->order[1]->id);
	// printf("id 1 usb 2 : %d\n", coder->dongles[1]->priority->order[0]->id);
	// printf("id 2 usb 2 : %d\n", coder->dongles[1]->priority->order[1]->id);
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


