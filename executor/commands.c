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

int	is_empty_or_whitespace_command(char *cmd)
{
	int	i;

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

static void	handle_child_process(t_const *node, char *path, int error_code,
		void (*setup_pipes)(t_const *))
{
	setup_child_signals();
	if (setup_pipes)
		setup_pipes(node);
	apply_redirections(node);
	if (!path)
	{
		handle_child_command_error(node, error_code);
		if (error_code == 127)
			exit(127);
		else
			exit(126);
	}
	execute_in_child(node, path);
	exit(1);
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
		handle_child_process(node, path, error_code, setup_pipes);
	free(path);
	return (1);
}

static void	execute_command_generic(t_const *node,
		void (*setup_pipes)(t_const *))
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
			if (setup_pipes)
				setup_pipes(node);
			exit(0);
		}
		return ;
	}
	path = acces_path_with_error(node, &error_code);
	execute_command_with_path(node, path, error_code, setup_pipes);
}

void	token_commands(t_const *node)
{
	if (node->type != TOKEN_COMMAND)
		return ;
	if (node->pipe_in == 0 && node->pipe_out == 0)
		execute_command_generic(node, NULL);
	else if (node->pipe_in == 0 && node->pipe_out == 1)
		execute_command_generic(node, setup_first_command_pipes);
	else if (node->pipe_in == 1 && node->pipe_out == 1)
		execute_command_generic(node, setup_middle_command_pipes);
	else if (node->pipe_in == 1 && node->pipe_out == 0)
		execute_command_generic(node, setup_last_command_pipes);
}
