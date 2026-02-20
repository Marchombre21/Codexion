/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pointers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:44:29 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/20 14:16:36 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*create_dongle(t_shared_env *shared_env)
{
	t_dongle	*dongles;
	int			*array_priority;
	int			i;
	
	i = 0;
	dongles = malloc(sizeof(t_dongle) * shared_env->numbers_of_coders);
	array_priority = malloc(sizeof(int) * shared_env->numbers_of_coders);
	if (!dongles || !array_priority)
		return NULL;
	while (i < shared_env->numbers_of_coders)
	{
		pthread_mutex_init(&dongles[i].lock, NULL);
		dongles[i].available = 0;
		dongles[i].priority = array_priority;
		i++;
	}
}
