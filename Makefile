NAME	= philo

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

SRCS	= srcs/philo.c srcs/philo_utils.c
OBJS	= $(SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c philosophers.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
