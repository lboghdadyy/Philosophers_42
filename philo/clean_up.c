/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaghdad <sbaghdad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:32:19 by sbaghdad          #+#    #+#             */
/*   Updated: 2025/07/03 18:32:22 by sbaghdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	finish(t_data *data)
{
	int	k;

	k = 0;
	while (k < data->b_philos)
	{
		pthread_mutex_destroy(&data->forks[k]);
		pthread_mutex_destroy(&data->philos[k].died);
		k++;
	}
	pthread_mutex_destroy(&data->p_lock);
	pthread_mutex_destroy(&data->died);
	free(data->philos);
	free(data->forks);
}
