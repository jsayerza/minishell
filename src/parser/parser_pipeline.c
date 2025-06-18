/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/06/10 17:45:14 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_pipeline_add_node(t_collector **collector,
	t_ast **left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		exit_program(collector,
			"Error malloc parser command node", EXIT_FAILURE);
	collector_append(collector, node);
	node->type = TOKEN_PIPE;
	node->left = *left;
	node->right = right;
	*left = node;
}

static int	parse_pipe_error_check(t_token **tokens)
{
	if (!tokens || !*tokens)
	{
		ft_putstr_fd("minishell: syntax error unexpected token `|`\n", 2);
		return (1);
	}
	return (0);
}

static int	parse_pipe_unexpected_eof(t_token **tokens)
{
	if (!*tokens || (*tokens)->type == TOKEN_EOF)
	{
		ft_putstr_fd("minishell: syntax error near", 2);
		ft_putstr_fd(" unexpected token `newline'\n", 2);
		return (1);
	}
	return (0);
}

static int	parse_pipe_error_after_pipe(void)
{
	ft_putstr_fd("minishell: syntax error unexpected token after `|`\n", 2);
	return (1);
}

t_ast	*parse_pipeline(t_collector **collector, t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_token	*curr;

	if (parse_pipe_error_check(tokens))
		return (NULL);
	left = parse_command(collector, tokens);
	if (!left)
		return (NULL);
	curr = *tokens;
	while (curr && curr->type == TOKEN_PIPE)
	{
		*tokens = curr->next;
		if (parse_pipe_unexpected_eof(tokens))
			return (NULL);
		right = parse_pipeline(collector, tokens);
		if (!right)
			return (parse_pipe_error_after_pipe(), NULL);
		parse_pipeline_add_node(collector, &left, right);
		curr = *tokens;
	}
	return (left);
}
