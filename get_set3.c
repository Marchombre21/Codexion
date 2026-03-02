/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 18:51:58 by bfitte            #+#    #+#             */
/*   Updated: 2026/03/02 10:15:16 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_count_comp(t_coder *coder)
{
	int	i;

	pthread_mutex_lock(&coder->lock_coder_count);
	i = coder->count_compile;
	pthread_mutex_unlock(&coder->lock_coder_count);
	return (i);
}

void	set_count_comp(t_coder *coder)
{
	pthread_mutex_lock(&coder->lock_coder_count);
	coder->count_compile++;
	pthread_mutex_unlock(&coder->lock_coder_count);
}

int	get_prio_dongle(t_dongle *dongle)
{
	int	i;

	pthread_mutex_lock(&dongle->lock_priority);
	i = dongle->priority->order[0]->id;
	pthread_mutex_unlock(&dongle->lock_priority);
	return (i);
}

int	get_free_dongle(t_dongle *dongle)
{
	int	i;

	pthread_mutex_lock(&dongle->lock_free);
	i = dongle->free;
	pthread_mutex_unlock(&dongle->lock_free);
	return (i);
}

void	set_free_dongle(t_dongle *dongle, int i)
{
	pthread_mutex_lock(&dongle->lock_free);
	dongle->free = i;
	pthread_mutex_unlock(&dongle->lock_free);
}