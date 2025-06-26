
#include "philosopher.h"

long present_time(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000); // ms
}

int	check_death(t_data *data)
{
	int	res;

	pthread_mutex_lock(&data->died);
	res = !data->someone_died;
	pthread_mutex_unlock(&data->died);
	return (res); 
}

void ft_sleep(t_philo *philo)
{
	if (!check_death(philo->data))
		return ;
	pthread_mutex_lock(&philo->data->p_lock);
	printf("%ld %d is sleeping\n", present_time() - philo->data->strt_time, philo->id);
	pthread_mutex_unlock(&philo->data->p_lock);
	usleep(philo->data->t_sleep * 1000);
}

void	ft_eat(t_philo *philo)
{
	if (!check_death(philo->data))
		return ;
	pthread_mutex_lock(&philo->right_fork);
	pthread_mutex_lock(&philo->left_fork);
	pthread_mutex_lock(&philo->data->p_lock);
	printf("%ld %d has taken a fork\n", present_time() - philo->data->strt_time, philo->id);
	printf("%ld %d has taken a fork\n", present_time() - philo->data->strt_time, philo->id);
	printf("%ld %d is eating\n", present_time() - philo->data->strt_time, philo->id);
	pthread_mutex_unlock(&philo->data->p_lock);
	philo->last_meal_time = present_time();
	usleep(philo->data->t_eat * 1000);
	pthread_mutex_unlock(&philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
}

void ft_think(t_philo *philo)
{
	if (!check_death(philo->data))
		return ;

	pthread_mutex_lock(&philo->data->p_lock);
	printf("%ld %d is thinking\n",
		present_time() - philo->data->strt_time, philo->id);
	pthread_mutex_unlock(&philo->data->p_lock);
}

void *monitor(void *arg)
{
	t_data *data = (t_data *)arg; 
	long	present;
	int		i;

	while (check_death(data))
	{
		i = 0;
		while (i < data->b_philos)
		{
			present = present_time();
			if (present - data->philos[i].last_meal_time > data->t_die)
			{
				pthread_mutex_lock(&data->died);
				data->philos[i].death_himself = 1;
				data->someone_died = 1;
				pthread_mutex_unlock(&data->died);

				pthread_mutex_lock(&data->p_lock);
				printf("%ld %d has died\n",
					present - data->strt_time, data->philos[i].id);
				pthread_mutex_unlock(&data->p_lock);
				return NULL;
			}
			i++;
		}
		usleep(1000);
	}
	return NULL;
}

void *philos(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	philo->death_himself = 0;
	while (check_death(philo->data))
	{
		ft_eat(philo);
		ft_sleep(philo);
		ft_think(philo);
	}
	return NULL;
}

void life(t_data *ctx)
{
	int			k = 0;
	pthread_t	monitor_thread;

	pthread_mutex_init(&ctx->died, NULL);
	ctx->strt_time = present_time();
	while (k < ctx->b_philos)
	{
		ctx->philos[k].last_meal_time = present_time();
		if (ctx->philos[k].id % 2 == 0)
			pthread_create(&ctx->philos[k].thread, NULL, &philos, &ctx->philos[k]);
		k++;
	}
	usleep(1000);
	k = 0;
	while (k < ctx->b_philos)
	{
		ctx->philos[k].last_meal_time = present_time();
		if (ctx->philos[k].id % 2 != 0)
			pthread_create(&ctx->philos[k].thread, NULL, &philos, &ctx->philos[k]);
		k++;
	}

	pthread_create(&monitor_thread, NULL, &monitor, ctx);

	k = 0;
	while (k < ctx->b_philos)
	{
		pthread_join(ctx->philos[k].thread, NULL);
		k++;
	}
	pthread_join(monitor_thread, NULL);
}
