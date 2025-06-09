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

int	handle_whitespace(const char *input, int *i)
{
	if (ft_strchr(" \f\r\n\t\v", input[*i]))
	{
		(*i)++;
		return (1);
	}
	return (0);
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
