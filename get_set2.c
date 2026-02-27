/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 14:43:53 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/27 08:43:39 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_finish(t_coder *coder)
{
	pthread_mutex_lock(coder->lock_coder_data);
	coder->finish = 1;
	pthread_mutex_unlock(coder->lock_coder_data);
}

int	get_finish(t_coder *coder)
{
	int	i;

	pthread_mutex_lock(coder->lock_coder_data);
	i = coder->finish;
	pthread_mutex_unlock(coder->lock_coder_data);
	return (i);
}

int	get_finish_stats(t_coder *coders, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
		if (!coders[i++].finish)
			return (0);
	return (1);
}

long long	get_request(t_coder *coder)
{
	long long	temp;

	pthread_mutex_lock(&coder->lock_coder_request);
	temp = coder->request_time;
	pthread_mutex_unlock(&coder->lock_coder_request);
	return (temp);
}

void	set_request(t_coder *coder, long long time)
{
	pthread_mutex_lock(&coder->lock_coder_request);
	coder->request_time = time;
	pthread_mutex_unlock(&coder->lock_coder_request);
}
