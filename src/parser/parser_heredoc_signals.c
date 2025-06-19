/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 parser_heredoc_signals.c							:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/06/19 20:20:38 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/06/19 20:20:38 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"

char	*heredoc_read_child(const char *delim, t_collector **collector)
{
	char		*line;
	char		*heredoc;
	size_t		total_len;

	heredoc = NULL;
	total_len = 0;
	while (1)
	{
		line = heredoc_read_line();
		if (!line)
			break ;
		if (heredoc_should_break(line, delim))
			break ;
		heredoc_update_vars(&heredoc, line, &total_len, collector);
		freer((void **)&line);
	}
	if (!heredoc)
		return (ft_strdup(""));
	return (heredoc);
}

static char	*heredoc_init_buffer(char **buffer)
{
	char	*result;

	*buffer = malloc(1024);
	result = malloc(1);
	result[0] = '\0';
	return (result);
}

static char	*heredoc_append_buffer(char *result, char *buffer, int *total_size)
{
	char	*temp;
	int		bytes_len;

	bytes_len = ft_strlen(buffer);
	temp = malloc(*total_size + bytes_len);
	if (result[0])
		ft_strlcpy(temp, result, *total_size + bytes_len);
	else
		temp[0] = '\0';
	ft_strlcat(temp, buffer, *total_size + bytes_len);
	free(result);
	*total_size += bytes_len;
	return (temp);
}

static char	*read_heredoc_from_pipe(int fd)
{
	char	*buffer;
	char	*result;
	int		bytes_read;
	int		total_size;

	result = heredoc_init_buffer(&buffer);
	total_size = 1;
	bytes_read = read(fd, buffer, 1023);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		result = heredoc_append_buffer(result, buffer, &total_size);
	}
	free(buffer);
	return (result);
}

char	*heredoc_read(const char *delim, t_collector **collector)
{
	pid_t	pid;
	int		pipe_fd[2];
	char	*heredoc;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (ft_strdup(""));
	pid = fork();
	if (pid == 0)
		heredoc_child_process(pipe_fd, delim, collector);
	close(pipe_fd[1]);
	heredoc = read_heredoc_from_pipe(pipe_fd[0]);
	close(pipe_fd[0]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		free(heredoc);
		return (ft_strdup(""));
	}
	return (heredoc);
}
