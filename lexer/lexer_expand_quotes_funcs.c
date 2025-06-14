/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expand_quotes_funcs.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:30:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_quote_token_remove(t_token **head, t_token *start,
	t_token *next, t_collector **collector)
{
	t_token	*tmp;

	while (start && start != next)
	{
		tmp = start;
		start = start->next;
		token_remove(head, tmp, collector);
	}
}

void	expand_quotes_token_insert(t_token **head, t_token *start,
	char *joined, t_collector **collector)
{
	t_token	*new;

	if (!joined || !*joined)
		return ;
	new = token_insert_before(head, start, collector);
	if (!new)
		exit_program(collector, "Error insert token expd", true);
	token_inserted_fill(new, TOKEN_WORD, joined, collector);
}

char	*expand_quotes_token_dquote(char *joined, t_token *curr,
	t_shell *shell, t_collector **collector)
{
	char	*expanded;
	char	*tmp;

	expanded = expand_string(curr->value, shell, collector);
	tmp = ft_strjoin(joined, expanded);
	if (!tmp)
		exit_program(collector, "Error join toks_expd (q)", true);
	freer((void **)&expanded);
	return (tmp);
}

void	expand_quotes_tmp_ctrl(char *tmp, char **joined,
	t_collector **collector)
{
	if (!tmp)
		exit_program(collector, "Error join toks_expd (q)", true);
	freer((void **)&*joined);
	*joined = tmp;
}
