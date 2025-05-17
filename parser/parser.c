/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <jsayerza@student.42.fr>          +#+  +:+       +#+        */
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
	node->type = TOKEN_PIPE;
	node->left = *left;
	node->right = right;
	*left = node;
	collector_append(collector, node);
}

static t_ast	*parse_pipeline(t_collector **collector, \
	t_token **tokens, int interact)
{
	t_ast	*left;
	t_ast	*right;
	t_token	*curr;

	left = parse_command(collector, tokens, interact);
	if (!left)
		return (NULL);
	curr = *tokens;
	while (curr && curr->type == TOKEN_PIPE)
	{
		*tokens = curr->next;
		right = parse_pipeline(collector, tokens, interact);
		if (!right)
		{
			printf("minishell: syntax error unexpected token after `|`\n");
			return (NULL);
		}
		parse_pipeline_add_node(collector, &left, right);
		curr = *tokens;
	}
	return (left);
}

t_ast	*parser(t_collector **collector, t_token *tokens, int interact)
{
	t_ast	*ast;

	printf("IN parser\n");
	if (!tokens_validate(tokens))
	{
		// print_error("minishell: syntax error");
		return (NULL);
	}
	ast = parse_pipeline(collector, &tokens, interact);
	// if (!ast)
	// 	print_error("minishell: parser error");
	printf("OUT parser\n");
	return (ast);
}
