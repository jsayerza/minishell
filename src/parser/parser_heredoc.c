/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/06/19 20:15:34 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*heredoc_read_line(void)
{
	char	*line;

	line = readline("> ");
	return (line);
}

static void	heredoc_read_ctrl(char *heredoc, char *heredoc_old, char *line,
	t_collector **collector)
{
	if (heredoc_old)
	{
		freer((void **)&heredoc_old);
		heredoc_old = NULL;
	}
	if (!heredoc)
	{
		freer((void **)&line);
		exit_program(collector, "Error reading heredoc cont", EXIT_FAILURE);
	}
}

static char	*heredoc_read_new(char *line, char *heredoc,
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

void	heredoc_update_vars(char **heredoc, char *line,
	size_t *len, t_collector **collector)
{
	size_t		line_len;
	char		*old;

	line_len = ft_strlen(line);
	old = *heredoc;
	*heredoc = heredoc_read_new(line, *heredoc, line_len, *len);
	heredoc_read_ctrl(*heredoc, old, line, collector);
	*len += line_len + 1;
}

void	heredoc_child_process(int *pipe_fd, const char *delim,
		t_collector **collector)
{
	char	*heredoc;

	close(pipe_fd[0]);
	setup_child_signals();
	signal(SIGQUIT, SIG_IGN);
	heredoc = heredoc_read_child(delim, collector);
	write(pipe_fd[1], heredoc, ft_strlen(heredoc));
	close(pipe_fd[1]);
	if (heredoc)
		free(heredoc);
	exit(0);
}
