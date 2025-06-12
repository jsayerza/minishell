/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 redirects_utils01.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:14:40 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:14:40 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_append_permissions_no_delete(char **files)
{
	int	fd;
	int	i;

	if (!files)
		return (1);
	i = 0;
	while (files[i])
	{
		fd = open(files[i], O_WRONLY | O_CREAT, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(files[i], STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			return (0);
		}
		close(fd);
		i++;
	}
	return (1);
}

int	validate_all_output_redirections_no_delete(t_const *node)
{
	if (!node)
		return (1);
	if (!validate_output_permissions_no_delete(node->redirect_out))
		return (0);
	if (!validate_append_permissions_no_delete(node->redirect_append))
		return (0);
	return (1);
}

int	validate_file_array(char **files, int flags)
{
	int	fd;
	int	i;

	if (!files)
		return (1);
	i = 0;
	while (files[i])
	{
		fd = open(files[i], flags, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(files[i], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			return (0);
		}
		close(fd);
		i++;
	}
	return (1);
}

int	validate_all_output_redirections(t_const *node)
{
	if (!node)
		return (1);
	if (!validate_file_array(node->redirect_out, O_WRONLY
			| O_CREAT | O_TRUNC))
		return (0);
	if (!validate_file_array(node->redirect_append, O_WRONLY
			| O_CREAT | O_APPEND))
		return (0);
	return (1);
}
