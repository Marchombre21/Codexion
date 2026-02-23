/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfitte <bfitte@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 14:18:09 by bfitte            #+#    #+#             */
/*   Updated: 2026/02/20 14:49:38 by bfitte           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	*free_all(void *ptr[], int number)
{
	int i;

	i = 0;
	while (i < number)
		free(ptr[i++]);
	return NULL;
}