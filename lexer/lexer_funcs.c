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
		printf("minishell: syntax error near unexpected token `%c`\n", \
			input[i]);
		return (1);
	}
	return (0);
}

void	get_quoted_str(const char *input, t_collector **collector, int *i, t_token **head)
{
	int				start;
	char			*quoted;
	char			quote_str[2];
	char			quote_type;
	t_token_type	token_type = TOKEN_WORD;

	//printf("IN get_quoted_str\n");
	quote_type = input[*i];
	start = ++(*i);
	while (input[*i] && input[*i] != quote_type)
		(*i)++;
	if (!input[*i])
		return ;
	if (quote_type == '"')
		token_type = TOKEN_DQUOTE;
	else if (quote_type == '\'')
		token_type = TOKEN_SQUOTE;
	else
		exit_program(collector, "minishell: unknown quote type", false);
	quote_str[0] = quote_type;
	quote_str[1] = '\0';
	token_create(collector, token_type, quote_str, head);
	quoted = ft_strndup(input + start, *i - start);
	if (!quoted)
		exit_program(collector, "Error malloc get value for token", EXIT_FAILURE);
	token_create(collector, TOKEN_WORD, quoted, head);
	freer(quoted);
	token_create(collector, token_type, quote_str, head);
	(*i)++;
	//printf("OUT get_quoted_str\n");
}

void	get_word(const char *input, t_collector **collector, int *i, t_token **head)
{
	int		istart;
	char	*value;

	//printf("IN get_word\n");
	istart = *i;
	while (input[*i]
		&& (ft_strchr(" \f\r\n\t\v|<>;'\"\\", input[*i]) == NULL))
		// && (ft_strchr(" \f\r\n\t\v|<>$;'\"\\", input[*i]) == NULL))
		(*i)++;
	value = ft_strndup(input + istart, *i - istart);
	//printf("get_word-value:%s\n", value);
	if (!value)
		exit_program(collector, "Error malloc get value for token", EXIT_FAILURE);
	token_create(collector, TOKEN_WORD, value, head);
	freer(value);
	//printf("OUT get_word\n");
}
