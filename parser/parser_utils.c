/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:20:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_token_invalid(t_token *prev, t_token *curr)
{
	if (!curr)
		return (0);
	// Caso 1: primer token inválido
	if (!prev && curr->type != TOKEN_WORD
		&& curr->type != TOKEN_REDIRECT_IN
		&& curr->type != TOKEN_REDIRECT_OUT
		&& curr->type != TOKEN_HEREDOC
		&& curr->type != TOKEN_APPEND)
	{
		print_error("minishell: syntax error near unexpected token at start");
		return (1);
	}
	// Caso 2: pipe mal colocado
	if (curr->type == TOKEN_PIPE)
	{
		if (!prev || prev->type == TOKEN_PIPE)
		{
			print_error("minishell: syntax error near unexpected token `|`");
			return (1);
		}
		if (!curr->next || curr->next->type == TOKEN_PIPE)
		{
			print_error("minishell: syntax error near unexpected token `|`");
			return (1);
		}
	}
	// Caso 3: redirecciones mal colocadas
	if (curr->type >= TOKEN_REDIRECT_IN && curr->type <= TOKEN_APPEND)
	{
		if (!curr->next)
		{
			print_error("minishell: syntax error near unexpected token `newline`");
			return (1);
		}
		if (curr->next->type >= TOKEN_REDIRECT_IN && curr->next->type <= TOKEN_APPEND)
		{
			printf("minishell: syntax error near unexpected token `%s`\n", curr->next->value);
			return (1);
		}
	}
	return (0);
}

int	tokens_validate(t_token *tokens)
{
	t_token	*curr;
	t_token	*prev;

	curr = tokens;
	prev = NULL;
	while (curr)
	{
		if (is_token_invalid(prev, curr))
			return (0);
		prev = curr;
		curr = curr->next;
	}
	if (prev && (prev->type == TOKEN_PIPE
		|| prev->type == TOKEN_REDIRECT_IN
		|| prev->type == TOKEN_REDIRECT_OUT
		|| prev->type == TOKEN_HEREDOC
		|| prev->type == TOKEN_APPEND))
	{
		print_error("minishell: syntax error near unexpected token `newline`");
		return (0);
	}
	return (1);
}

void	remove_trailing_newline(char *line)
{
	size_t	len;

	if (!line)
		return ;
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

int	is_assignment(const char *str)
{
	int	i;

	i = 0;
	if (!str || !ft_isalpha(str[0]))
		return (0);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (str[i] == '=' && i > 0);
}
