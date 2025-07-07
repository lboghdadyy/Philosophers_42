CC = cc -Wall -Wextra -Werror -fsanitize=thread

SRC = philosopher.c \
	utils/ft_atoi.c \
	utils/ft_putstrfd.c \
	utils/ft_strlen.c \
	utils/ft_isnum.c \
	philos.c \
	clean_up.c \
	ft_checkdeath.c \
    utils/ft_usleep.c \
	init_philos.c

RM=rm -rf

OBJ = $(SRC:.c=.o)

NAME = philo

HEADER = philosopher.h

CFLAGS = -Wall -Wextra -Werror

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

all : $(NAME)

%.o : %.c $(HEADER) Makefile
	$(CC) $(CFLAGS) -c $< -o $@ 

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.phony : clean