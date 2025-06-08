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

static void	expand_quote_token_remove(t_token **head, t_token	*start, \
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

static void	expand_quotes_token_insert(t_token **head, t_token *start, \
	char **joined, t_collector **collector)
{
	t_token	*token_tmp;

	token_tmp = token_insert_before(head, start, collector);
	if (!token_tmp)
		exit_program(collector, "Error insert token expd", true);
	token_inserted_fill(token_tmp, TOKEN_WORD, *joined, collector);
	freer(*joined);
}

static char	*expand_quotes_token_dquote(char *joined, t_token *curr, \
	t_shell *shell, t_collector **collector)
{
	char	*expanded;
	char	*tmp;

	expanded = expand_string(curr->value, shell, collector);
	tmp = ft_strjoin(joined, expanded);
	if (!tmp)
		exit_program(collector, "Error join toks_expd (q)", true);
	freer(expanded);
	return (tmp);
}

static void	expand_quotes_tmp_ctrl(char *tmp, char **joined, \
	t_collector **collector)
{
	if (!tmp)
		exit_program(collector, "Error join toks_expd (q)", true);
	freer(*joined);
	*joined = tmp;
}

void	expand_quotes(t_token **head, t_token *curr, t_shell *shell, \
	t_collector **collector)
{
	t_token	*start;
	t_token	*next;
	char	*joined;
	char	*tmp;

	start = curr;
	curr = curr->next;
	joined = ft_strdup("");
	if (!joined)
		exit_program(collector, "Error mllc toks_expd init join", true);
	while (curr && curr->type != start->type)
	{
		if (start->type == TOKEN_DQUOTE)
			tmp = expand_quotes_token_dquote(joined, curr, shell, collector);
		else
			tmp = ft_strjoin(joined, curr->value);
		expand_quotes_tmp_ctrl(tmp, &joined, collector);
		curr = curr->next;
	}
	if (!curr || curr->type != start->type)
		exit_program(collector, "minishell: unclosed quote", false);
	expand_quotes_token_insert(head, start, &joined, collector);
	next = curr->next;
	expand_quote_token_remove(head, start, next, collector);
	curr = next;
}
