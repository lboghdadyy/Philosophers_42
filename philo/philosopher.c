#include "philosopher.h"

int	check_number(char *string)
{
	int j;

	j = 0;
	while (string[j])
	{
		if (!ft_isnum(string[j]))
			return (1);
		j++;
	}
	return (0);
}

void destroy_mutexes(t_data *data, int up_to_index)
{
	int	i;
	
	i = 0;
	pthread_mutex_destroy(&data->p_lock);
	pthread_mutex_destroy(&data->died);
	while (i < up_to_index)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].died);
		i++;
	}
}

int	check_arguments(t_data *data, int ac, char **av)
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
	(1) && (data->b_philos = ft_atoi(av[1]), data->t_die = ft_atoi(av[2]));
	(1) && (data->t_eat = ft_atoi(av[3]), data->t_sleep = ft_atoi(av[4]));
	if (ac == 6)
		data->m_eat = ft_atoi(av[5]);
	else
		data->m_eat = -1;
	if (data->b_philos <= 0 || data->t_die <= 0 || data->t_eat <= 0 || data->t_sleep <= 0)
		return (ft_putstrfd("arguments must be greater than zero\n", 2), 1);
	data->someone_died = 0;
	data->s_time = data->strt_time;
	data->philos = malloc(sizeof(t_philo) * data->b_philos);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->b_philos);
	if (!data->philos || !data->forks)
		return (ft_putstrfd("malloc failed\n", 2), 1);
	if (pthread_mutex_init(&data->p_lock, NULL) != 0)
		return (free(data->philos), free(data->forks), ft_putstrfd("Error : pthread_mutex_init\n", 2), 1);
	if (pthread_mutex_init(&data->died, NULL))
		return (free(data->forks), free(data->philos), pthread_mutex_destroy(&data->p_lock), ft_putstrfd("Error : pthread_mutex_init\n", 2), 1);
	j = 0;
	while (j < data->b_philos)
	{
		data->philos[j].id = j + 1;
		data->philos[j].data = data;
		data->philos[j].death_himself = 0;
		data->philos[j].meals_eaten = 0;
		pthread_mutex_init(&data->forks[j], NULL);
		pthread_mutex_init(&data->philos[j].died, NULL);
		data->philos[j].left_fork = data->forks + j;
		data->philos[j].right_fork = data->forks + (j + 1) % data->b_philos;
		j++;
	}
	return (0);
}

int main(int ac, char **av)
{
	t_data		ctx;

	if (check_arguments(&ctx, ac, av))
		return (1);
	life(&ctx);
	return (0);
}
