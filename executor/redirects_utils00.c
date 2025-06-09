/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 redirects_utils00.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:24 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:24 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void	check_redirect_in_file_exists(t_const *node)
{
	t_const	*current;
	int		fd;
	int		i;

	if (!node)
		return ;
	current = node;
	if (!current->redirect_in)
		return ;
	i = 0;
	while (current->redirect_in[i])
	{
		fd = open(current->redirect_in[i], O_RDONLY);
		if (fd < 0)
		{
			perror("open");
			return ;
		}
		close(fd);
		i++;
	}
}

void	create_output_files(t_const *node)
{
	int	fd;
	int	i;

	if (!node || !node->redirect_out)
		return ;
	i = 0;
	while (node->redirect_out[i])
	{
		if (node->redirect_out[i + 1] != NULL)
		{
			fd = open(node->redirect_out[i], O_WRONLY
					| O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("open");
				return ;
			}
			close(fd);
		}
		i++;
	}
}

void	create_append_files(t_const *node)
{
	int	fd;
	int	i;

	if (!node || !node->redirect_append)
		return ;
	i = 0;
	while (node->redirect_append[i])
	{
		if (node->redirect_append[i + 1] != NULL)
		{
			fd = open(node->redirect_append[i], O_WRONLY
					| O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				perror("open");
				return ;
			}
			close(fd);
		}
		i++;
	}
}
