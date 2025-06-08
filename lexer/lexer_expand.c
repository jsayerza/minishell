/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_no_quotes(t_token *curr, t_shell *shell, \
	t_collector **collector)
{
	char	*expanded;

	expanded = expand_string(curr->value, shell, collector);
	if (!expanded)
		exit_program(collector, "Error in expand_string", true);
	if (curr->value)
		collector_remove_ptr(collector, curr->value);
	curr->value = ft_strdup(expanded);
	freer(expanded);
	if (!curr->value)
		exit_program(collector, "Error strdup in expansion", true);
	collector_append(collector, curr->value);
}

void	tokens_expand(t_token **head, t_shell *shell, t_collector **collector)
{
	t_token	*curr;
	t_token	*prev;

	curr = *head;
	while (curr)
	{
		if ((curr->type == TOKEN_DQUOTE || curr->type == TOKEN_SQUOTE))
		{
			prev = token_get_prev(*head, curr);
			if (prev && prev->type == TOKEN_WORD && ft_strchr(prev->value, '='))
			{
				curr = expand_assignment_quote(head, curr, shell, collector);
				continue ;
			}
		}
		if (curr->type == TOKEN_DQUOTE || curr->type == TOKEN_SQUOTE)
		{
			curr = expand_quotes(head, curr, shell, collector);
			continue ;
		}
		else if (curr->type == TOKEN_WORD || curr->type == TOKEN_COMMAND)
			expand_no_quotes(curr, shell, collector);
		curr = curr->next;
	}
}
