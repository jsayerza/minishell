/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lexer_funcs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/03/08 10:47:17 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_expand_var(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	int		start;
	char	*var_name;
	char	*value;
	char	*expanded;

	start = ++(*i);
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	var_name = ft_strndup(input + start, *i - start);
	if (!var_name)
		exit_program(collector, "Error malloc get value for token", true);
	value = getenv(var_name);
	free(var_name);
	var_name = NULL;
	if (value)
		expanded = ft_strdup(value);
	else
		expanded = ft_strdup("");
	if (!expanded)
		exit_program(collector, "Error malloc get value for token", true);
	token_create(collector, TOKEN_WORD, expanded, head);
	free(expanded);
	expanded = NULL;
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
		exit_program(collector, "Error malloc get value for token", true);
	(*i)++;
	token_create(collector, TOKEN_WORD, quoted, head);
	free(quoted);
}

void	get_operator(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	e_token_type	type;
	char			op[3];

	op[0] = input[*i];
	op[1] = '\0';
	op[2] = '\0';
	if ((ft_strchr("<>", input[*i]) != NULL)
		&& input[*i + 1] == input[*i])
	{
		op[1] = input[*i];
		(*i)++;
	}
	type = TOKEN_PIPE;
	if ((ft_strcmp(op, ">>") == 0))
		type = TOKEN_APPEND;
	else if ((ft_strcmp(op, "<<") == 0))
		type = TOKEN_HEREDOC;
	else if (op[0] == '>')
		type = TOKEN_REDIRECT_OUT;
	else
		type = TOKEN_REDIRECT_IN;
	token_create(collector, type, op, head);
}

void	get_word(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	int		istart;
	char	*value;

	istart = *i;
	while (input[*i]
		&& (ft_strchr(" \f\r\n\t\v|<>'\"$", input[*i]) == NULL))
		(*i)++;
	value = ft_strndup(input + istart, *i - istart);
	if (!value)
		exit_program(collector, "Error malloc get value for token", true);
	token_create(collector, TOKEN_WORD, value, head);
	free(value);
}
