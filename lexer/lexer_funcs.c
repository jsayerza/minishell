/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_invalidchars(const char *input, int i)
{
	if (ft_strchr("*&(){}[];\\", input[i]))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `%c`\n", \
			input[i]);
		return (1);
	}
	return (0);
}

t_token_type	get_token_type(const char *input, int *i, char quote_type)
{
	while (input[*i] && input[*i] != quote_type)
		(*i)++;
	if (!input[*i])
		return (TOKEN_WORD);
	if (quote_type == '"')
		return (TOKEN_DQUOTE);
	else if (quote_type == '\'')
		return (TOKEN_SQUOTE);
	else
		return (TOKEN_ENV_ASSIGN);
}

void	get_quoted_str(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	int				start;
	char			*quoted;
	char			quote_str[2];
	char			quote_type;
	t_token_type	token_type;

	quote_type = input[*i];
	start = ++(*i);
	token_type = get_token_type(input, i, quote_type);
	if (token_type == TOKEN_WORD)
		return ;
	else if (token_type == TOKEN_ENV_ASSIGN)
		exit_program(collector, "minishell: unknown quote type", false);
	quote_str[0] = quote_type;
	quote_str[1] = '\0';
	token_create(collector, token_type, quote_str, head);
	quoted = ft_strndup(input + start, *i - start);
	if (!quoted)
		exit_program(collector, "Error malloc get val for token", EXIT_FAILURE);
	token_create(collector, TOKEN_WORD, quoted, head);
	freer(quoted);
	token_create(collector, token_type, quote_str, head);
	(*i)++;
}

void	get_word(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	int		istart;
	char	*value;

	istart = *i;
	while (input[*i]
		&& (ft_strchr(" \f\r\n\t\v|<>;'\"\\", input[*i]) == NULL))
		(*i)++;
	value = ft_strndup(input + istart, *i - istart);
	if (!value)
		exit_program(collector, "Error malloc get val for token", EXIT_FAILURE);
	token_create(collector, TOKEN_WORD, value, head);
	freer(value);
}
