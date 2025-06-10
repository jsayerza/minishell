/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 23:19:27 by acarranz          #+#    #+#             */
/*   Updated: 2025/06/10 18:43:33 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Nueva función para manejar errores específicos
static int	handle_command_error(t_const *node, char *path, int error_code)
{
	if (!path)
	{
		if (error_code == 127)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(node->executable[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			node->shell->last_exit = 127;
		}
		else if (error_code == 126)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(node->executable[0], STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			node->shell->last_exit = 126;
		}
		else if (error_code == 2)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(node->executable[0], STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			node->shell->last_exit = 2;
		}
		return (1);
	}
	return (0);
}

void	execute_command(t_const *node)
{
	char	*path;
	int		error_code;

	if (node->type != TOKEN_COMMAND || !node->executable
		|| !node->executable[0])
		return ;

	path = acces_path_with_error(node, &error_code);
	if (handle_command_error(node, path, error_code))
		return ;

	execute_command_with_path(node, path, NULL);
}

void	execute_first_command(t_const *node)
{
	char	*path;
	int		error_code;

	if (node->type != TOKEN_COMMAND || !node->executable
		|| !node->executable[0])
		return ;

	path = acces_path_with_error(node, &error_code);
	if (handle_command_error(node, path, error_code))
		return ;

	if (!execute_command_with_path(node, path, setup_first_command_pipes))
		return ;
}

void	execute_middle_command(t_const *node)
{
	char	*path;
	int		error_code;

	if (node->type != TOKEN_COMMAND || !node->executable
		|| !node->executable[0])
		return ;

	path = acces_path_with_error(node, &error_code);
	if (handle_command_error(node, path, error_code))
		return ;

	if (!execute_command_with_path(node, path, setup_middle_command_pipes))
		return ;
}

void	execute_last_command(t_const *node)
{
	char	*path;
	int		error_code;

	if (node->type != TOKEN_COMMAND || !node->executable
		|| !node->executable[0])
		return ;

	path = acces_path_with_error(node, &error_code);
	if (handle_command_error(node, path, error_code))
		return ;

	if (!execute_command_with_path(node, path, setup_last_command_pipes))
		return ;
}

void	token_commands(t_const *node)
{
	if (node->type != TOKEN_COMMAND)
		return ;
	if (node->pipe_in == 0 && node->pipe_out == 0)
		execute_command(node);
	else if (node->pipe_in == 0 && node->pipe_out == 1)
		execute_first_command(node);
	else if (node->pipe_in == 1 && node->pipe_out == 1)
		execute_middle_command(node);
	else if (node->pipe_in == 1 && node->pipe_out == 0)
		execute_last_command(node);
}
