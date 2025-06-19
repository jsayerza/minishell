/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Makefile                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 12:00:00 by acarranz          #+#    #+#             */
/*   Updated: 2025/06/07 10:39:58 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
USE_READLINE = 1
ifeq ($(USE_READLINE), 1)
	READLINE_LIB = -lreadline
else
	READLINE_LIB =
endif
RM = rm -f

# Directories
SRCDIR = src
INCDIR = includes
LIBFTDIR = libft
OBJDIR = obj

# Include paths
INCLUDES = -I$(INCDIR) -I$(LIBFTDIR) -I. -Ilibft

# Target
NAME = minishell

# Library
LIBFT = $(LIBFTDIR)/libft.a

# Source files
MAIN_SRC = $(SRCDIR)/main/minishell.c \
           $(SRCDIR)/main/minishell_funcs.c \
           $(SRCDIR)/main/collector.c \
           $(SRCDIR)/main/prompt.c \
           $(SRCDIR)/main/utils.c \
           $(SRCDIR)/main/utils_str.c

LEXER_SRC = $(SRCDIR)/lexer/lexer.c \
            $(SRCDIR)/lexer/lexer_funcs.c \
            $(SRCDIR)/lexer/lexer_funcs_operator.c \
            $(SRCDIR)/lexer/lexer_funcs_handle.c \
            $(SRCDIR)/lexer/lexer_expand.c \
            $(SRCDIR)/lexer/lexer_expand_var.c \
            $(SRCDIR)/lexer/lexer_expand_str.c \
            $(SRCDIR)/lexer/lexer_expand_quotes.c \
            $(SRCDIR)/lexer/lexer_expand_quotes_funcs.c \
            $(SRCDIR)/lexer/lexer_expand_ass_quote.c \
            $(SRCDIR)/lexer/tokens.c \
            $(SRCDIR)/lexer/tokens_funcs.c \
            $(SRCDIR)/lexer/tokens_funcs_insert.c

PARSER_SRC = $(SRCDIR)/parser/parser.c \
             $(SRCDIR)/parser/parser_nodes.c \
             $(SRCDIR)/parser/parser_pipeline.c \
             $(SRCDIR)/parser/parser_utils.c \
             $(SRCDIR)/parser/parser_funcs.c \
             $(SRCDIR)/parser/parser_utils_token_invalid.c \
             $(SRCDIR)/parser/parser_heredoc.c \
             $(SRCDIR)/parser/parser_heredoc_should_break.c \
             $(SRCDIR)/parser/parser_ast_print.c \
             $(SRCDIR)/parser/parser_ast_print_type.c

CONSTRUCTOR_SRC = $(SRCDIR)/constructor/constructor.c \
                  $(SRCDIR)/constructor/constructor_funcs.c \
                  $(SRCDIR)/constructor/constructor_add_redirs.c \
                  $(SRCDIR)/constructor/constructor_node_create.c \
                  $(SRCDIR)/constructor/constructor_cmd_node_create.c \
                  $(SRCDIR)/constructor/constructor_print.c

EXECUTOR_SRC = $(SRCDIR)/executor/commands.c \
               $(SRCDIR)/executor/commands_utils00.c \
               $(SRCDIR)/executor/commands_utils01.c \
               $(SRCDIR)/executor/commands_utils02.c \
               $(SRCDIR)/executor/display_shell.c \
               $(SRCDIR)/executor/signals.c \
               $(SRCDIR)/executor/wait_children.c

BUILTINS_SRC = $(SRCDIR)/builtins/builtins.c \
               $(SRCDIR)/builtins/cd.c \
               $(SRCDIR)/builtins/cd_utils00.c \
               $(SRCDIR)/builtins/cd_utils01.c \
               $(SRCDIR)/builtins/copy_envv.c \
               $(SRCDIR)/builtins/create_export.c \
               $(SRCDIR)/builtins/echo.c \
               $(SRCDIR)/builtins/env.c \
               $(SRCDIR)/builtins/env_to_export.c \
               $(SRCDIR)/builtins/env_utils00.c \
               $(SRCDIR)/builtins/exit.c \
               $(SRCDIR)/builtins/export.c \
               $(SRCDIR)/builtins/export_utils00.c \
               $(SRCDIR)/builtins/export_utils01.c \
               $(SRCDIR)/builtins/export_utils02.c \
               $(SRCDIR)/builtins/export_utils03.c \
               $(SRCDIR)/builtins/pwd.c \
               $(SRCDIR)/builtins/unset.c \
               $(SRCDIR)/builtins/unset_export.c

REDIRECTS_SRC = $(SRCDIR)/redirects/heredoc.c \
                $(SRCDIR)/redirects/redirects.c \
                $(SRCDIR)/redirects/redirects_utils00.c \
                $(SRCDIR)/redirects/redirects_utils01.c

UTILS_SRC = $(SRCDIR)/utils/acces_path.c \
            $(SRCDIR)/utils/acces_path_utils00.c \
            $(SRCDIR)/utils/clean_shell.c \
            $(SRCDIR)/utils/init_shell.c \
            $(SRCDIR)/utils/path.c \
            $(SRCDIR)/utils/path_utils00.c \
            $(SRCDIR)/utils/path_utils01.c \
            $(SRCDIR)/utils/prints.c

# All source files
SRC = $(MAIN_SRC) $(LEXER_SRC) $(PARSER_SRC) $(CONSTRUCTOR_SRC) \
      $(EXECUTOR_SRC) $(BUILTINS_SRC) $(REDIRECTS_SRC) $(UTILS_SRC)

# Object files
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Headers
HEADERS = $(INCDIR)/minishell.h $(LIBFTDIR)/libft.h

# Colors for output
RED = \033[31m
BLUE = \033[34m
YELLOW = \033[33m
CYAN = \033[36m
WHITE = \033[37m
NC = \033[0m # No Color

# Rules
.PHONY: all clean fclean re info

all: $(NAME)
$(NAME): $(LIBFT) $(OBJ) Makefile
	@echo "$(CYAN)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJ) -L$(LIBFTDIR) -lft $(READLINE_LIB) -o $(NAME)
	@echo "$(YELLOW)$(NAME) creado con éxito!$(NC)"

$(LIBFT): $(shell find $(LIBFTDIR) -name "*.c" -o -name "*.h")
	@echo "$(CYAN)[LIBFT] Building libft...$(NC)"
	@$(MAKE) -C $(LIBFTDIR) --no-print-directory

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) Makefile | $(OBJDIR)
	@mkdir -p $(dir $@)
	@if echo "$<" | grep -q "/main/"; then \
		printf "$(CYAN)[MAIN]$(NC) "; \
	elif echo "$<" | grep -q "/lexer/"; then \
		printf "$(CYAN)[LEXER]$(NC) "; \
	elif echo "$<" | grep -q "/parser/"; then \
		printf "$(CYAN)[PARSER]$(NC) "; \
	elif echo "$<" | grep -q "/constructor/"; then \
		printf "$(CYAN)[CONSTRUCTOR]$(NC) "; \
	elif echo "$<" | grep -q "/executor/"; then \
		printf "$(CYAN)[EXECUTOR]$(NC) "; \
	elif echo "$<" | grep -q "/builtins/"; then \
		printf "$(CYAN)[BUILTINS]$(NC) "; \
	elif echo "$<" | grep -q "/redirects/"; then \
		printf "$(CYAN)[REDIRECTS]$(NC) "; \
	elif echo "$<" | grep -q "/utils/"; then \
		printf "$(CYAN)[UTILS]$(NC) "; \
	fi; \
	printf "$(WHITE)$<$(NC)\n"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/main
	@mkdir -p $(OBJDIR)/lexer
	@mkdir -p $(OBJDIR)/parser
	@mkdir -p $(OBJDIR)/constructor
	@mkdir -p $(OBJDIR)/executor
	@mkdir -p $(OBJDIR)/builtins
	@mkdir -p $(OBJDIR)/redirects
	@mkdir -p $(OBJDIR)/utils

clean:
	@$(MAKE) clean -C $(LIBFTDIR) --no-print-directory
	@$(RM) -r $(OBJDIR)

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) fclean -C $(LIBFTDIR) --no-print-directory

re: fclean all

# Debug info
info:
	@echo "$(YELLOW)Project structure:$(NC)"
	@echo "Source files: $(words $(SRC))"
	@echo "Object files: $(words $(OBJ))"
	@echo "Include paths: $(INCLUDES)"
	@echo ""
	@echo "$(YELLOW)Modules breakdown:$(NC)"
	@echo "$(YELLOW)  MAIN:$(NC) $(words $(MAIN_SRC)) files"
	@echo "$(BLUE)  LEXER:$(NC) $(words $(LEXER_SRC)) files"
	@echo "$(BLUE)  PARSER:$(NC) $(words $(PARSER_SRC)) files"
	@echo "$(BLUE)  CONSTRUCTOR:$(NC) $(words $(CONSTRUCTOR_SRC)) files"
	@echo "$(BLUE)  EXECUTOR:$(NC) $(words $(EXECUTOR_SRC)) files"
	@echo "$(BLUE)  BUILTINS:$(NC) $(words $(BUILTINS_SRC)) files"
	@echo "$(BLUE)  REDIRECTS:$(NC) $(words $(REDIRECTS_SRC)) files"
	@echo "$(BLUE)  UTILS:$(NC) $(words $(UTILS_SRC)) files"
