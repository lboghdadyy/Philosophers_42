/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaghdad <sbaghdad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:35:51 by sbaghdad          #+#    #+#             */
/*   Updated: 2025/07/06 19:50:01 by sbaghdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	one_philo(t_philo *philo)
{
	if (philo->data->n_ph == 1)
	{
		print_action(philo, "has taken fork");
		ft_usleep(philo->data->t_die, philo->data);	
		return (1);
	}
	return (0);
}

void	take_forks(t_philo *philo)
{
	long	time;
	
	pthread_mutex_lock(&philo->data->p_l);
	if (!philo->data->someone_died)
	{
		time = present_time();
		printf("%ld %d has taken a fork\n", \
			time - philo->data->strt_time, philo->id);
		printf("%ld %d has taken a fork\n", \
			time - philo->data->strt_time, philo->id);
	}
	pthread_mutex_unlock(&philo->data->p_l);
}

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
		return;
	(pthread_mutex_lock(first), pthread_mutex_lock(second));
	take_forks(philo);
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->data->last_meal);
	philo->last_meal_time = present_time();
	pthread_mutex_unlock(&philo->data->last_meal);
	ft_usleep(philo->data->t_eat, philo->data);
	pthread_mutex_lock(&philo->data->last_meal);
	philo->last_meal_time = present_time();
	pthread_mutex_unlock(&philo->data->last_meal);
	(pthread_mutex_unlock(first), pthread_mutex_unlock(second));
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

void *orgnize(void *arg)
{
	t_data *data;
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

int init(t_data *c)
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

void life(t_data *c)
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
		if (pthread_create(&c->philos[i].thread, NULL, philos, &c->philos[i]) != 0)
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
