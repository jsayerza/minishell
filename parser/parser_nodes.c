/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_nodes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/22 17:41:44 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redir_node_heredoc(t_collector **collector, \
	t_token *next, t_ast *redir_node, int interact)
{
	redir_node->heredoc_content = heredoc_read(next->value, \
		interact, collector);
	if (!redir_node->heredoc_content)
		exit_program(collector, \
			"Error saving heredoc content", EXIT_FAILURE);
	collector_append(collector, redir_node->heredoc_content);
}

//// Eliminarem interact en versió final (amb terminal)
t_ast	*init_redir_node(t_collector **collector, \
	t_token *curr, t_token *next, t_ast *cmd_node, int interact)
{
	t_ast	*redir_node;

	redir_node = malloc(sizeof(t_ast));
	if (!redir_node)
		exit_program(collector, \
			"Error malloc parser redirect node", EXIT_FAILURE);
	collector_append(collector, redir_node);
	redir_node->type = curr->type;
	redir_node->file = ft_strdup(next->value);
	if (!redir_node->file)
		exit_program(collector, \
			"Error malloc parser redirect file node", EXIT_FAILURE);
	collector_append(collector, redir_node->file);
	redir_node->left = cmd_node;
	redir_node->right = NULL;
	redir_node->args = NULL;
	redir_node->heredoc_content = NULL;
	if (curr->type == TOKEN_HEREDOC)
		redir_node_heredoc(collector, next, redir_node, interact);
	return (redir_node);
}

t_ast	*init_command_node(t_collector **collector)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		exit_program(collector, "Error malloc parser command node", EXIT_FAILURE);
	collector_append(collector, node);
	node->type = TOKEN_COMMAND;
	node->left = NULL;
	node->right = NULL;
	node->file = NULL;
	node->heredoc_content = NULL;
	node->args = malloc(sizeof(char *) * MAX_CMD_ARGS);
	if (!node->args)
		exit_program(collector, "Error malloc parser command node args", EXIT_FAILURE);
	collector_append(collector, node->args);
	//node->envp = malloc(sizeof(char *) * MAX_CMD_ARGS);
	//if (!node->envp)
	//	exit_program(collector, "Error malloc parser command node envp", EXIT_FAILURE);
	//collector_append(collector, node->envp);
	return (node);
}

t_ast	*init_word_node(t_collector **collector, const char *value)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		exit_program(collector, "Error malloc word node", EXIT_FAILURE);
	collector_append(collector, node);
	node->type = TOKEN_WORD;
	node->left = NULL;
	node->right = NULL;
	node->file = NULL;
	node->heredoc_content = NULL;
	node->args = malloc(sizeof(char *) * 2);
	if (!node->args)
		exit_program(collector, "Error malloc args for word node", EXIT_FAILURE);
	collector_append(collector, node->args);
	node->args[0] = ft_strdup(value);
	if (!node->args[0])
		exit_program(collector, "Error strdup word node arg", EXIT_FAILURE);
	collector_append(collector, node->args[0]);
	node->args[1] = NULL;
	return (node);
}
