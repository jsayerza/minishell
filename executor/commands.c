/* ************************************************************************** */
/*  */
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

// Función para verificar si un comando está vacío o es solo espacios
int	is_empty_or_whitespace_command(char *cmd)
{
	int i;
	
	if (!cmd)
		return (1);
	
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

void	execute_command(t_const *node)
{
	char	*path;
	int		error_code;

	if (node->type != TOKEN_COMMAND || !node->executable
		|| !node->executable[0])
		return ;

	if (is_empty_or_whitespace_command(node->executable[0]))
	{
		// Para comandos vacíos, SIEMPRE hacer fork para mantener la pipeline
		node->pid = fork();
		if (node->pid == 0)
		{
			setup_child_signals();
			// Aplicar redirecciones aunque el comando esté vacío
			if (!validate_and_apply_redirections(node))
				exit(1);
			exit(0);  // Comando vacío = exit 0
		}
		return ;
	}

	path = acces_path_with_error(node, &error_code);
	execute_command_with_path(node, path, error_code,  NULL);
}

void	execute_first_command(t_const *node)
{
	char	*path;
	int		error_code;

	if (node->type != TOKEN_COMMAND || !node->executable
		|| !node->executable[0])
		return ;

	if (is_empty_or_whitespace_command(node->executable[0]))
	{
		node->pid = fork();
		if (node->pid == 0)
		{
			setup_child_signals();
			setup_first_command_pipes(node);
			if (!validate_and_apply_redirections(node))
				exit(1);
			exit(0);
		}
		return ;
	}

	path = acces_path_with_error(node, &error_code);
	execute_command_with_path(node, path, error_code,  setup_first_command_pipes);
}

void	execute_middle_command(t_const *node)
{
	char	*path;
	int		error_code;

	if (node->type != TOKEN_COMMAND || !node->executable
		|| !node->executable[0])
		return ;

	if (is_empty_or_whitespace_command(node->executable[0]))
	{
		node->pid = fork();
		if (node->pid == 0)
		{
			setup_child_signals();
			setup_middle_command_pipes(node);
			if (!validate_and_apply_redirections(node))
				exit(1);
			exit(0);
		}
		return ;
	}

	path = acces_path_with_error(node, &error_code);
	execute_command_with_path(node, path, error_code,  setup_middle_command_pipes);
}

void	execute_last_command(t_const *node)
{
	char	*path;
	int		error_code;

	if (node->type != TOKEN_COMMAND || !node->executable
		|| !node->executable[0])
		return ;

	if (is_empty_or_whitespace_command(node->executable[0]))
	{
		node->pid = fork();
		if (node->pid == 0)
		{
			setup_child_signals();
			setup_last_command_pipes(node);
			if (!validate_and_apply_redirections(node))
				exit(1);
			exit(0);
		}
		return ;
	}

	path = acces_path_with_error(node, &error_code);
	execute_command_with_path(node, path, error_code, setup_last_command_pipes);
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