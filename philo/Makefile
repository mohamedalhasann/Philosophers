NAME	= philo

CC		= cc
FLAGS	= -pthread -Wall -Wextra -Werror

SRCS	= srcs/philo.c srcs/philo_utils.c
OBJS	= $(SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.c philosophers.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
