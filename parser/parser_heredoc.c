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

static char	*heredoc_read_line(int g_is_interactive)
{
	char *line;

	if (g_is_interactive)
		line = readline("> ");
	else
		line = get_next_line(STDIN_FILENO);
	return (line);
}

static void	remove_trailing_newline(char *line)
{
	size_t	len;

	if (!line)
		return;
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

char	*heredoc_read(const char *delimiter, int g_is_interactive, t_collector **collector)
{
	char	*line;
	char	*heredoc = NULL;
	size_t	total_len = 0;
	size_t	line_len;
	size_t	k;

	while (1)
	{
		line = heredoc_read_line(g_is_interactive);
		if (!line)
			break;
		remove_trailing_newline(line);
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		line_len = ft_strlen(line);
		char *new_heredoc = malloc(total_len + line_len + 2); // +1 \n +1 \0
		if (!new_heredoc)
			return (free(heredoc), NULL);
		collector_append(collector, new_heredoc);
		k = 0;
		if (heredoc)
		{
			while (heredoc[k])
			{
				new_heredoc[k] = heredoc[k];
				k++;
			}
		}
		for (size_t i = 0; i < line_len; i++)
			new_heredoc[k++] = line[i];
		new_heredoc[k++] = '\n';
		new_heredoc[k] = '\0';
		heredoc = new_heredoc;
		total_len += line_len + 1;
		free(line);
	}
	return (heredoc);
}
