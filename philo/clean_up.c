#include "philosopher.h"

void    finish(t_data *data)
{
    int k;

    k = 0;
    while (k < data->b_philos)
    {
        pthread_mutex_destroy(&data->forks[k]);
        pthread_mutex_destroy(&data->philos[k].died);
        k++;
    }
    pthread_mutex_destroy(&data->p_lock);
    pthread_mutex_destroy(&data->died);
    free(data->philos);
    free(data->forks);
}