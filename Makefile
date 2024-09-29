CC		= cc
CFLAGS 	= -g -Wall -Wextra -Werror 
RM		= rm -rf
LDFLAGS = -pthread

SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

NAME	= philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(OBJSB)

fclean: 
	$(RM) $(NAME) $(BONUS)
	$(RM) $(OBJS) $(OBJSB)

re:
	$(RM) $(OBJS) $(OBJSB)

.PHONY: all bonus clean fclean re