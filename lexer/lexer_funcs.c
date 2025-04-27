/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_funcs.c                                :+:      :+:    :+:   */
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
		// return (input[i]);
	}
	return (0);
}

void	get_expand_var(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	int		start;
	char	*var_name;
	char	*value;
	char	*expanded;

	start = ++(*i);
	while (ft_isalnum(input[*i]) || input[*i] == '_')
	{
		printf("get_expand_var-input[%d]:%c\n", *i, input[*i]);
		(*i)++;
	}
	var_name = ft_strndup(input + start, *i - start);
	if (!var_name)
		exit_program(collector, "Error malloc get value token", EXIT_FAILURE);
	value = getenv(var_name);
	freer(var_name);
	if (value)
		expanded = ft_strdup(value);
	else
		expanded = ft_strdup("");
	if (!expanded)
		exit_program(collector, "Error malloc get value token", EXIT_FAILURE);
	token_create(collector, TOKEN_WORD, expanded, head);
	freer(expanded);
}

void	get_quoted_str(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	int		start;
	char	*quoted;
	char	quote_type;

	quote_type = input[*i];
	start = ++(*i);
	while (input[*i] && input[*i] != quote_type)
		(*i)++;
	quoted = ft_strndup(input + start, *i - start);
	if (!quoted)
		exit_program(collector, \
			"Error malloc get value for token", EXIT_FAILURE);
	(*i)++;
	token_create(collector, TOKEN_WORD, quoted, head);
	freer(quoted);
}

void	get_word(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	int		istart;
	char	*value;

	istart = *i;
	while (input[*i]
		&& (ft_strchr(" \f\r\n\t\v|<>$;'\"\\", input[*i]) == NULL))
		(*i)++;
	value = ft_strndup(input + istart, *i - istart);
	// printf("get_word-value:%s\n", value);
	if (!value)
		exit_program(collector, \
			"Error malloc get value for token", EXIT_FAILURE);
	token_create(collector, TOKEN_WORD, value, head);
	freer(value);
}
