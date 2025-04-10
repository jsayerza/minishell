/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:20:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast	*create_redirect_node(t_collector **collector, \
	t_token *curr, t_token *next, t_ast *cmd)
{
	t_ast	*redir;

	redir = malloc(sizeof(t_ast));
	if (!redir)
		exit_program(collector, \
			"Error malloc parser redirect node", EXIT_FAILURE);
	collector_append(collector, redir);
	redir->type = curr->type;
	redir->file = ft_strdup(next->value);
	if (!redir->file)
		exit_program(collector, \
			"Error malloc parser redirect file node", EXIT_FAILURE);
	collector_append(collector, redir->file);
	redir->left = cmd;
	redir->right = NULL;
	redir->args = NULL;
	return (redir);
}

t_ast	*parse_redirection(t_collector **collector, \
	t_token **tokens, t_ast *cmd)
{
	t_token	*curr;
	t_ast	*redir;

	while (*tokens && (*tokens)->type >= TOKEN_REDIRECT_IN)
	{
		curr = *tokens;
		*tokens = curr->next;
		if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
		{
			printf("Syntax error: expected file after `%s`\n", curr->value);
			return (NULL);
		}
		redir = create_redirect_node(collector, curr, *tokens, cmd);
		if (!redir)
			return (NULL);
		cmd = redir;
		*tokens = (*tokens)->next;
	}
	return (cmd);
}

static t_ast	*init_command_node(t_collector **collector)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		exit_program(collector, "Error malloc parser command node", \
			EXIT_FAILURE);
	collector_append(collector, node);
	node->type = TOKEN_COMMAND;
	node->left = NULL;
	node->right = NULL;
	node->file = NULL;
	node->args = malloc(sizeof(char *) * 256);
	// if (!node->args)
	// {
	// 	free(node);
	// 	return (NULL);
	// }
	if (!node->args)
		exit_program(collector, "Error malloc parser command node args", \
			EXIT_FAILURE);
	collector_append(collector, node->args);
	return (node);
}

t_ast	*parse_command(t_collector **collector, t_token **tokens)
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
	return (parse_redirection(collector, tokens, node));
}
