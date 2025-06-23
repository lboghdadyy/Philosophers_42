#include "../philosopher.h"

void	ft_putstrfd(char *s, int fd)
{
	if (s)
		write(fd, s, ft_strlen(s));
}