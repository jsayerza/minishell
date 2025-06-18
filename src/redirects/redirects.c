/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 redirects.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:14:40 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:14:40 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	apply_input_redirection(t_const *node)
{
	int	fd;
	int	i;

	if (!node || !node->redirect_in)
		return ;
	i = 0;
	while (node->redirect_in[i])
	{
		fd = open(node->redirect_in[i], O_RDONLY);
		if (fd < 0)
		{
			perror("open");
			exit(1);
		}
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			close(fd);
			exit(1);
		}
		close(fd);
		i++;
	}
}

void	apply_output_redirection(t_const *node)
{
	int	fd;
	int	i;

	if (!node || !node->redirect_out)
		return ;
	i = 0;
	while (node->redirect_out[i] && node->redirect_out[i + 1])
		i++;
	if (node->redirect_out[i])
	{
		fd = open(node->redirect_out[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("open");
			exit(1);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2");
			close(fd);
			exit(1);
		}
		close(fd);
	}
}

void	apply_append_redirection(t_const *node)
{
	int	fd;
	int	i;

	if (!node || !node->redirect_append)
		return ;
	i = 0;
	while (node->redirect_append[i] && node->redirect_append[i + 1])
		i++;
	if (node->redirect_append[i])
	{
		fd = open(node->redirect_append[i], O_WRONLY
				| O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("open");
			exit(1);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2");
			close(fd);
			exit(1);
		}
		close(fd);
	}
}

void	apply_all_redirections(t_const *node)
{
	if (!node)
		return ;
	apply_input_redirection(node);
	if (node->redirect_out_type == TOKEN_REDIRECT_OUT)
		apply_output_redirection(node);
	else if (node->redirect_out_type == TOKEN_APPEND)
		apply_append_redirection(node);
}
