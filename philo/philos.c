
#include "philosopher.h"

void *philos(void *arg)
{
	int id = *(int *)arg;
	for(int j = 0; j < 10; j++)
	{
		printf("this philosopher is here {%d} \n", id);
		sleep (1);
	}
	return NULL;
}

void life(t_data *ctx)
{
	int k = 0;

	while (k < ctx->b_philos)
	{
		pthread_create(ctx->philos[k].thread, NULL, &philos, ctx->philos + k);
		k++;
	}
	k = 0;
	while (k < ctx->b_philos)
	{
		pthread_join(ctx->philos[k].thread, NULL);
		k++;
	}
}
