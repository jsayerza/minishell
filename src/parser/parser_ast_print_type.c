/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_print_type.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 11:40:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/22 17:45:41 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ast_print_str_array(char *label, char **array, int depth)
{
	int	i;

	if (!array)
	{
		ast_print_indent(depth);
		printf("%s: [NULL]\n", label);
		return ;
	}
	ast_print_indent(depth);
	printf("%s:\n", label);
	i = 0;
	while (array[i])
	{
		ast_print_indent(depth + 1);
		printf("- %s\n", array[i]);
		i++;
	}
}

static void	ast_print_heredoc(t_ast *root, int depth)
{
	ast_print_indent(depth);
	printf(MAGENTA "HEREDOC (<<): %s\n" RESET, root->file);
	ast_print_indent(depth);
	printf("CONTENT:\n");
	ast_print_indent(depth + 1);
	if (root->heredoc_content)
		printf("%s\n", root->heredoc_content);
	else
		printf("[NULL]\n");
}

static void	ast_print_redirect_file(t_ast *root, int depth)
{
	if (root->file
		&& (root->type == TOKEN_REDIRECT_IN || root->type == TOKEN_REDIRECT_OUT
			|| root->type == TOKEN_APPEND))
	{
		ast_print_indent(depth);
		printf("file: %s\n", root->file);
	}
}

static void	ast_print_node_type(t_ast *root)
{
	if (root->type == TOKEN_COMMAND)
		printf(BOLD GREEN "CMD\n" RESET);
	else if (root->type == TOKEN_WORD)
		printf(BOLD GREEN "WORD\n" RESET);
	else if (root->type == TOKEN_PIPE)
		printf(YELLOW "PIPE\n" RESET);
	else if (root->type == TOKEN_REDIRECT_IN)
		printf(BOLD RED "REDIRECT_IN (<)\n" RESET);
	else if (root->type == TOKEN_REDIRECT_OUT)
		printf(BOLD RED "REDIRECT_OUT (>)\n" RESET);
	else if (root->type == TOKEN_APPEND)
		printf(MAGENTA "APPEND (>>)\n" RESET);
	else
		printf(RED "UNKNOWN NODE\n" RESET);
}

void	ast_print_type(t_ast *root, int depth)
{
	ast_print_indent(depth);
	if (root->type == TOKEN_HEREDOC)
	{
		ast_print_heredoc(root, depth);
		return ;
	}
	ast_print_node_type(root);
	ast_print_redirect_file(root, depth);
	ast_print_str_array("args", root->args, depth);
}
