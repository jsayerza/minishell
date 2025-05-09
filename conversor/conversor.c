/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:30:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/09 12:19:07 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_constructor_node_builtin(t_constructor *node)
{
	if (node->executable && node->executable[0])
	{
		if (!ft_strcmp(node->executable[0], "echo"))
			node->builtin = BUILTIN_ECHO;
		else if (!ft_strcmp(node->executable[0], "cd"))
			node->builtin = BUILTIN_CD;
		else if (!ft_strcmp(node->executable[0], "pwd"))
			node->builtin = BUILTIN_PWD;
		else if (!ft_strcmp(node->executable[0], "export"))
			node->builtin = BUILTIN_EXPORT;
		else if (!ft_strcmp(node->executable[0], "unset"))
			node->builtin = BUILTIN_UNSET;
		else if (!ft_strcmp(node->executable[0], "env"))
			node->builtin = BUILTIN_ENV;
		else if (!ft_strcmp(node->executable[0], "exit"))
			node->builtin = BUILTIN_EXIT;
	}
}

t_constructor	*create_constructor_node(t_collector **collector, \
	t_ast *ast, t_shell *shell)
{
	t_constructor	*node;

	node = malloc(sizeof(t_constructor));
	if (!node)
		exit_program(collector, "Error malloc constructor node", EXIT_FAILURE);
	collector_append(collector, node);
	//node->executable = ast->cmd;
	node->size_exec = 0;
	while (node->executable && node->executable[node->size_exec])
		node->size_exec++;
	node->input_file = NULL;
	node->output_file = NULL;
	node->pipe_in = 0;
	node->pipe_out = 0;
	node->shell = shell;
	node->pid = -1;
	node->builtin = BUILTIN_NONE;
	node->type = ast->type;
	node->error = 0;
	node->next = NULL;
	node->prev = NULL;
	create_constructor_node_builtin(node);
	return (node);
}

static void	set_pipe_flags_and_link(t_constructor *left, t_constructor *right)
{
	if (left)
	{
		left->pipe_out = (right != NULL);
		left->next = right;
	}
	if (right)
	{
		right->pipe_in = (left != NULL);
		right->prev = left;
	}
}

t_constructor	*ast_to_constructor(t_collector **collector, \
	t_ast *ast, t_shell *shell)
{
	t_constructor	*left;
	t_constructor	*right;
	t_constructor	*curr;

	printf("IN ast_to_constructor\n");
	if (!ast)
		return (NULL);
	if (ast && (ast->type == TOKEN_PIPE ||
			ast->type == TOKEN_APPEND || ast->type == TOKEN_HEREDOC ||
			ast->type == TOKEN_REDIRECT_OUT || ast->type == TOKEN_REDIRECT_IN))
	{
		left = ast_to_constructor(collector, ast->left, shell);
		curr = left;
		while (curr && curr->next)
			curr = curr->next;
		right = ast_to_constructor(collector, ast->right, shell);
		if (ast->type == TOKEN_PIPE)
			set_pipe_flags_and_link(curr, right);
		return (left);
	}
	// if (ast && (ast->type == TOKEN_REDIRECT_OUT || ast->type == TOKEN_REDIRECT_IN ||
	// 		ast->type == TOKEN_APPEND || ast->type == TOKEN_HEREDOC || ast->type == TOKEN_PIPE))
	// {
	// 	printf(" IN ast_to_constructor - redirector\n");
	// 	printf("%s\n", ast->file);
	// 	left = ast_to_constructor(collector, ast->left, shell);
	// 	curr = left;
	// 	while (curr && curr->next)
	// 		curr = curr->next;
	// 	right = ast_to_constructor(collector, ast->right, shell);
	// 	printf(" OUT ast_to_constructor - redirector\n");
	// 	return (left);
	// }
	if (ast->type != TOKEN_COMMAND)
		return (NULL);
	printf("OUT ast_to_constructor --------------------\n\n");
	return (create_constructor_node(collector, ast, shell));
}