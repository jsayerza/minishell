/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:20:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_token_invalid(t_token *curr)
{
	if ((curr->type == TOKEN_PIPE
			&& (!curr->next || curr->next->type == TOKEN_PIPE))
		|| (curr->type >= TOKEN_REDIRECT_IN
			&& (!curr->next || curr->next->type >= TOKEN_REDIRECT_IN)))
	{
		printf("Syntax error near unexpected token `%s`\n", curr->value);
		return (1);
	}
	return (0);
}

int	tokens_validate(t_token *tokens)
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
