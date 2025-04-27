/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_funcs_operator.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 11:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_type	get_operator_type(const char *input, int *i)
{
	if (input[*i] == '|')
		return (TOKEN_PIPE);
	else if (input[*i] == '<' && (*i > 0) && input[*i - 1] == '<')
		return (TOKEN_HEREDOC);
	else if (input[*i] == '>' && (*i > 0) && input[*i - 1] == '>')
		return (TOKEN_APPEND);
	else if (input[*i] == '<')
		return (TOKEN_REDIRECT_IN);
	else if (input[*i] == '>')
		return (TOKEN_REDIRECT_OUT);
	else if (input[*i] == '$')
		return (TOKEN_DOLLAR);
	return (999);
}

void	get_operator(const char *input, t_collector **collector, \
	int *i, t_token **head)
{
	t_token_type	type;
	char			op[3];

	op[0] = input[*i];
	op[1] = '\0';
	op[2] = '\0';
	if ((ft_strchr("<>", input[*i]) != NULL)
		&& input[*i + 1] == input[*i])
	{
		op[1] = input[*i + 1];
		(*i)++;
	}
	type = get_operator_type(input, i);
	if (type == 999)
		exit_program(collector, "Error in get_operator", EXIT_FAILURE);
	token_create(collector, type, op, head);
}
