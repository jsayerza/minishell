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

void	get_expand_var(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	int		start;
	char	*var_name;
	char	*value;
	char	*expanded;

	printf("IN get_expand_var\n");
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
	printf("OUT get_expand_var\n");
}

// void	get_quoted_str(const char *input, t_collector **collector, int *i, t_token **head)
// {
// 	int		start;
// 	char	*quoted;
// 	char	quote_type;

// 	printf("IN get_quoted_str\n");
// 	quote_type = input[*i];
// 	start = ++(*i);
// 	while (input[*i] && input[*i] != quote_type)
// 		(*i)++;
// 	if (!input[*i]) // No s'ha trobat "'" o """
// 		exit_program(collector, "minishell: unclosed quotes", false);
// 	quoted = ft_strndup(input + start, *i - start);
// 	if (!quoted)
// 		exit_program(collector, "Error malloc get value for token", EXIT_FAILURE);
// 	(*i)++;
// 	token_create(collector, TOKEN_WORD, quoted, head);
// 	freer(quoted);
// 	printf("OUT get_quoted_str\n");
// }

void	get_quoted_str(const char *input, t_collector **collector, int *i, t_token **head)
{
	int		start;
	char	*quoted;
	char	quote_str[2];
	char	quote_type;

	printf("IN get_quoted_str\n");
	quote_type = input[*i];
	start = ++(*i);
	while (input[*i] && input[*i] != quote_type)
		(*i)++;
	if (!input[*i])
		exit_program(collector, "minishell: unclosed quotes", false);
	if (quote_type == '"')
	{
		quote_str[0] = '"';
		quote_str[1] = '\0';
		token_create(collector, TOKEN_DQUOTE, quote_str, head);
	}
	quoted = ft_strndup(input + start, *i - start);
	if (!quoted)
		exit_program(collector, "Error malloc get value for token", EXIT_FAILURE);
	token_create(collector, TOKEN_WORD, quoted, head);
	freer(quoted);
	if (quote_type == '"')
	{
		quote_str[0] = '"';
		quote_str[1] = '\0';
		token_create(collector, TOKEN_DQUOTE, quote_str, head);
	}
	(*i)++;
	printf("OUT get_quoted_str\n");
}

void	get_word(const char *input, t_collector **collector, int *i, t_token **head)
{
	int		istart;
	char	*value;

	printf("IN get_word\n");
	istart = *i;
	while (input[*i]
		&& (ft_strchr(" \f\r\n\t\v|<>;'\"\\", input[*i]) == NULL))
		// && (ft_strchr(" \f\r\n\t\v|<>$;'\"\\", input[*i]) == NULL))
		(*i)++;
	value = ft_strndup(input + istart, *i - istart);
	printf("get_word-value:%s\n", value);
	if (!value)
		exit_program(collector, "Error malloc get value for token", EXIT_FAILURE);
	token_create(collector, TOKEN_WORD, value, head);
	freer(value);
	printf("OUT get_word\n");
}

// void	get_word(const char *input, t_collector **collector, int *i, t_token **head)
// {
// 	int		start;
// 	char	*value;
// 	char	*tmp;
// 	char	*joined;
// 	char	quote_type;

// 	printf("IN get_word\n");
// 	joined = ft_strdup("");
// 	if (!joined)
// 		exit_program(collector, "Error malloc get_word init", true);
// 	collector_append(collector, joined);

// 	while (input[*i] && !ft_strchr(" \f\r\n\t\v|<>", input[*i]))
// 	{
// 		if (input[*i] == '\'' || input[*i] == '"')
// 		{
// 			quote_type = input[(*i)++];
// 			start = *i;
// 			while (input[*i] && input[*i] != quote_type)
// 				(*i)++;
// 			if (!input[*i])
// 				exit_program(collector, "minishell: syntax error: unmatched quote", false);

// 			value = ft_strndup(input + start, *i - start);
// 			if (!value)
// 				exit_program(collector, "Error malloc get_word quoted", true);
// 			collector_append(collector, value);
// 			tmp = ft_strjoin(joined, value);
// 			if (!tmp)
// 				exit_program(collector, "Error malloc get_word join quoted", true);
// 			collector_append(collector, tmp);
// 			joined = tmp;  // No se libera, sigue en collector
// 			(*i)++;
// 		}
// 		else if (input[*i] == '$')
// 			break;
// 		else
// 		{
// 			start = *i;
// 			while (input[*i] && !ft_strchr(" \f\r\n\t\v|<>'\"$", input[*i]))
// 				(*i)++;
// 			value = ft_strndup(input + start, *i - start);
// 			if (!value)
// 				exit_program(collector, "Error malloc get_word plain", true);
// 			collector_append(collector, value);
// 			tmp = ft_strjoin(joined, value);
// 			if (!tmp)
// 				exit_program(collector, "Error malloc get_word join plain", true);
// 			collector_append(collector, tmp);
// 			joined = tmp;  // No se libera, sigue en collector
// 		}
// 	}
// 	token_create(collector, TOKEN_WORD, joined, head);
// 	printf("OUT get_word\n");
// }
