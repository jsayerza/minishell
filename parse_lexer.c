/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_whitespace(const char *input, int *i)
{
	if (ft_strchr(" \f\r\n\t\v", input[*i]))
	{
		(*i)++;
		return (1);
	}
	return (0);
}

static int	handle_operator(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	if (ft_strchr("|<>", input[*i]))
	{
		get_operator(input, collector, *i, head);
		(*i)++;
		return (1);
	}
	return (0);
}

static int	handle_quotes(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	if (ft_strchr("'\"", input[*i]))
	{
		get_quoted_str(input, collector, i, head);
		return (1);
	}
	return (0);
}

static int	handle_variables(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	if (input[*i] == '$')
	{
		get_expand_var(input, collector, i, head);
		return (1);
	}
	return (0);
}

t_token	*lexer(const char *input, t_collector **collector, t_token **head)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (handle_whitespace(input, &i)
			|| handle_operator(input, collector, &i, head)
			|| handle_quotes(input, collector, &i, head)
			|| handle_variables(input, collector, &i, head))
			continue ;
		get_word(input, collector, &i, head);
	}
	token_create(collector, TOKEN_EOF, "EOF", head);
	return (*head);
}
