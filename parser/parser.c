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

static int	tokens_validate(t_token *tokens)
{
	t_token	*curr = tokens;

	while (curr)
	{
		if ((curr->type == TOKEN_PIPE && (!curr->next ||
			 curr->next->type == TOKEN_PIPE)) ||
			(curr->type >= TOKEN_REDIRECT_IN &&
				(!curr->next || curr->next->type >= TOKEN_REDIRECT_IN)))
		{
			printf("Syntax error near unexpected token `%s`\n", curr->value);
			return (false);
		}
		curr = curr->next;
	}
	return (true);
}

static t_ast	*parse_redirection(t_token **tokens, t_ast *cmd)
{
	t_token	*curr = *tokens;

	while (curr && curr->type >= TOKEN_REDIRECT_IN)
	{
		*tokens = curr->next;
		if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
		{
			printf("Syntax error: expected file after `%s`\n", curr->value);
			return (NULL);
		}
		t_ast *redir = malloc(sizeof(t_ast));
		redir->type = curr->type;
		redir->file = strdup((*tokens)->value);
		redir->left = cmd;
		redir->right = NULL;
		cmd = redir;
		*tokens = (*tokens)->next;
		curr = *tokens;
	}
	return (cmd);
}

static t_ast	*parse_command(t_token **tokens)
{
	t_ast	*node = malloc(sizeof(t_ast));
	node->type = TOKEN_COMMAND;
	node->args = malloc(sizeof(char *) * 10); // Adjust dynamically
	int i = 0;

	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		node->args[i++] = strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	node->args[i] = NULL;
	return (parse_redirection(tokens, node));
}

static t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left = parse_command(tokens);
	t_token	*curr = *tokens;

	while (curr && curr->type == TOKEN_PIPE)
	{
		*tokens = curr->next;
		t_ast *right = parse_command(tokens);
		// TODO: usar collector x fer malloc
		t_ast *node = malloc(sizeof(t_ast));
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
