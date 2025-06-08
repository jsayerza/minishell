/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 20:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/29 21:07:20 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_constructor	*process_redir_nodes_l_r(t_constructor *left, \
	t_constructor *right)
{
	t_constructor	*curr;

	if (right)
	{
		curr = left;
		while (curr->next)
			curr = curr->next;
		curr->next = right;
		right->prev = curr;
	}
	return (left);
}

static void	process_redir_nodes(t_collector **collector, \
	t_ast *ast, t_shell *shell, t_constructor *left)
{
	t_constructor	*cmd;

	cmd = find_or_create_command_node(collector, ast, shell, &left);
	add_redirections(collector, ast, cmd);
}

static t_constructor	*handle_pipe(t_constructor *left, t_constructor *right)
{
	t_constructor	*curr;

	if (!left)
		return (right);
	if (!right)
		return (left);
	curr = left;
	while (curr->next)
		curr = curr->next;
	set_pipe_flags_and_link(curr, right);
	return (left);
}

static t_constructor	*ast_node_process(t_collector **collector, \
	t_ast *ast, t_shell *shell)
{
	t_constructor	*left_nodes;
	t_constructor	*right_nodes;

	if (!ast)
		return (NULL);
	if (ast->type == TOKEN_WORD || ast->type == TOKEN_COMMAND)
		return (constructor_node_create(collector, ast, shell));
	left_nodes = ast_node_process(collector, ast->left, shell);
	right_nodes = ast_node_process(collector, ast->right, shell);
	if (ast->type == TOKEN_PIPE)
		return (handle_pipe(left_nodes, right_nodes));
	process_redir_nodes(collector, ast, shell, left_nodes);
	return (process_redir_nodes_l_r(left_nodes, right_nodes));
}

t_constructor	*ast_to_constructor(t_collector **collector, t_ast *ast, \
	t_shell *shell)
{
	return (ast_node_process(collector, ast, shell));
}
