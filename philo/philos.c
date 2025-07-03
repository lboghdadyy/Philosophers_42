#include "philosopher.h"

long present_time(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	check_death(t_data *data)
{
	int	res;

	pthread_mutex_lock(&data->died);
	res = !data->someone_died;
	pthread_mutex_unlock(&data->died);
	return (res);
}

void	ft_usleep(long time, t_data *data)
{
	long	start = present_time();
	while (check_death(data) && present_time() - start < time)
		usleep(100);
}

int	is_dead(t_philo *philo)
{
	int	res;

	pthread_mutex_lock(&philo->died);
	res = philo->death_himself;
	pthread_mutex_unlock(&philo->died);
	return (res);
}

void print_action(t_philo *philo, char *msg)
{
	if (!check_death(philo->data) || is_dead(philo))
		return ;
	pthread_mutex_lock(&philo->data->p_lock);
	printf("%ld %d %s\n", present_time() - philo->data->strt_time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->p_lock);
}

void ft_sleep(t_philo *philo)
{
	print_action(philo, "is sleeping");
	ft_usleep(philo->data->t_sleep, philo->data);
}

void	ft_eat(t_philo *philo)
{
	pthread_mutex_t *first;
	pthread_mutex_t *second;

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
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(second);
	print_action(philo, "has taken a fork");
	print_action(philo, "is eating");
	philo->last_meal_time = present_time();
	ft_usleep(philo->data->t_eat, philo->data);
	philo->last_meal_time = present_time();
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

	if (philo->id % 2 == 0)
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
	t_data *data = (t_data *)arg;
	long	time;
	int		i;

	while (check_death(data))
	{
		i = 0;
		while (i < data->b_philos)
		{
			pthread_mutex_lock(&data->f);
			if (data->full == data->b_philos)
				return (pthread_mutex_unlock(&data->f), NULL);
			pthread_mutex_unlock(&data->f);
			pthread_mutex_lock(&data->philos[i].died);
			time = present_time();
			if (time - data->philos[i].last_meal_time > data->t_die)
			{
				pthread_mutex_unlock(&data->philos[i].died);
				pthread_mutex_lock(&data->p_lock);
				printf("%ld %d died\n", time - data->strt_time, data->philos[i].id);
				data->someone_died = 1;
				pthread_mutex_unlock(&data->p_lock);
				return (NULL);
			}
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
	pthread_mutex_init(&ctx->f, NULL);
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
