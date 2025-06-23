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

int	ft_init_philo(t_philo *philo, int ac, char **av)
{
	int	j;
	struct	timeval tv;

	j = 1;
	if (ac > 6 || ac < 5)
		return (ft_putstrfd("please enter a valid number\n", 2), 1);
	while (av[j])
	{
		if (check_number(av[1]))
			return (ft_putstrfd("please enter a valid number\n", 2), 1);
		j++;
	}
	gettimeofday(&tv, NULL);
	philo->data.start_time = tv.tv_sec;
	return (0);
}

int main(int ac, char **av)
{
	t_philo philo;
	
	if (ft_init_philo(&philo, ac, av))
		return (1);
	printf("%ld\n", philo.data.start_time);
	return (0);
}