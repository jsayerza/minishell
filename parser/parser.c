/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 11:40:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_token_invalid(t_token *curr)
{
	if ((curr->type == TOKEN_PIPE && (!curr->next ||
			curr->next->type == TOKEN_PIPE)) ||
		(curr->type >= TOKEN_REDIRECT_IN &&
			(!curr->next || curr->next->type >= TOKEN_REDIRECT_IN)))
	{
		printf("Syntax error near unexpected token `%s`\n", curr->value);
		return (1);
	}
	return (0);
}

static int	tokens_validate(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		if (is_token_invalid(curr))
			return (0);
		curr = curr->next;
	}
	return (1);
}

static t_ast	*create_redirect_node(t_collector **collector, \
	t_token *curr, t_token *next, t_ast *cmd)
{
	t_ast	*redir;

	redir = malloc(sizeof(t_ast));
	// if (!redir)
	// 	return (NULL);
	if (!redir)
	exit_program(collector, "Error malloc parser redirect node", \
		EXIT_FAILURE);
	collector_append(collector, redir);
	redir->type = curr->type;
	redir->file = ft_strdup(next->value);
	if (!redir->file)
		exit_program(collector, \
			"Error malloc parser redirect node", EXIT_FAILURE);
	collector_append(collector, redir->file);
	redir->left = cmd;
	redir->right = NULL;
	redir->args = NULL;
	return (redir);
}

static t_ast	*parse_redirection(t_collector **collector, \
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
	// if (!node)
	// 	return (NULL);
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

static t_ast	*parse_command(t_collector **collector, t_token **tokens)
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

static t_ast	*parse_pipeline(t_collector **collector, t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;
	t_token	*curr;

	left = parse_command(collector, tokens);
	if (!left)
		return (NULL);
	curr = *tokens;
	while (curr && curr->type == TOKEN_PIPE)
	{
		*tokens = curr->next;
		right = parse_pipeline(collector, tokens);
		if (!right)
		{
			printf("Syntax error: unexpected token after `|`\n");
			return (NULL);
		}
		node = malloc(sizeof(t_ast));
		// if (!node)
		// 	return (NULL);
		if (!node)
			exit_program(collector, "Error malloc parser node", EXIT_FAILURE);
		collector_append(collector, node);
		node->type = TOKEN_PIPE;
		node->left = left;
		node->right = right;
		left = node;
		curr = *tokens;
	}
	return (left);
}

t_ast	*parser(t_collector **collector, t_token *tokens)
{
	if (!tokens_validate(tokens))
	{
		printf("Error: Invalid tokens\n");
		tokens_free(tokens);
		return (NULL);
	}
	printf("parse-Tokens OK!\n");
	return (parse_pipeline(collector, &tokens));
}
