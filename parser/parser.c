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

static void	parse_pipeline_add_node(t_collector **collector, \
	t_ast **left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		exit_program(collector, \
			"Error malloc parser command node", EXIT_FAILURE);
	collector_append(collector, node);
	node->type = TOKEN_PIPE;
	node->left = *left;
	node->right = right;
	*left = node;
}

static t_ast	*parse_pipeline(t_collector **collector, t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
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
		parse_pipeline_add_node(collector, &left, right);
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
