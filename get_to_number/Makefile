##
## Makefile
## 

NAME		= genetic_algorithm
SRCS		= src/main.cpp
OBJS		= $(SRCS:.cpp=.o)

CC		= g++
CLIBS		= 
INCL_PATH	= ./include/
CFLAGS		= -W -Wall -Werror -I$(INCL_PATH)
RM		= rm -f

all		: $(OBJS)
		$(CC) -o $(NAME) $(OBJS) $(CLIBS)

$(NAME)		: all

clean		:
		$(RM) *~ ./src/*~ $(INCL_PATH)*~ $(OBJS)

fclean		: clean
		$(RM) $(NAME)

re		: fclean all

.PHONY		: all clean fclean re
