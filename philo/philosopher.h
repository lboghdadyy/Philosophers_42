/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaghdad <sbaghdad@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 14:22:06 by sbaghdad          #+#    #+#             */
/*   Updated: 2025/06/23 22:07:40 by sbaghdad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

# include <stdio.h>
# include <unistd.h> 
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo t_philo;

typedef struct s_data {
	int				b_philos;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				m_eat;
	long			s_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	p_lock;
	int 			someone_died;
	t_philo			*philos;
}	t_data;

typedef struct s_philo {
	int 			*id;
	pthread_t		*thread;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	long 			last_meal_time;
	int				meals_eaten;
	t_data			data;
}		t_philo;

int		ft_atoi(char *s);
void	ft_putstrfd(char *s, int fd);
size_t	ft_strlen(char *str);
int 	ft_isnum(char c);
void    life(t_philo  *ctx);

#endif