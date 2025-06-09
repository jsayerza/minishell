/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expand_ass_quote.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 11:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_ass_quote_token_remove(t_token **head, t_token	*start,
	t_token	*next, t_collector **collector)
{
	t_token	*del;
	t_token	*token_tmp;

	del = start;
	while (del != next)
	{
		token_tmp = del;
		del = del->next;
		token_remove(head, token_tmp, collector);
	}
}

static void	expand_ass_quote_join_prev(t_token *prev,
	char *joined, t_collector **collector)
{
	char	*tmp;

	tmp = ft_strjoin(prev->value, joined);
	if (!collector_contains(*collector, prev->value))
		freer(prev->value);
	prev->value = tmp;
	collector_append(collector, prev->value);
	freer(joined);
}

static void	expand_ass_quote_join_expanded(char *expanded,
	char **joined, t_collector **collector)
{
	char	*tmp;

	tmp = ft_strjoin(*joined, expanded);
	freer(expanded);
	if (!tmp)
		exit_program(collector, "Error join ass+quote", true);
	freer(*joined);
	*joined = tmp;
}

static char	*expand_ass_quote_expand(t_token *start, t_token *curr,
	t_shell *shell, t_collector **collector)
{
	char	*expanded;

	if (start->type == TOKEN_DQUOTE)
		expanded = expand_string(curr->value, shell, collector);
	else
		expanded = ft_strdup(curr->value);
	if (!expanded)
		exit_program(collector, "Error strdup/expd as+q", true);
	return (expanded);
}

t_token	*expand_assignment_quote(t_token **head, t_token *curr,
	t_shell *shell, t_collector **collector)
{
	t_token	*start;
	t_token	*next;
	t_token	*prev;	
	char	*joined;
	char	*expanded;

	prev = token_get_prev(*head, curr);
	start = curr;
	curr = curr->next;
	joined = ft_strdup("");
	if (!joined)
		exit_program(collector, "Error mllc tok_expd ass+q", true);
	while (curr && curr->type != start->type)
	{
		expanded = expand_ass_quote_expand(start, curr, shell, collector);
		expand_ass_quote_join_expanded(expanded, &joined, collector);
		curr = curr->next;
	}
	if (!curr || curr->type != start->type)
		exit_program(collector,
			"minishell: unclosed quote in assign", false);
	next = curr->next;
	expand_ass_quote_join_prev(prev, joined, collector);
	expand_ass_quote_token_remove(head, start, next, collector);
	return (next);
}
