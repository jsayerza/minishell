/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_read_line(int interact)
{
	char	*line;

	if (interact)
		line = readline("> ");
	else
		line = get_next_line(STDIN_FILENO);
	return (line);
}

static void	heredoc_read_ctrl(char *heredoc, char *heredoc_old, char *line, \
	t_collector **collector)
{
	if (heredoc_old)
	{
		freer(heredoc_old);
		heredoc_old = NULL;
	}
	if (!heredoc)
	{
		freer(line);
		exit_program(collector, "Error reading heredoc cont", EXIT_FAILURE);
	}
}

static char	*heredoc_read_new(char *line, char *heredoc, \
	size_t line_len, size_t total_len)
{
	char	*new_heredoc;
	size_t	k;
	size_t	i;

	new_heredoc = malloc(total_len + line_len + 2);
	if (!new_heredoc)
		return (NULL);
	k = 0;
	if (heredoc)
	{
		while (heredoc[k])
		{
			new_heredoc[k] = heredoc[k];
			k++;
		}
	}
	i = 0;
	while (i < line_len)
		new_heredoc[k++] = line[i++];
	new_heredoc[k++] = '\n';
	new_heredoc[k] = '\0';
	return (new_heredoc);
}

int	is_delimiter(char *line, const char *delim)
{
	if (ft_strcmp(line, delim) == 0)
	{
		freer(line);
		return (true);
	}
	return (false);
}

char	*heredoc_read(const char *delim, int interact, t_collector **collector)
{
	char	*line;
	char	*heredoc;
	char	*heredoc_old;
	size_t	total_len;
	size_t	line_len;

	heredoc = NULL;
	total_len = 0;
	while (1)
	{
		line = heredoc_read_line(interact);
		if (!line)
			break ;
		remove_trailing_newline(line);
		if (is_delimiter(line, delim))
			break ;
		line_len = ft_strlen(line);
		heredoc_old = heredoc;
		heredoc = heredoc_read_new(line, heredoc, line_len, total_len);
		heredoc_read_ctrl(heredoc, heredoc_old, line, collector);
		total_len += line_len + 1;
		freer(line);
	}
	return (heredoc);
}
