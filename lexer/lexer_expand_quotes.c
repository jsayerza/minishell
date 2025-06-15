/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expand_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_quotes_join_loop(t_token *start, t_token **curr,
	t_shell *shell, t_collector **collector)
{
	char	*joined;
	char	*tmp;

	joined = ft_strdup("");
	if (!joined)
		exit_program(collector, "Error mllc toks_expd init join", true);
	while (*curr && (*curr)->type != start->type)
	{
		if (start->type == TOKEN_DQUOTE)
			tmp = expand_quotes_token_dquote(joined, *curr, shell, collector);
		else
			tmp = ft_strjoin(joined, (*curr)->value);
		expand_quotes_tmp_ctrl(tmp, &joined, collector);
		*curr = (*curr)->next;
	}
	if (!(*curr) || (*curr)->type != start->type)
		exit_program(collector, "minishell: unclosed quote", false);
	return (joined);
}

static bool	expand_quotes_merge_prev(t_token **head, t_token *start,
	char *joined, t_collector **collector)
{
	t_token	*prev;
	char	*merged;

	prev = token_get_prev(*head, start);
	if (prev && prev->type == TOKEN_WORD && prev != *head)
	{
		merged = ft_strjoin(prev->value, joined);
		if (!merged)
			exit_program(collector, "Error fusion prev+joined", true);
		collector_remove_ptr(collector, prev->value);
		prev->value = merged;
		collector_append(collector, prev->value);
		return (true);
	}
	return (false);
}

static bool	expand_quotes_merge_adjacent(t_token *prev, t_collector **collector)
{
	t_token	*next;
	char	*merged;

	if (!prev)
		return (false);
	next = prev->next;
	if (!next || next->type != TOKEN_WORD || prev->type != TOKEN_WORD)
		return (false);
	merged = ft_strjoin(prev->value, next->value);
	if (!merged)
		exit_program(collector, "Error fusion prev+next", true);
	collector_remove_ptr(collector, prev->value);
	prev->value = merged;
	collector_append(collector, prev->value);
	return (true);
}

static t_token	*expand_quotes_merge_adjacent_proc(t_token **head,
	t_token *next, bool merged_into_prev, t_collector **collector)
{
	t_token	*prev;
	t_token	*tmp_next;

	prev = token_get_prev(*head, next);
	merged_into_prev = expand_quotes_merge_adjacent(prev, collector);
	if (merged_into_prev)
	{
		tmp_next = prev->next->next;
		token_remove(head, prev->next, collector);
		return (tmp_next);
	}
	return (next);
}

t_token	*expand_quotes(t_token **head, t_token *curr,
	t_shell *shell, t_collector **collector)
{
	t_token	*start;
	t_token	*next;
	char	*joined;
	bool	merged_into_prev;

	start = curr;
	curr = curr->next;
	joined = expand_quotes_join_loop(start, &curr, shell, collector);
	next = curr->next;
	merged_into_prev = expand_quotes_merge_prev(head, start, joined, collector);
	if (joined && *joined)
	{
		if (!merged_into_prev)
			expand_quotes_token_insert(head, start, joined, collector);
		expand_quote_token_remove(head, start, next, collector);
	}
	if (joined && *joined)
		next = expand_quotes_merge_adjacent_proc(head, next,
				merged_into_prev, collector);
	freer((void **)&joined);
	return (next);
}
