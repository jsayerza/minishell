/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	process_input_tokens(const char *input, t_collector **collector,
	int *i, t_token **head)
{
	while (input[*i])
	{
		if (handle_invalidchars(input, *i))
		{
			exit_program(collector, "minishell: invalid character", false);
			return (false);
		}
		if (handle_whitespace(input, collector, i, head)
			|| handle_operator(input, collector, i, head)
			|| handle_quotes(input, collector, i, head))
			continue ;
		get_word(input, collector, i, head);
	}
	return (true);
}

static t_token	*finalize_tokens(t_token **head, t_collector **collector,
	t_shell *shell)
{
	t_token	*first_token;

	tokens_expand(head, shell, collector);
	first_token = *head;
	while (first_token && first_token->type == TOKEN_WHITESPACE)
		first_token = first_token->next;
	if (!first_token)
	{
		exit_program(collector, "minishell: syntax error: empty input", false);
		return (NULL);
	}
	token_create(collector, TOKEN_EOF, "EOF", head);
	return (*head);
}

t_token	*lexer(const char *input, t_collector **collector,
	t_token **head, t_shell *shell)
{
	int	i;

	i = 0;
	if (!process_input_tokens(input, collector, &i, head))
		return (NULL);
	return (finalize_tokens(head, collector, shell));
}
