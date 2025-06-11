/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 commands_utils00.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:24 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:24 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

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
		if (stat(node->executable[0], &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
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

// Función simplificada que valida y aplica redirecciones
int	validate_and_apply_redirections(t_const *node)
{
	// Validar redirecciones de entrada primero
	if (!validate_all_input_redirections(node))
	{
		// Salir con exit code 1 cuando falla redirección de entrada
		node->shell->last_exit = 1;
		return (0);
	}
	
	// Validar redirecciones de salida SIN eliminar archivos
	if (!validate_all_output_redirections_no_delete(node))
	{
		node->shell->last_exit = 1;
		return (0);
	}
	apply_all_redirections(node);
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
		if (!path)
		{
			handle_child_command_error(node, error_code);
			exit(error_code == 127 ? 127 : 126);
		}
		if (!validate_and_apply_redirections(node))
			exit(1);  
		
		execute_in_child(node, path);
	}
	free(path);
	return (1);
}