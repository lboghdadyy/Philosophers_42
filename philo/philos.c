
#include "philosopher.h"

long present_time(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	check_death(t_data *data)
{
	int	res;

	printf("here\n");
	pthread_mutex_lock(&data->died);
	res = !data->someone_died;
	pthread_mutex_unlock(&data->died);
	return (res); 
}

void	ft_usleep(long time, t_data *data)
{
	long	j;
	int		b;
	int		part;

	j = 0;
	b = 2;
	part = time;
	while (b < time)
	{
		if (time % b == 0)
		{
			part = (time / b);
			break ;
		}
		b++;
	}
	while (j < time && !data->someone_died)
	{
		usleep(part);
		j += part;
	}
}

void ft_sleep(t_philo *philo)
{
	if (!check_death(philo->data))
		return ;
	pthread_mutex_lock(&philo->data->p_lock);
	printf("%ld %d is sleeping\n", present_time() - philo->data->strt_time, philo->id);
	pthread_mutex_unlock(&philo->data->p_lock);
	ft_usleep(philo->data->t_sleep * 1000, philo->data);
}


void	ft_eat(t_philo *philo)
{
	if (!check_death(philo->data))
		return ;
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->data->p_lock);
	printf("%ld %d took a fork\n", present_time() - philo->data->strt_time, philo->id);
	pthread_mutex_unlock(&philo->data->p_lock);
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(&philo->data->p_lock);
	printf("%ld %d took a fork\n", present_time() - philo->data->strt_time, philo->id);
	printf("%ld %d is eating\n", present_time() - philo->data->strt_time, philo->id);
	pthread_mutex_unlock(&philo->data->p_lock);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	philo->last_meal_time = present_time();
	ft_usleep(philo->data->t_eat * 1000, philo->data);
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
				printf("%ld %d died\n", present - data->strt_time, data->philos[i].id);
				data->someone_died = 1;
				return NULL;
			}
			i++;
		}
		ft_usleep(1000, data);
	}
	return NULL;
}

int	check_must_eat(t_data *data, int *must_eat)
{
	if (data->m_eat == -1)
		return (0);
	if (data->m_eat != -1 && *must_eat < data->m_eat)
		return ((*must_eat)++, 0);
	else
		return (1);
}

void *philos(void *arg)
{
	t_philo	*philo = (t_philo *)arg;
	int		must_eat[200];

	must_eat[philo->id] = 0;
	while (check_death(philo->data))
	{
		if (check_must_eat(philo->data, must_eat + philo->id))
			break;
		ft_eat(philo);
		ft_sleep(philo);
		ft_think(philo);
	}
	return (NULL);
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
	// ft_usleep(1000, ctx);
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
