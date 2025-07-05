/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaghdad <sbaghdad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:20:01 by sbaghdad          #+#    #+#             */
/*   Updated: 2025/07/05 10:53:02 by sbaghdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	clean_philos(t_data *data, int index)
{
	int	j;

	j = 0;
	while (j < index)
	{
		pthread_mutex_destroy(&data->philos[j].died);
		pthread_mutex_destroy(&data->forks[j]);
		j++;
	}
}

int	init_philos(t_data *data)
{
	int	j;

	j = 0;
	while (j < data->b_philos)
	{
		data->philos[j].id = j + 1;
		data->philos[j].data = data;
		data->philos[j].death_himself = 0;
		data->philos[j].meals_eaten = 0;
		if (pthread_mutex_init(&data->forks[j], NULL) != 0)
		{
			ft_putstrfd("pthread_mutex_init failed\n", 2);
			return (clean_philos(data, j), 1);
		}
		if (pthread_mutex_init(&data->philos[j].died, NULL) != 0)
		{
			ft_putstrfd("pthread_mutex_init failed\n", 2);
			pthread_mutex_destroy(&data->forks[j]);
			return (clean_philos(data, j), 1);
		}
		data->philos[j].left_fork = data->forks + j;
		data->philos[j].right_fork = data->forks + (j + 1) % data->b_philos;
		j++;
	}
	return (0);
}

int	alloc_philos(t_data *data)
{
	data->philos = malloc(sizeof(t_philo) * data->b_philos);
	if (!data->philos)
		return (ft_putstrfd("malloc failed\n", 2), 1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->b_philos);
	if (!data->forks)
		return (ft_putstrfd("malloc failed\n", 2), free(data->philos), 1);
	if (pthread_mutex_init(&data->p_lock, NULL) != 0)
	{
		ft_putstrfd("pthread_mutex_init failed\n", 2);
		return (free(data->philos), free(data->forks), 1);
	}
	if (pthread_mutex_init(&data->died, NULL))
	{
		pthread_mutex_destroy(&data->p_lock);
		ft_putstrfd("pthread_mutex_init failed\n", 2);
		return (free(data->forks), free(data->philos), 1);
	}
	return (0);
}
