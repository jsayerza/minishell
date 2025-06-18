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

void	tokens_remove_whitespace(t_token **head, t_collector **collector)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*next;

	curr = *head;
	prev = NULL;
	while (curr)
	{
		next = curr->next;
		if (curr->type == TOKEN_WHITESPACE)
		{
			if (prev)
				prev->next = next;
			else
				*head = next;
			collector_remove_ptr(collector, curr);
		}
		else
			prev = curr;
		curr = next;
	}
}

static char	*tokens_merge_adjacent_words_proc(t_token **head,
	t_token *prev, t_token *next, t_collector **collector)
{
	t_token			*new;
	char			*merged;

	merged = ft_strjoin(prev->value, next->next->next->value);
	if (!merged)
		exit_program(collector, "merge_adjacent_words failed", true);
	new = token_insert_before(head, prev, collector);
	if (!new)
		exit_program(collector, "Error insert token merged", true);
	token_inserted_fill(new, TOKEN_WORD, merged, collector);
	return (merged);
}

void	tokens_merge_adjacent_words(t_token **head, t_collector **collector)
{
	t_token			*prev;
	t_token			*curr;
	t_token			*next;
	char			*merged;

	prev = NULL;
	curr = *head;
	while (curr && curr->next)
	{
		next = curr->next;
		if ((curr->type == TOKEN_DQUOTE || curr->type == TOKEN_SQUOTE)
			&& prev && next && next->next && next->next->type == curr->type
			&& prev->type == TOKEN_WORD && next->type == TOKEN_WORD
			&& next->next->next && next->next->next->type == TOKEN_WORD)
		{
			merged = tokens_merge_adjacent_words_proc(head,
					prev, next, collector);
			curr = next->next->next->next;
			expand_quote_token_remove(head, prev,
				next->next->next->next, collector);
			freer((void **)&merged);
		}
		prev = curr;
		curr = curr->next;
	}
}

t_ast	*parser(t_collector **collector, t_token *tokens)
{
	t_ast	*ast;

	tokens_remove_whitespace(&tokens, collector);
	tokens_merge_adjacent_words(&tokens, collector);
	if (!tokens_validate(tokens))
		return (NULL);
	ast = parse_pipeline(collector, &tokens);
	return (ast);
}
