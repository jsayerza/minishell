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
