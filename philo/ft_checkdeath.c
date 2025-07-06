/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_checkdeath.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaghdad <sbaghdad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:00:59 by sbaghdad          #+#    #+#             */
/*   Updated: 2025/07/06 19:10:06 by sbaghdad         ###   ########.fr       */
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
