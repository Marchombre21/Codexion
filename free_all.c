/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 14:18:09 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/23 15:54:41 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coders/codexion.h"

void	*free_all(void *ptr[], int number, int nb_priority_array)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (j < nb_priority_array)
		free(((t_dongle *)ptr[i])[j++].priority);
	while (i < number)
		free(ptr[i++]);
	return NULL;
}