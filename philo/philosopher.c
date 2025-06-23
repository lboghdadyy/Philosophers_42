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

int	check_arguments(t_data *data, int ac, char **av)
{
	int	j;

	j = 1;
	if (ac > 6 || ac < 5)
		return (ft_putstrfd("please enter a valid arguments\n", 2), 1);
	while (av[j])
	{
		if (check_number(av[1]))
			return (ft_putstrfd("please enter a valid number\n", 2), 1);
		j++;
	}
	data->b_philos = ft_atoi(av[1]);
	data->t_die = ft_atoi(av[2]);
	data->t_eat = ft_atoi(av[3]);
	if (av[4])
		data->t_eat = ft_atoi(av[4]);
	else
		data->t_eat = 0;
	data->philos = malloc(sizeof(t_philo) * data->b_philos);
	j = 0;
	while (j < data->b_philos)
	{
		data->philos[j].id = j + 1;
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
