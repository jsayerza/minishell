/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 11:40:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/09 11:05:48 by acarranz         ###   ########.fr       */
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

static t_ast	*parse_pipeline(t_collector **collector, \
	t_token **tokens, int interact)
{
	t_ast	*left;
	t_ast	*right;
	t_token	*curr;

	printf("IN parse_pipeline\n");
	if (!tokens || !*tokens)
	{
		printf("minishell: syntax error unexpected token `|`\n");
		return (NULL);
	}
	left = parse_command(collector, tokens, interact);
	if (!left)
	{
		printf("OUT parse_pipeline !left\n");
		return (NULL);
	}
	printf(" IN parse_pipeline 1\n");
	curr = *tokens;
	while (curr && curr->type == TOKEN_PIPE)
	{
		*tokens = curr->next;
		if (!*tokens || (*tokens)->type == TOKEN_EOF)
		{
			printf("minishell: syntax error near unexpected token `newline'\n");
			return (NULL);
		}
		printf("  cap a IN parse_pipeline right\n");
		right = parse_pipeline(collector, tokens, interact);
		printf("  OUT parse_pipeline right\n");
		if (!right)
		{
			printf("minishell: syntax error unexpected token after `|`\n");
			return (NULL);
		}
		parse_pipeline_add_node(collector, &left, right);
		curr = *tokens;
	}
	printf("OUT parse_pipeline\n");
	return (left);
}

t_ast	*parser(t_collector **collector, t_token *tokens, int interact)
{
	t_ast	*ast;

	printf("IN parser\n");
	if (!tokens_validate(tokens))
		return (NULL);
	ast = parse_pipeline(collector, &tokens, interact);
	printf("OUT parser --------------------\n\n");
	return (ast);
}
