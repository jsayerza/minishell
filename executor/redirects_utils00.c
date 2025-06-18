/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 redirects_utils00.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:14:40 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:14:40 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_all_input_redirections(t_const *node)
{
	int	i;

	if (!node || !node->redirect_in)
		return (1);
	i = 0;
	while (node->redirect_in[i])
	{
		if (access(node->redirect_in[i], F_OK) != 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(node->redirect_in[i], STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			return (0);
		}
		if (access(node->redirect_in[i], R_OK) != 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(node->redirect_in[i], STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			return (0);
		}
		i++;
	}
	return (1);
}

int	validate_output_permissions_no_delete(char **files)
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
