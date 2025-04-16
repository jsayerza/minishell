/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:30:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_constructor	*create_constructor_node(t_collector **collector, t_ast *ast, t_shell *shell)
{
	t_constructor	*node;
	// int				i;

	node = malloc(sizeof(t_constructor));
	if (!node)
		exit_program(collector, "Error malloc constructor node", EXIT_FAILURE);
	collector_append(collector, node);

	node->executable = ast->args;
	node->size_exec = 0;
	while (node->executable && node->executable[node->size_exec])
		node->size_exec++;

	node->read_fd = -1;
	node->write_fd = -1;
	node->type = ast->type;
	node->error = 0;
	node->shell = shell;
	node->next = NULL;

	// Detectar si es builtin
	node->builtin = BUILTIN_NONE;
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

	return (node);
}

t_constructor	*ast_to_constructor(t_collector **collector, t_ast *ast, t_shell *shell)
{
	t_constructor	*head;
	t_constructor	*curr;

	if (!ast)
		return (NULL);

	// Ir a la izquierda si es pipe
	if (ast->type == TOKEN_PIPE)
	{
		head = ast_to_constructor(collector, ast->left, shell);
		curr = head;
		while (curr && curr->next)
			curr = curr->next;
		curr->next = ast_to_constructor(collector, ast->right, shell);
		return (head);
	}

	// Si es una redirección, buscar el comando real en .left
	while (ast && (ast->type == TOKEN_REDIRECT_IN || ast->type == TOKEN_REDIRECT_OUT
		|| ast->type == TOKEN_APPEND || ast->type == TOKEN_HEREDOC))
		ast = ast->left;

	if (!ast || ast->type != TOKEN_COMMAND)
		return (NULL);

	return create_constructor_node(collector, ast, shell);
}
