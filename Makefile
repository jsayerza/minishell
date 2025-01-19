CC				=	cc
FLAGS			=	-g -Wextra -Werror -Wall #--sanitize=address
RM				=	rm -f
INCLUDES		=	-I. -Ilibft

NAME			=	minishell
PROG_SRC		=	minishell.c parse_lexer.c parse_lexer_funcs.c \
					tokens.c collector.c
PROG_OBJ		=	$(PROG_SRC:.c=.o)

UTILS_SRC       =	
UTILS_OBJ       =	$(UTILS_SRC:.c=.o)

LIBFT_DIR       =	libft
LIBFT           =	$(LIBFT_DIR)/libft.a

HEADERS			=	minishell.h

PARAMS			= 

all:	$(LIBFT) $(NAME)

$(LIBFT):
	@echo "Construint libft..."
	$(MAKE) -C $(LIBFT_DIR)

$(NAME):	$(PROG_OBJ) $(UTILS_OBJ) $(HEADERS) $(LIBFT)
#	@echo "Construint $(NAME)..."
	$(CC) $(FLAGS) $(INCLUDES) $(PROG_OBJ) $(UTILS_OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)	
	
%.o: %.c Makefile $(HEADERS)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Eliminant object files..."
	$(MAKE) clean -C $(LIBFT_DIR)
	$(RM) $(PROG_OBJ) $(UTILS_OBJ)

fclean:	clean
	@echo "Eliminant $(NAME) i libft.a..."
	$(MAKE) fclean -C $(LIBFT_DIR)
	$(RM) $(NAME)

re:		fclean all

run: $(NAME)
	./$(NAME) $(PARAMS)

valgrind1: $(NAME)
#	For memory leaks and invalid accesses:
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) $(PARAMS)

.PHONY:	all clean fclean re run valgrind1 


# source ~/norminette_env/bin/activate