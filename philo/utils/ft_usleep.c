/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaghdad <sbaghdad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:04:23 by sbaghdad          #+#    #+#             */
/*   Updated: 2025/07/07 12:09:00 by sbaghdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosopher.h"

void	ft_usleep(long time, t_data *data)
{
	long	start;

	start = present_time();
	while (check_death(data) && present_time() - start < time)
		usleep(100);
}

void	ft_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->f);
	if (philo->data->full == philo->data->n_ph)
	{
		pthread_mutex_unlock(&philo->data->f);
		return ;
	}
	pthread_mutex_unlock(&philo->data->f);
	print_action(philo, "is sleeping");
	ft_usleep(philo->data->t_sleep, philo->data);
}
