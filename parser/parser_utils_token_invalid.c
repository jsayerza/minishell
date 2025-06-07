/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_token_invalid.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:20:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_invalid_start(t_token *curr)
{
	if (!curr)
		return (0);
	if (curr->type != TOKEN_WORD
		&& curr->type != TOKEN_REDIRECT_IN
		&& curr->type != TOKEN_REDIRECT_OUT
		&& curr->type != TOKEN_HEREDOC
		&& curr->type != TOKEN_APPEND)
	{
		print_error("minishell: syntax error near unexpected token at start");
		return (1);
	}
	return (0);
}

static int	is_invalid_pipe(t_token *prev, t_token *curr)
{
	if (curr->type != TOKEN_PIPE)
		return (0);
	if (!prev || prev->type == TOKEN_PIPE)
	{
		print_error("minishell: syntax error near unexpected token `|`");
		return (1);
	}
	if (!curr->next || curr->next->type == TOKEN_PIPE)
	{
		print_error("minishell: syntax error near unexpected token `|`");
		return (1);
	}
	return (0);
}

static int	is_invalid_redirect(t_token *curr)
{
	if (curr->type < TOKEN_REDIRECT_IN || curr->type > TOKEN_APPEND)
		return (0);
	if (!curr->next)
	{
		print_error("minishell: syntax error near unexpectd token newline");
		return (1);
	}
	if (curr->next->type >= TOKEN_REDIRECT_IN
		&& curr->next->type <= TOKEN_APPEND)
	{
		printf("minishell: syntax error near unexpected token `%s`\n", \
			curr->next->value);
		return (1);
	}
	return (0);
}

int	is_token_invalid(t_token *prev, t_token *curr)
{
	if (!curr)
		return (0);
	if (!prev && is_invalid_start(curr))
		return (1);
	if (is_invalid_pipe(prev, curr))
		return (1);
	if (is_invalid_redirect(curr))
		return (1);
	return (0);
}
