/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 heredoc.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:24 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:24 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	create_and_write_pipe(int *pipefd, char *content)
{
	ssize_t	bytes_written;

	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe creation failed");
		exit(1);
	}
	bytes_written = write(pipefd[1], content, strlen(content));
	if (bytes_written == -1)
	{
		perror("minishell: write to pipe failed");
		exit(1);
	}
}

static int	get_last_heredoc_index(char **heredoc)
{
	int	i;

	i = 0;
	while (heredoc[i])
		i++;
	return (i - 1);
}

static void	setup_stdin_redirect(int pipefd)
{
	if (dup2(pipefd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 heredoc");
		close(pipefd);
		exit(1);
	}
	close(pipefd);
}

void	check_heredoc(t_const *node)
{
	int	pipefd[2];
	int	last_index;

	if (!node->heredoc)
		return ;
	signal(SIGINT, SIG_DFL);
	last_index = get_last_heredoc_index(node->heredoc);
	create_and_write_pipe(pipefd, node->heredoc[last_index]);
	close(pipefd[1]);
	setup_stdin_redirect(pipefd[0]);
}
