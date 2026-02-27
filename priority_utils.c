/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 11:59:12 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/27 13:54:15 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	fifo(t_coder *coder, int i, t_coder *temp)
{
	long long	now;
	long long	request_time_0;
	long long	request_time_1;

	now = get_time_now();
	request_time_0 = get_request(coder->dongles[i]->priority->order[0]);
	request_time_1 = get_request(coder->dongles[i]->priority->order[1]);
	if (request_time_0 == 0 || request_time_0 > now
		|| ((request_time_0 > request_time_1) && request_time_1 != 0
			&& request_time_1 <= now))
	{
		temp = coder->dongles[i]->priority->order[0];
		coder->dongles[i]->priority->order[0]
			= coder->dongles[i]->priority->order[1];
		coder->dongles[i]->priority->order[1] = temp;
	}
}

void	edf(t_coder *coder, int i, t_coder *temp)
{
	long long	now;
	long long	request_time_0;
	long long	request_time_1;
	long long	comp_0;
	long long	comp_1;

	now = get_time_now();
	request_time_0 = get_request(coder->dongles[i]->priority->order[0]);
	request_time_1 = get_request(coder->dongles[i]->priority->order[1]);
	comp_0 = get_comp_time(coder->dongles[i]->priority->order[0]);
	comp_1 = get_comp_time(coder->dongles[i]->priority->order[1]);
	if (request_time_0 == 0
		|| (request_time_1 != 0 && request_time_1 <= now && comp_0 > comp_1)
		|| (request_time_1 != 0 && request_time_1 <= now && comp_0 == comp_1
			&& request_time_0 > now))
	{
		temp = coder->dongles[i]->priority->order[0];
		coder->dongles[i]->priority->order[0]
			= coder->dongles[i]->priority->order[1];
		coder->dongles[i]->priority->order[1] = temp;
	}
}

int	check_priority(t_coder *coder)
{
	if ((coder->id == coder->dongles[0]->priority->order[0]->id)
		&& (coder->id == coder->dongles[1]->priority->order[0]->id))
	{
		coder->dongles[0]->free = 0;
		coder->dongles[1]->free = 0;
		return (1);
	}
	else
	{
		pthread_mutex_unlock(&coder->dongles[1]->lock);
		pthread_cond_wait(coder->cond_priority, &coder->dongles[0]->lock);
		pthread_mutex_lock(&coder->dongles[1]->lock);
		return (0);
	}
}
