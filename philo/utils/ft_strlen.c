#include "../philosopher.h"

size_t	ft_strlen(char *str)
{
	size_t	j;

	j = 0;
	if (!str)
		return (0);
	while (str[j])
		j++;
	return (j);
}