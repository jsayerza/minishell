/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:30:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/09 17:03:35 by acarranz         ###   ########.fr       */
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
	node->executable = ast->args;
	node->size_exec = 0;
	while (node->executable && node->executable[node->size_exec])
		node->size_exec++;
	// if (ast->file && (ast->type == TOKEN_REDIRECT_IN || ast->type == TOKEN_REDIRECT_OUT
	// 	|| ast->type == TOKEN_APPEND))
	// 	node->file = ast->file;
	// else
	// 	node->file = NULL;
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

t_constructor	*ast_to_constructor(t_collector **collector, t_ast *ast, t_shell *shell)
{
	t_constructor	*left;
	t_constructor	*right;
	t_constructor	*curr;
	t_constructor	*node;

	printf("IN ast_to_constructor\n");
	if (!ast)
		return (NULL);
	if (ast->type == TOKEN_PIPE)
	{
		printf("pipe\n");
		left = ast_to_constructor(collector, ast->left, shell);
		right = ast_to_constructor(collector, ast->right, shell);
		curr = left;
		while (curr && curr->next)
			curr = curr->next;
		set_pipe_flags_and_link(curr, right);
		curr->next = right;
		return (left);
	}
	if (ast->type == TOKEN_REDIRECT_IN
		|| ast->type == TOKEN_REDIRECT_OUT
		|| ast->type == TOKEN_APPEND
		|| ast->type == TOKEN_HEREDOC)
	{
		printf("redirect: %s\n", ast->file);
		left = ast_to_constructor(collector, ast->left, shell);
		right = ast_to_constructor(collector, ast->right, shell);
		node = create_constructor_node(collector, ast, shell);
		if (!left)
			left = node;
		else
		{
			curr = left;
			while (curr->next)
				curr = curr->next;
			curr->next = node;
		}
		if (right)
		{
			curr = node;
			while (curr->next)
				curr = curr->next;
			curr->next = right;
		}
		return (left);
	}
	if (ast->type == TOKEN_COMMAND)
	{
		printf("command: creating node\n");
		return (create_constructor_node(collector, ast, shell));
	}
	return (NULL);
}
