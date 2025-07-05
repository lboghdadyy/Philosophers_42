/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaghdad <sbaghdad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:35:51 by sbaghdad          #+#    #+#             */
/*   Updated: 2025/07/05 15:16:48 by sbaghdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	ft_eat(t_philo *philo)
{
	pthread_mutex_t *first;
	pthread_mutex_t *second;

	if (philo->data->b_philos == 1)
	{
		print_action(philo, "has taken fork");
		ft_usleep(philo->data->t_die, philo->data);
	}
	if (philo->id % 2 != 0)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	if (!check_death(philo->data))
		return;
	pthread_mutex_lock(first);
	pthread_mutex_lock(second);
	pthread_mutex_lock(&philo->data->p_lock);
	if (!philo->data->someone_died)
	{
		printf("%ld %d has taken a fork\n", present_time() - philo->data->strt_time, philo->id);
		printf("%ld %d has taken a fork\n", present_time() - philo->data->strt_time, philo->id);
	}
	pthread_mutex_unlock(&philo->data->p_lock);
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->data->last_meal);
	philo->last_meal_time = present_time();
	pthread_mutex_unlock(&philo->data->last_meal);
	ft_usleep(philo->data->t_eat, philo->data);
	pthread_mutex_lock(&philo->data->last_meal);
	philo->last_meal_time = present_time();
	pthread_mutex_unlock(&philo->data->last_meal);
	pthread_mutex_unlock(first);
	pthread_mutex_unlock(second);
}

void ft_think(t_philo *philo)
{
	print_action(philo, "is thinking");
}

void *philos(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	if (philo->id % 2 != 0)
		usleep(500);
	while (check_death(philo->data))
	{
		if (philo->data->m_eat != -1 && philo->meals_eaten >= philo->data->m_eat)
		{
			pthread_mutex_lock(&philo->data->f);
			philo->data->full++;
			pthread_mutex_unlock(&philo->data->f);
			break;
		}
		ft_eat(philo);
		philo->meals_eaten++;
		ft_sleep(philo);
		ft_think(philo);
	}
	return (NULL);
}

void *monitor(void *arg)
{
	t_data *data;
	long	time;
	int		i;

	data = (t_data *)arg;
	while (check_death(data))
	{
		i = 0;
		while (i < data->b_philos)
		{
			pthread_mutex_lock(&data->f);
			if (data->full == data->b_philos)
				return (pthread_mutex_unlock(&data->f), NULL);
			(pthread_mutex_unlock(&data->f), pthread_mutex_lock(&data->philos[i].died));
			time = present_time();
			pthread_mutex_lock(&data->last_meal);
			if (time - data->philos[i].last_meal_time > data->t_die)
			{
				(pthread_mutex_unlock(&data->philos[i].died), pthread_mutex_lock(&data->p_lock));
				printf("%ld %d died\n", time - data->strt_time, data->philos[i].id);
				pthread_mutex_lock(&data->died);
				data->someone_died = 1;
				pthread_mutex_unlock(&data->died);
				pthread_mutex_unlock(&data->p_lock);
				pthread_mutex_unlock(&data->last_meal);
				return (NULL);
			}
			pthread_mutex_unlock(&data->last_meal);
			pthread_mutex_unlock(&data->philos[i].died);
			i++;
		}
	}
	return (NULL);
}

void life(t_data *ctx)
{
	int			i = 0;
	pthread_t	monitor_thread;

	ctx->strt_time = present_time();
	(pthread_mutex_init(&ctx->f, NULL), pthread_mutex_init(&ctx->last_meal, NULL));
	while (i < ctx->b_philos)
	{
		ctx->philos[i].last_meal_time = present_time();
		if (pthread_create(&ctx->philos[i].thread, NULL, philos, &ctx->philos[i]) != 0)
			perror("Failed to create thread");
		i++;
	}
	pthread_create(&monitor_thread, NULL, monitor, ctx);
	i = 0;
	while (i < ctx->b_philos)
		pthread_join(ctx->philos[i++].thread, NULL);
	pthread_join(monitor_thread, NULL);
}
