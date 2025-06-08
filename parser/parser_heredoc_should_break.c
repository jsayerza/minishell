/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_should_break.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 11:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/06/07 10:55:37 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_delimiter(char *line, const char *delim)
{
	if (ft_strcmp(line, delim) == 0)
	{
		freer(line);
		return (true);
	}
	return (false);
}

int	heredoc_should_break(char *line, const char *delim)
{
	if (!line)
		return (1);
	remove_trailing_newline(line);
	if (is_delimiter(line, delim))
		return (1);
	return (0);
}
