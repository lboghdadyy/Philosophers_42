CC = cc -fsanitize=address -g3
SRC = philosopher.c \
	utils/ft_atoi.c \
	utils/ft_putstrfd.c \
	utils/ft_strlen.c \
	utils/ft_isnum.c \
	philos.c \
	clean_up.c
RM=rm -rf
OBJ = $(SRC:.c=.o)
NAME = philosopher
HEADER = philosopher.h

$(NAME) : $(OBJ)
	$(CC) $(OBJ) -o $@

all : $(NAME)

%.o : %.c $(HEADER)
	$(CC) -c $< -o $@ 

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.phony : clean