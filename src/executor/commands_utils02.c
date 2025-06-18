/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils02.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:13:24 by acarranz          #+#    #+#             */
/*   Updated: 2025/06/17 18:44:23 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_error_message(t_const *node, char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(node->executable[0], STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
}

void	handle_child_command_error(t_const *node, int error_code)
{
	struct stat	path_stat;

	if (error_code == 127)
		print_error_message(node, ": command not found\n");
	else if (error_code == 126)
	{
		if (stat(node->executable[0], &path_stat) == 0
			&& S_ISDIR(path_stat.st_mode))
			print_error_message(node, ": Is a directory\n");
		else
			print_error_message(node, ": Permission denied\n");
	}
	else if (error_code == 2)
		print_error_message(node, ": No such file or directory\n");
}

static int	validate_files_array(char **files, int flags)
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
			return (0);
		close(fd);
		i++;
	}
	return (1);
}

int	validate_and_create_output_files(t_const *node)
{
	if (!validate_files_array(node->redirect_out,
			O_WRONLY | O_CREAT | O_TRUNC))
		return (0);
	if (!validate_files_array(node->redirect_append,
			O_WRONLY | O_CREAT | O_APPEND))
		return (0);
	return (1);
}
