/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 11:40:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/22 17:45:41 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ast_print_indent(int depth)
{
	int	i = 0;
	while (i++ < depth)
		printf("  ");
}

static void	ast_print_str_array(char *label, char **array, int depth)
{
	int	i;

	if (!array)
	{
		ast_print_indent(depth);
		printf("%s: [NULL]\n", label);
		return;
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

static void	ast_print_type(t_ast *root, int depth)
{
	ast_print_indent(depth);
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
	else if (root->type == TOKEN_HEREDOC)
	{
		printf(MAGENTA "HEREDOC (<<): %s\n" RESET, root->file);
		ast_print_indent(depth);
		printf("CONTENT:\n");
		ast_print_indent(depth + 1);
		printf("%s\n", root->heredoc_content ? root->heredoc_content : "[NULL]");
		return;
	}
	else if (root->type == TOKEN_APPEND)
		printf(MAGENTA "APPEND (>>)\n" RESET);
	else
		printf(RED "UNKNOWN NODE\n" RESET);

	if (root->file && root->type < TOKEN_REDIRECT_IN) // evita repetir en redirecciones
	{
		ast_print_indent(depth);
		printf("file: %s\n", root->file);
	}

	ast_print_str_array("args", root->args, depth);
	//ast_print_str_array("envp", root->envp, depth);
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
	ast_print_type(root, depth);
	ast_print_branch("LEFT", root->left, depth);
	ast_print_branch("RIGHT", root->right, depth);
}
