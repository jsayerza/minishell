/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:20:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/13 20:13:09 by acarranz         ###   ########.fr       */
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
static t_ast	*crea_redir_node(t_collector **collector, \
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

static t_ast	*parse_redirection(t_collector **collector, \
	t_token **tokens, t_ast *cmd_node, int interact)
{
	t_token	*curr;
	t_ast	*redir_node;

	while (*tokens && (*tokens)->type >= TOKEN_REDIRECT_IN)
	{
		curr = *tokens;
		*tokens = curr->next;
		if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
		{
			printf("minishell: syntax error expected file after `%s`\n",\
				curr->value);
			return (NULL);
		}
		redir_node = crea_redir_node(collector, curr, *tokens, \
			cmd_node, interact);
		if (!redir_node)
			return (NULL);
		cmd_node = redir_node;
		*tokens = (*tokens)->next;
	}
	return (cmd_node);
}

static t_ast	*init_command_node(t_collector **collector)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		exit_program(collector, "Error malloc parser command node", \
			EXIT_FAILURE);
	node->type = TOKEN_COMMAND;
	node->left = NULL;
	node->right = NULL;
	node->file = NULL;
	node->args = malloc(sizeof(char *) * MAX_CMD_ARGS);
	if (!node->args)
		exit_program(collector, "Error malloc parser command node args", \
			EXIT_FAILURE);
	collector_append(collector, node);
	collector_append(collector, node->args);
	return (node);
}

t_ast	*parse_command(t_collector **collector, t_token **tokens, int interact)
{
	t_ast	*node;
	int		i;

	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	node = init_command_node(collector);
	if (!node)
		return (NULL);
	i = 0;
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		node->args[i] = ft_strdup((*tokens)->value);
		if (!node->args[i])
			exit_program(collector, \
				"Error malloc parser command args node", EXIT_FAILURE);
		collector_append(collector, node->args[i]);
		i++;
		*tokens = (*tokens)->next;
	}
	node->args[i] = NULL;
	collector_append(collector, node);
	return (parse_redirection(collector, tokens, node, interact));
}
