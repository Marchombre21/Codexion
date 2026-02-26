/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 11:59:12 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/26 12:09:33 by bfitte           ###   ########lyon.fr   */
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
