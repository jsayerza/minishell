/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 commands_utils00.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:24 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/06/11 XX:XX:XX by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

int	has_output_redirection(t_const *node)
{
	if (!node)
		return (0);
	if ((node->redirect_out && node->redirect_out[0])
		|| (node->redirect_append && node->redirect_append[0]))
		return (1);
	return (0);
}

void	setup_first_command_pipes(t_const *node)
{
	if (node->pipe_out == 1)
		dup2(node->fd[1], STDOUT_FILENO);
	close_all_pipes_except(node, 0, 0);
}

void	setup_middle_command_pipes(t_const *node)
{
	if (node->prev && node->prev->pipe_out == 1)
		dup2(node->prev->fd[0], STDIN_FILENO);
	if (node->pipe_out == 1)
		dup2(node->fd[1], STDOUT_FILENO);
	close_all_pipes_except(node, 0, 0);
}

void	setup_last_command_pipes(t_const *node)
{
	if (node->prev && node->prev->pipe_out == 1)
		dup2(node->prev->fd[0], STDIN_FILENO);
	close_all_pipes_except(node, 0, 0);
}

void	handle_child_command_error(t_const *node, int error_code)
{
	struct stat	path_stat;

	if (error_code == 127)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(node->executable[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else if (error_code == 126)
	{
		if (stat(node->executable[0], &path_stat) == 0
			&& S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(node->executable[0], STDERR_FILENO);
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		}
		else
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(node->executable[0], STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		}
	}
	else if (error_code == 2)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(node->executable[0], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	}
}

int	validate_and_create_output_files(t_const *node)
{
	int	fd;
	int	i;

	if (node->redirect_out)
	{
		i = 0;
		while (node->redirect_out[i])
		{
			fd = open(node->redirect_out[i], O_WRONLY
					| O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(node->redirect_out[i], STDERR_FILENO);
				ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
				return (0);
			}
			close(fd);
			i++;
		}
	}
	if (node->redirect_append)
	{
		i = 0;
		while (node->redirect_append[i])
		{
			fd = open(node->redirect_append[i], O_WRONLY
					| O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(node->redirect_append[i], STDERR_FILENO);
				ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
				return (0);
			}
			close(fd);
			i++;
		}
	}
	return (1);
}

int	validate_and_apply_redirections(t_const *node)
{
	if (!validate_all_output_redirections_no_delete(node))
		return (0);
	if (node->redirect_out_type == TOKEN_REDIRECT_OUT)
		apply_output_redirection(node);
	else if (node->redirect_out_type == TOKEN_APPEND)
		apply_append_redirection(node);
	if (!validate_all_input_redirections(node))
		return (0);
	apply_input_redirection(node);
	return (1);
}

int	execute_command_with_path(t_const *node, char *path, int error_code,
		void (*setup_pipes)(t_const *))
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("Error al crear el proceso hijo");
		free(path);
		node->shell->last_exit = 1;
		return (0);
	}
	if (node->pid == 0)
	{
		setup_child_signals();
		if (setup_pipes)
			setup_pipes(node);
		if (!validate_and_apply_redirections(node))
			exit(1);
		if (!path)
		{
			handle_child_command_error(node, error_code);
			exit(error_code == 127 ? 127 : 126);
		}
		execute_in_child(node, path);
		exit(1);
	}
	free(path);
	return (1);
}
