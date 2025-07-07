/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaghdad <sbaghdad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:20:01 by sbaghdad          #+#    #+#             */
/*   Updated: 2025/07/07 19:53:28 by sbaghdad         ###   ########.fr       */
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
	while (j < data->n_ph)
	{
		data->philos[j].id = j + 1;
		data->philos[j].data = data;
		data->philos[j].death_himself = 0;
		data->philos[j].m_eting = 0;
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
		data->philos[j].right_fork = data->forks + (j + 1) % data->n_ph;
		j++;
	}
	return (0);
}

int	alloc_philos(t_data *data)
{
	data->philos = malloc(sizeof(t_philo) * data->n_ph);
	if (!data->philos)
		return (ft_putstrfd("malloc failed\n", 2), 1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->n_ph);
	if (!data->forks)
		return (ft_putstrfd("malloc failed\n", 2), free(data->philos), 1);
	if (pthread_mutex_init(&data->p_l, NULL) != 0)
	{
		ft_putstrfd("pthread_mutex_init failed\n", 2);
		return (free(data->philos), free(data->forks), 1);
	}
	if (pthread_mutex_init(&data->died, NULL))
	{
		pthread_mutex_destroy(&data->p_l);
		ft_putstrfd("pthread_mutex_init failed\n", 2);
		return (free(data->forks), free(data->philos), 1);
	}
	return (0);
}

int	init(t_data *c)
{
	if (pthread_mutex_init(&c->f, NULL))
	{
		(pthread_mutex_destroy(&c->died), pthread_mutex_destroy(&c->p_l));
		return (clean_philos(c, c->n_ph), free(c->forks), free(c->philos), 1);
	}
	if (pthread_mutex_init(&c->last_meal, NULL))
	{
		pthread_mutex_destroy(&c->died);
		(pthread_mutex_destroy(&c->p_l), pthread_mutex_destroy(&c->f));
		return (clean_philos(c, c->n_ph), free(c->forks), free(c->philos), 1);
	}
	return (0);
}

int	one_philo(t_philo *philo)
{
	if (philo->data->n_ph == 1)
	{
		print_action(philo, "has taken fork");
		while (!philo->data->someone_died)
			ft_usleep(50, philo->data);
		return (1);
	}
	return (0);
}
