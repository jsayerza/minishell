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

static t_ast	*create_redirect_node(t_token *curr, t_token *next, t_ast *cmd)
{
	t_ast	*redir;

	redir = malloc(sizeof(t_ast));
	if (!redir)
		return (NULL);
	redir->type = curr->type;
	redir->file = strdup(next->value);
	redir->left = cmd;
	redir->right = NULL;
	return (redir);
}

static t_ast	*parse_redirection(t_token **tokens, t_ast *cmd)
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
		redir = create_redirect_node(curr, *tokens, cmd);
		if (!redir)
			return (NULL);
		cmd = redir;
		*tokens = (*tokens)->next;
	}
	return (cmd);
}

static t_ast	*init_command_node(void)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = TOKEN_COMMAND;
	node->args = malloc(sizeof(char *) * 256);
	if (!node->args)
	{
		free(node);
		return (NULL);
	}
	return (node);
}

static t_ast	*parse_command(t_token **tokens)
{
	t_ast	*node;
	int		i;

	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	node = init_command_node();
	if (!node)
		return (NULL);
	i = 0;
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		node->args[i] = strdup((*tokens)->value);
		i++;
		*tokens = (*tokens)->next;
	}
	node->args[i] = NULL;
	return (parse_redirection(tokens, node));
}

static t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;
	t_token	*curr;

	left = parse_command(tokens);
	if (!left)
		return (NULL);
	curr = *tokens;
	while (curr && curr->type == TOKEN_PIPE)
	{
		*tokens = curr->next;
		right = parse_pipeline(tokens);
		if (!right)
		{
			printf("Syntax error: unexpected token after `|`\n");
			return (NULL);
		}
		node = malloc(sizeof(t_ast));
		if (!node)
			return (NULL);
		node->type = TOKEN_PIPE;
		node->left = left;
		node->right = right;
		left = node;
		curr = *tokens;
	}
	return (left);
}

t_ast	*parse(t_token *tokens)
{
	if (!tokens_validate(tokens))
	{
		printf("Error: Invalid tokens\n");
		tokens_free(tokens);
		return (NULL);
	}
	printf("parse-Tokens OK!\n");
	return (parse_pipeline(&tokens));
}
