/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor_node_create.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 20:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/29 21:07:20 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	constructor_node_create_builtin(t_constructor *node)
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

static void	constructor_fields_init(t_constructor *node, t_ast *ast, \
	t_shell *shell)
{
	node->executable = ast->args;
	node->size_exec = 0;
	node->redirect_in = NULL;
	node->redirect_out = NULL;
	node->redirect_append = NULL;
	node->heredoc = NULL;
	node->redirect_in_type = 0;
	node->redirect_out_type = 0;
	node->pipe_in = 0;
	node->pipe_out = 0;
	node->shell = shell;
	node->pid = -1;
	node->builtin = BUILTIN_NONE;
	node->type = ast->type;
	node->next = NULL;
	node->prev = NULL;
}

static void	count_exec_args(t_constructor *node)
{
	char	**args;

	args = node->executable;
	while (args && args[node->size_exec])
		node->size_exec++;
}

t_constructor	*constructor_node_create(t_collector **collector, \
	t_ast *ast, t_shell *shell)
{
	t_constructor	*node;

	node = malloc(sizeof(t_constructor));
	if (!node)
		exit_program(collector, "Error malloc constructor node", EXIT_FAILURE);
	constructor_fields_init(node, ast, shell);
	count_exec_args(node);
	constructor_node_create_builtin(node);
	collector_append(collector, node);
	return (node);
}
