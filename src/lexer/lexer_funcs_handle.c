/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_funcs_handle.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_quote_open(const char *input, int pos)
{
	bool	in_squote;
	bool	in_dquote;
	int		i;

	in_squote = false;
	in_dquote = false;
	i = 0;
	while (i < pos)
	{
		if (input[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (input[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		i++;
	}
	return (in_squote || in_dquote);
}

int	handle_whitespace(const char *input,
	t_collector **collector, int *i, t_token **head)
{
	int	start;
	int	before;
	int	after;

	if (!input[*i] || !ft_strchr(" \f\r\n\t\v", input[*i]))
		return (0);
	start = *i;
	while (input[*i] && ft_strchr(" \f\r\n\t\v", input[*i]))
		(*i)++;
	if (is_quote_open(input, start))
		return (1);
	before = start - 1;
	after = *i;
	while (before >= 0 && ft_strchr(" \f\r\n\t\v", input[before]))
		before--;
	while (input[after] && ft_strchr(" \f\r\n\t\v", input[after]))
		after++;
	if (before >= 0 && input[after] != '\0')
		token_create(collector, TOKEN_WHITESPACE, "_", head);
	return (1);
}

int	handle_operator(const char *input, t_collector **collector,
	int *i, t_token **head)
{
	if (ft_strchr("<>|", input[*i]))
	{
		get_operator(input, collector, i, head);
		(*i)++;
		return (1);
	}
	return (0);
}

int	handle_quotes(const char *input, t_collector **collector,
	int *i, t_token **head)
{
	if (ft_strchr("'\"", input[*i]))
	{
		get_quoted_str(input, collector, i, head);
		return (1);
	}
	return (0);
}
