/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 18:51:58 by bfitte            #+#    #+#             */
/*   Updated: 2026/03/01 17:08:54 by bfitte           ###   ########lyon.fr   */
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
