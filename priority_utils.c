/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 11:59:12 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/26 14:02:47 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	fifo(t_coder *coder, int i, t_coder *temp)
{
	if (coder->dongles[i]->priority->order[0]->request_time == 0 ||
				((coder->dongles[i]->priority->order[0]->request_time >
				coder->dongles[i]->priority->order[1]->request_time) &&
				coder->dongles[i]->priority->order[1]->request_time != 0))
			{
				temp = coder->dongles[i]->priority->order[0];
				coder->dongles[i]->priority->order[0] =
				coder->dongles[i]->priority->order[1];
				coder->dongles[i]->priority->order[1] = temp;
			}
}

void	edf(t_coder *coder, int i, t_coder *temp)
{
	if (get_comp_time(coder->dongles[i]->priority->order[0]) >
			get_comp_time(coder->dongles[i]->priority->order[1]))
			{
				temp = coder->dongles[i]->priority->order[0];
				coder->dongles[i]->priority->order[0] =
				coder->dongles[i]->priority->order[1];
				coder->dongles[i]->priority->order[1] = temp;
			}
			else if (coder->dongles[i]->priority->order[0]->last_comp_time ==
			coder->dongles[i]->priority->order[1]->last_comp_time)
				if(coder->dongles[i]->priority->order[0]->id % 2 != 0)
				{
					temp = coder->dongles[i]->priority->order[0];
					coder->dongles[i]->priority->order[0] =
					coder->dongles[i]->priority->order[1];
					coder->dongles[i]->priority->order[1] = temp;
				}
}

int	check_priority(t_coder *coder)
{
	if ((coder->id == coder->dongles[0]->priority->order[0]->id) &&
			(coder->id == coder->dongles[1]->priority->order[0]->id))
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
