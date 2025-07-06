/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaghdad <sbaghdad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 14:22:06 by sbaghdad          #+#    #+#             */
/*   Updated: 2025/07/06 19:10:39 by sbaghdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <stdio.h>
# include <unistd.h> 
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;

typedef struct s_data {
	int				n_ph;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				full;
	pthread_mutex_t	f;
	int				m_eat;
	pthread_mutex_t	*forks;
	pthread_mutex_t	p_l;
	int				someone_died;
	pthread_mutex_t	died;
	t_philo			*philos;
	long			strt_time;
	pthread_mutex_t	last_meal;
}	t_data;

typedef struct s_philo {
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	died;
	long			last_meal_time;
	int				meals_eaten;
	int				death_himself;
	t_data			*data;
}		t_philo;

int		ft_atoi(char *s);
void	ft_putstrfd(char *s, int fd);
size_t	ft_strlen(char *str);
int		ft_isnum(char c);
void	life(t_data *ctx);
long	present_time(void);
void	finish(t_data *data);
void	ft_usleep(long time, t_data *data);
void	ft_sleep(t_philo *philo);
int		check_death(t_data *data);
void	print_action(t_philo *philo, char *msg);
long	present_time(void);
int		init_philos(t_data *data);
int		alloc_philos(t_data *data);
void	clean_philos(t_data *data, int index);

#endif