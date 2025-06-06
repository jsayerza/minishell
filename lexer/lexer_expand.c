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

static void expand_ass_q_token_remove(t_token **head, t_token	*start, \
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

static void expand_q_token_remove(t_token **head, t_token	*start, \
	t_token	*next, t_collector **collector)
{
	t_token	*token_tmp;
	
	while (start != next)
	{
		token_tmp = start;
		start = start->next;
		token_remove(head, token_tmp, collector);
	}
}

static void	expand_assignment_quote(t_token **head, t_token *curr, \
	t_shell *shell, t_collector **collector)
{
	t_token	*start;
	t_token	*prev;	
	t_token	*next;
	// t_token	*token_tmp;
	// t_token	*del;
	char	*joined;
	char	*tmp;
	char	*expanded;

	prev = token_get_prev(*head, curr);
	start = curr;
	curr = curr->next;
	joined = ft_strdup("");
	if (!joined)
		exit_program(collector, "Error mllc tok_expd ass+q", true);
	while (curr && curr->type != start->type)
	{
		if (start->type == TOKEN_DQUOTE)
			expanded = expand_string(curr->value, shell, collector);
		else
			expanded = ft_strdup(curr->value);
		if (!expanded)
			exit_program(collector, "Error strdup/expd as+q", true);
		tmp = ft_strjoin(joined, expanded);
		freer(expanded);
		if (!tmp)
			exit_program(collector, "Error join ass+quote", true);
		freer(joined);
		joined = tmp;
		curr = curr->next;
	}
	if (!curr || curr->type != start->type)
		exit_program(collector, \
			"minishell: unclosed quote in assign", false);
	next = curr->next;
	tmp = ft_strjoin(prev->value, joined);
	if (!collector_contains(*collector, prev->value))
		freer(prev->value);
	prev->value = tmp;
	collector_append(collector, prev->value);
	freer(joined);
	// del = start;
	// while (del != next)
	// {
	// 	token_tmp = del;
	// 	del = del->next;
	// 	token_remove(head, token_tmp, collector);
	// }
	expand_ass_q_token_remove(head, start, next, collector);
	curr = next;
}

static void	expand_quotes(t_token **head, t_token *curr, t_shell *shell, t_collector **collector)
{
	t_token	*start;
	t_token	*next;
	t_token	*token_tmp;
	char	*joined;
	char	*tmp;
	char	*expanded;

	start = curr;
	curr = curr->next;
	joined = ft_strdup("");
	if (!joined)
		exit_program(collector, "Error mllc toks_expd init join", true);
	while (curr && curr->type != start->type)
	{
		if (start->type == TOKEN_DQUOTE)
		{
			expanded = expand_string(curr->value, shell, collector);
			tmp = ft_strjoin(joined, expanded);
			freer(expanded);
		}
		else
			tmp = ft_strjoin(joined, curr->value);
		if (!tmp)
			exit_program(collector, "Error join toks_expd (q)", true);
		freer(joined);
		joined = tmp;
		curr = curr->next;
	}
	if (!curr || curr->type != start->type)
		exit_program(collector, "minishell: unclosed quote", false);
	token_tmp = token_insert_before(head, start, collector);
	if (!token_tmp)
			exit_program(collector, "Error insert token expd", true);
	token_inserted_fill(token_tmp, TOKEN_WORD, joined, collector);
	freer(joined);
	next = curr->next;
	// while (start != next)
	// {
	// 	token_tmp = start;
	// 	start = start->next;
	// 	token_remove(head, token_tmp, collector);
	// }
	expand_q_token_remove(head, start, next, collector);
	curr = next;
}

void	tokens_expand(t_token **head, t_shell *shell, t_collector **collector)
{
	t_token	*curr;
	t_token	*prev;
	char	*expanded;

	curr = *head;
	while (curr)
	{
		if ((curr->type == TOKEN_DQUOTE || curr->type == TOKEN_SQUOTE))
		{
			prev = token_get_prev(*head, curr);
			if (prev && prev->type == TOKEN_WORD && ft_strchr(prev->value, '='))
			{
				expand_assignment_quote(head, curr, shell, collector);
				continue ;
			}
		}
		if (curr->type == TOKEN_DQUOTE || curr->type == TOKEN_SQUOTE)
		{
			expand_quotes(head, curr, shell, collector);
			continue ;
		}
		else if (curr->type == TOKEN_WORD || curr->type == TOKEN_COMMAND)
		{
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
		curr = curr->next;
	}
}
