/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
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
		ft_putstr_fd("  ", 1);
}

static void	ast_print_str_array(char *label, char **array, int depth)
{
	int	i;
	if (!array)
	{
		ast_print_indent(depth);
		ft_putstr_fd(label, 1);
		ft_putstr_fd(": [NULL]\n", 1);
		return;
	}
	ast_print_indent(depth);
	ft_putstr_fd(label, 1);
	ft_putstr_fd(":\n", 1);
	i = 0;
	while (array[i])
	{
		ast_print_indent(depth + 1);
		ft_putstr_fd("- ", 1);
		ft_putstr_fd(array[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
}

static void	ast_print_type(t_ast *root, int depth)
{
	ast_print_indent(depth);
	if (root->type == TOKEN_COMMAND)
		ft_putstr_fd(BOLD GREEN "CMD\n" RESET, 1);
	else if (root->type == TOKEN_WORD)
		ft_putstr_fd(BOLD GREEN "WORD\n" RESET, 1);
	else if (root->type == TOKEN_PIPE)
		ft_putstr_fd(YELLOW "PIPE\n" RESET, 1);
	else if (root->type == TOKEN_REDIRECT_IN)
		ft_putstr_fd(BOLD RED "REDIRECT_IN (<)\n" RESET, 1);
	else if (root->type == TOKEN_REDIRECT_OUT)
		ft_putstr_fd(BOLD RED "REDIRECT_OUT (>)\n" RESET, 1);
	else if (root->type == TOKEN_HEREDOC)
	{
		ft_putstr_fd(MAGENTA "HEREDOC (<<): ", 1);
		ft_putstr_fd(root->file, 1);
		ft_putstr_fd("\n" RESET, 1);
		ast_print_indent(depth);
		ft_putstr_fd("CONTENT:\n", 1);
		ast_print_indent(depth + 1);
		ft_putstr_fd(root->heredoc_content ? root->heredoc_content : "[NULL]", 1);
		ft_putstr_fd("\n", 1);
		return;
	}
	else if (root->type == TOKEN_APPEND)
		ft_putstr_fd(MAGENTA "APPEND (>>)\n" RESET, 1);
	else
		ft_putstr_fd(RED "UNKNOWN NODE\n" RESET, 1);
	
	if (root->file &&
		(root->type == TOKEN_REDIRECT_IN || root->type == TOKEN_REDIRECT_OUT ||
		 root->type == TOKEN_APPEND))
	{
		ast_print_indent(depth);
		ft_putstr_fd("file: ", 1);
		ft_putstr_fd(root->file, 1);
		ft_putstr_fd("\n", 1);
	}
	ast_print_str_array("args", root->args, depth);
	// ast_print_str_array("envp", root->envp, depth);
}

static void	ast_print_branch(char *label, t_ast *child, int depth)
{
	ast_print_indent(depth);
	ft_putstr_fd(CYAN, 1);
	ft_putstr_fd(label, 1);
	ft_putstr_fd(" →\n" RESET, 1);
	if (child)
		ast_print(child, depth + 1);
	else
	{
		ast_print_indent(depth + 1);
		ft_putstr_fd("[NULL]\n", 1);
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