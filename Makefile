CC				=	cc
FLAGS			=	-g -Wextra -Werror -Wall --sanitize=address
RM				=	rm -f
INCLUDES		=	-I.

NAME			=	minishell
PROG_SRC		=	minishell.c
PROG_OBJ		=	$(PROG_SRC:.c=.o)

UTILS_SRC       =	parse_utils.c
UTILS_OBJ       =	$(UTILS_SRC:.c=.o)

HEADERS			=	minishell.h
HEADERS_UTILS	=	

PARAMS			= 

all: 	$(NAME)

$(NAME):	$(PROG_OBJ) $(UTILS_OBJ) $(HEADERS) $(HEADERS_UTILS)
	$(CC) $(FLAGS) $(PROG_OBJ) $(UTILS_OBJ) -o $(NAME)
	
%.o: %.c Makefile $(HEADERS) $(HEADERS_UTILS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(PROG_OBJ) $(UTILS_OBJ)

fclean:	clean
	$(RM) $(NAME)

re:		fclean all

run: $(NAME)
	./$(NAME) $(PARAMS)

valgrind1: $(NAME)
#	For memory leaks and invalid accesses:
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) $(PARAMS)

.PHONY:	all clean fclean re run valgrind1 


# source ~/norminette_env/bin/activate