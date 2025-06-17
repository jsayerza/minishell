CC				=	cc
FLAGS			=	-g -Wextra -Werror -Wall
USE_READLINE	=	1
ifeq ($(USE_READLINE), 1)
	READLINE_LIB = -lreadline
else
	READLINE_LIB =
endif
RM				=	rm -f
INCLUDES		=	-I. -Ilibft -Ignl
EXEC_DIR		=	executor
NAME			=	minishell
PROG_SRC		=	minishell.c minishell_funcs.c \
					collector.c prompt.c\
					utils.c utils_str.c \
					lexer/lexer.c lexer/lexer_funcs.c \
					lexer/lexer_funcs_operator.c \
					lexer/lexer_funcs_handle.c \
					lexer/lexer_expand.c \
					lexer/lexer_expand_var.c \
					lexer/lexer_expand_str.c \
					lexer/lexer_expand_quotes.c \
					lexer/lexer_expand_quotes_funcs.c \
					lexer/lexer_expand_ass_quote.c \
					lexer/tokens.c lexer/tokens_funcs.c \
					lexer/tokens_funcs_insert.c \
					parser/parser.c parser/parser_nodes.c \
					parser/parser_pipeline.c \
					parser/parser_utils.c parser/parser_funcs.c \
					parser/parser_utils_token_invalid.c \
					parser/parser_heredoc.c \
					parser/parser_heredoc_should_break.c \
					parser/parser_ast_print.c \
					parser/parser_ast_print_type.c \
					constructor/constructor.c \
					constructor/constructor_funcs.c \
					constructor/constructor_add_redirs.c \
					constructor/constructor_node_create.c \
					constructor/constructor_cmd_node_create.c \
					constructor/constructor_print.c

EXEC_SRC =	$(EXEC_DIR)/init_shell.c \
			$(EXEC_DIR)/copy_envv.c \
			$(EXEC_DIR)/env_to_export.c \
			$(EXEC_DIR)/create_export.c \
			$(EXEC_DIR)/display_shell.c \
			$(EXEC_DIR)/builtins.c \
			$(EXEC_DIR)/export.c \
			$(EXEC_DIR)/export_utils00.c \
			$(EXEC_DIR)/export_utils01.c \
			$(EXEC_DIR)/export_utils02.c \
			$(EXEC_DIR)/export_utils03.c \
			$(EXEC_DIR)/env.c \
			$(EXEC_DIR)/env_utils00.c \
			$(EXEC_DIR)/pwd.c \
			$(EXEC_DIR)/echo.c \
			$(EXEC_DIR)/prints.c \
			$(EXEC_DIR)/unset.c \
			$(EXEC_DIR)/unset_export.c \
			$(EXEC_DIR)/path.c \
			$(EXEC_DIR)/path_utils00.c \
			$(EXEC_DIR)/path_utils01.c \
			$(EXEC_DIR)/cd.c \
			$(EXEC_DIR)/cd_utils00.c \
			$(EXEC_DIR)/cd_utils01.c \
			$(EXEC_DIR)/exit.c \
			$(EXEC_DIR)/commands.c \
			$(EXEC_DIR)/commands_utils00.c \
			$(EXEC_DIR)/commands_utils01.c \
			$(EXEC_DIR)/commands_utils02.c \
			$(EXEC_DIR)/redirects.c \
			$(EXEC_DIR)/redirects_utils00.c \
			$(EXEC_DIR)/redirects_utils01.c \
			$(EXEC_DIR)/signals.c \
			$(EXEC_DIR)/heredoc.c \
			$(EXEC_DIR)/acces_path.c \
			$(EXEC_DIR)/acces_path_utils00.c \
			$(EXEC_DIR)/wait_children.c \
			$(EXEC_DIR)/clean_shell.c

SRC_GNL =	gnl/get_next_line.c

PROG_OBJ		=	$(PROG_SRC:.c=.o)
EXEC_OBJ		=	$(EXEC_SRC:.c=.o)

UTILS_SRC       =
UTILS_OBJ       =	$(UTILS_SRC:.c=.o)

OBJ_GNL			=	$(SRC_GNL:.c=.o)

LIBFT_DIR       =	libft
LIBFT           =	$(LIBFT_DIR)/libft.a

HEADERS			=	minishell.h gnl/get_next_line.h

#PARAMS			= "nonexistent_command"
#PARAMS			= "echo \"$USER\" || $$USER$ 'hello & *world' | *** grep hello >> output*.txt && < test.txt"
#PARAMS			= "echo $USER | grep jsay >> output*.txt"
PARAMS			= < heredoc_test.txt


all:	$(LIBFT) $(NAME)

$(LIBFT):
	@echo "Construint libft..."
	$(MAKE) -C $(LIBFT_DIR)

$(NAME):	$(PROG_OBJ) $(EXEC_OBJ) $(UTILS_OBJ) $(HEADERS) $(LIBFT) $(OBJ_GNL)
#	@echo "Construint $(NAME)..."
	$(CC) $(FLAGS) $(INCLUDES) $(PROG_OBJ) $(EXEC_OBJ) $(UTILS_OBJ) $(OBJ_GNL) -L$(LIBFT_DIR) -lft $(READLINE_LIB) -o $(NAME)

%.o: %.c Makefile $(HEADERS)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Eliminant object files..."
	$(MAKE) clean -C $(LIBFT_DIR)
	$(MAKE) clean -C gnl

	$(RM) $(PROG_OBJ) $(EXEC_OBJ) $(UTILS_OBJ)

fclean:	clean
	@echo "Eliminant $(NAME) i libft.a..."
	$(MAKE) fclean -C $(LIBFT_DIR)
	$(MAKE) fclean -C gnl
	$(RM) $(NAME)

re:		fclean all

run: $(NAME)
	./$(NAME) $(PARAMS)

valgrind1: $(NAME)
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) $(PARAMS)

.PHONY: all clean fclean re run valgrind1 pretty run_pretty

# source ~/norminette_env/bin/activate
