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

static void	remove_trailing_newline(char *line)
{
	size_t	len;

	if (!line)
		return ;
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

static char	*heredoc_read_new(char *line, char *heredoc, \
	size_t line_len, size_t total_len)
{
	char	*new_heredoc;
	size_t	k;
	size_t	i;

	new_heredoc = malloc(total_len + line_len + 2);
	if (!new_heredoc)
		return (free(heredoc), NULL);
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

static void	heredoc_read_ctrl(char *heredoc, char *line, \
	t_collector **collector)
{
	if (!heredoc)
	{
		free(line);
		exit_program(collector, "Error reading heredoc cont", EXIT_FAILURE);
	}
	collector_append(collector, heredoc);
}

char	*heredoc_read(const char *delim, int interact, t_collector **collector)
{
	char	*line;
	char	*heredoc;
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
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		line_len = ft_strlen(line);
		heredoc = heredoc_read_new(line, heredoc, line_len, total_len);
		heredoc_read_ctrl(heredoc, line, collector);
		total_len += line_len + 1;
		free(line);
	}
	return (heredoc);
}
