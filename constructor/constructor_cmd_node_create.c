/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor_cmd_node_create.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 20:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/29 21:07:20 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	link_new_cmd_node(t_constructor *new_node, \
	t_constructor **first_node)
{
	if (*first_node)
		(*first_node)->prev = new_node;
	new_node->next = *first_node;
	*first_node = new_node;
}

static void	init_cmd_node_defaults(t_constructor *node, t_shell *shell)
{
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
	node->type = TOKEN_COMMAND;
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

static t_constructor	*create_new_cmd_node(t_collector **collector, \
	t_ast *ast, t_shell *shell)
{
	t_constructor	*node;
	char			**args;

	node = malloc(sizeof(t_constructor));
	if (!node)
		exit_program(collector, "Error malloc command node", EXIT_FAILURE);
	args = NULL;
	if (ast && ast->args)
		args = ast->args;
	node->executable = args;
	node->size_exec = 0;
	count_exec_args(node);
	init_cmd_node_defaults(node, shell);
	return (node);
}

t_constructor	*find_or_create_command_node(t_collector **collector, \
	t_ast *ast, t_shell *shell, t_constructor **first_node)
{
	t_constructor	*cmd_node;
	t_constructor	*curr;

	curr = *first_node;
	while (curr)
	{
		if (curr->type == TOKEN_COMMAND)
			return (curr);
		curr = curr->next;
	}
	cmd_node = create_new_cmd_node(collector, ast, shell);
	link_new_cmd_node(cmd_node, first_node);
	collector_append(collector, cmd_node);
	return (cmd_node);
}
