/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 11:40:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ast_print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
}

static void	ast_print_args(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
	{
		printf(" %s", args[i]);
		i++;
	}
}

static void	ast_print_type(t_ast *root)
{
	if (root->type == TOKEN_COMMAND)
	{
		printf(BOLD GREEN "CMD:" RESET);
		if (root->args)
			ast_print_args(root->args);
		printf("\n");
	}
	else if (root->type == TOKEN_PIPE)
		printf(YELLOW "PIPE\n" RESET);
	else if (root->type == TOKEN_REDIRECT_IN)
		printf(BOLD RED "REDIRECT_IN (<): %s\n" RESET, root->file);
	else if (root->type == TOKEN_REDIRECT_OUT)
		printf(BOLD RED "REDIRECT_OUT (>): %s\n" RESET, root->file);
	else if (root->type == TOKEN_HEREDOC)
		printf(MAGENTA "HEREDOC (<<): %s\nCONTENT:\n%s\n" RESET, \
			root->file, root->heredoc_content);
	else if (root->type == TOKEN_APPEND)
		printf(MAGENTA "APPEND (>>): %s\n" RESET, root->file);
	else
		printf(RED "UNKNOWN NODE\n" RESET);
}

static void	ast_print_branch(char *label, t_ast *child, int depth)
{
	ast_print_indent(depth);
	printf(CYAN "%s →\n" RESET, label);
	if (child)
		ast_print(child, depth + 1);
	else
	{
		ast_print_indent(depth + 1);
		printf("[NULL]\n");
	}
}

void	ast_print(t_ast *root, int depth)
{
	if (!root)
		return ;
	ast_print_indent(depth);
	ast_print_type(root);
	ast_print_branch("LEFT", root->left, depth);
	ast_print_branch("RIGHT", root->right, depth);
}
