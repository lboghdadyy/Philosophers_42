/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_checkdeath.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaghdad <sbaghdad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:00:59 by sbaghdad          #+#    #+#             */
/*   Updated: 2025/07/07 12:41:23 by sbaghdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	check_death(t_data *data)
{
	int	res;

	pthread_mutex_lock(&data->died);
	res = !data->someone_died;
	pthread_mutex_unlock(&data->died);
	return (res);
}

void	print_action(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->p_l);
	if (!check_death(philo->data))
	{
		pthread_mutex_unlock(&philo->data->p_l);
		return ;
	}
	if (philo->data->m_eat != -1 && philo->m_eting >= philo->data->m_eat)
	{
		pthread_mutex_unlock(&philo->data->p_l);
		return ;
	}
	printf("%ld %d %s\n", present_time() - philo->data->strt_time, \
	philo->id, msg);
	pthread_mutex_unlock(&philo->data->p_l);
}

long	present_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
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
