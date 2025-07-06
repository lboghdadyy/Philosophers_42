/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaghdad <sbaghdad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:07:21 by sbaghdad          #+#    #+#             */
/*   Updated: 2025/07/06 19:10:39 by sbaghdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	check_number(char *string)
{
	int	j;

	j = 0;
	while (string[j])
	{
		if (!ft_isnum(string[j]))
			return (1);
		j++;
	}
	return (0);
}

void	destroy_mutexes(t_data *data, int up_to_index)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->p_l);
	pthread_mutex_destroy(&data->died);
	while (i < up_to_index)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].died);
		i++;
	}
}

int	check_args(int ac, char **av)
{
	int	j;

	if (ac < 5 || ac > 6)
		return (ft_putstrfd("please enter valid arguments\n", 2), 1);
	j = 1;
	while (av[j])
	{
		if (check_number(av[j]))
			return (ft_putstrfd("please enter valid numbers\n", 2), 1);
		j++;
	}
	return (0);
}

int	init_data(t_data *data, int ac, char **av)
{
	if (check_args(ac, av))
		return (1);
	(1) && (data->n_ph = ft_atoi(av[1]), data->t_die = ft_atoi(av[2]));
	(1) && (data->t_eat = ft_atoi(av[3]), data->t_sleep = ft_atoi(av[4]));
	if (ac == 6)
		data->m_eat = ft_atoi(av[5]);
	else
		data->m_eat = -1;
	if (data->n_ph <= 0 || data->t_die <= 0 \
		|| data->t_eat <= 0 || data->t_sleep <= 0 || data->m_eat == 0)
		return (ft_putstrfd("arguments must be greater than zero\n", 2), 1);
	data->someone_died = 0;
	if (alloc_philos(data))
		return (1);
	if (init_philos(data))
		return (free(data->philos), free(data->forks), 1);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	ctx;

	if (init_data(&ctx, ac, av))
		return (1);
	life(&ctx);
	return (0);
}
