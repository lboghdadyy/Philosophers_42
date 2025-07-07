/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaghdad <sbaghdad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:35:51 by sbaghdad          #+#    #+#             */
/*   Updated: 2025/07/07 12:41:09 by sbaghdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	ft_eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (one_philo(philo))
		return ;
	if (philo->id % 2 != 0)
		(1) && (first = philo->left_fork, second = philo->right_fork);
	else
		(1) && (first = philo->right_fork, second = philo->left_fork);
	if (!check_death(philo->data))
		return ;
	(pthread_mutex_lock(first), pthread_mutex_lock(second));
	take_forks(philo);
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->data->last_meal);
	philo->last_meal_time = present_time();
	pthread_mutex_unlock(&philo->data->last_meal);
	ft_usleep(philo->data->t_eat, philo->data);
	philo->m_eting++;
	pthread_mutex_lock(&philo->data->last_meal);
	philo->last_meal_time = present_time();
	pthread_mutex_unlock(&philo->data->last_meal);
	(pthread_mutex_unlock(first), pthread_mutex_unlock(second));
}

void	*philos(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 != 0)
		usleep(100);
	while (check_death(philo->data))
	{
		if (philo->data->m_eat != -1 \
			&& philo->m_eting >= philo->data->m_eat)
		{
			pthread_mutex_lock(&philo->data->f);
			philo->data->full++;
			pthread_mutex_unlock(&philo->data->f);
			break ;
		}
		ft_eat(philo);
		ft_sleep(philo);
		print_action(philo, "is thinking");
	}
	return (NULL);
}

int	check_philo_s(t_philo *philo)
{
	long	time;

	pthread_mutex_lock(&philo->data->f);
	if (philo->data->full == philo->data->n_ph)
		return (pthread_mutex_unlock(&philo->data->f), 1);
	pthread_mutex_unlock(&philo->data->f);
	pthread_mutex_lock(&philo->died);
	pthread_mutex_lock(&philo->data->p_l);
	pthread_mutex_lock(&philo->data->last_meal);
	time = present_time();
	if (time - philo->last_meal_time > philo->data->t_die)
	{
		pthread_mutex_unlock(&philo->died);
		printf("%ld %d died\n", time - philo->data->strt_time, philo->id);
		pthread_mutex_lock(&philo->data->died);
		philo->data->someone_died = 1;
		pthread_mutex_unlock(&philo->data->died);
		pthread_mutex_unlock(&philo->data->p_l);
		pthread_mutex_unlock(&philo->data->last_meal);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->p_l);
	pthread_mutex_unlock(&philo->data->last_meal);
	pthread_mutex_unlock(&philo->died);
	return (0);
}

void	*orgnize(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (check_death(data))
	{
		i = 0;
		while (i < data->n_ph)
		{
			if (check_philo_s(data->philos + i))
				return (NULL);
			i++;
		}
	}
	return (NULL);
}

void	life(t_data *c)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	c->strt_time = present_time();
	if (init(c))
		return ;
	while (i < c->n_ph)
	{
		c->philos[i].last_meal_time = present_time();
		if (pthread_create(&c->philos[i].thread, NULL, \
			philos, &c->philos[i]) != 0)
			ft_putstrfd("Failed to create thread", 2);
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, orgnize, c))
		ft_putstrfd("Failed to create thread", 2);
	i = 0;
	while (i < c->n_ph)
		pthread_join(c->philos[i++].thread, NULL);
	pthread_join(monitor_thread, NULL);
	finish(c);
}
