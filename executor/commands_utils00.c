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

int	execute_command_with_path(t_const *node, char *path,
	void (*setup_pipes)(t_const *))
{
	if (!validate_all_input_redirections(node))
	{
		node->shell->last_exit = 1;
		free(path);
		return (0);
	}
	if (!validate_all_output_redirections(node))
	{
		node->shell->last_exit = 1;
		free(path);
		return (0);
	}
	node->pid = fork();
	if (handle_fork_error(node, path))
		return (0);
	if (node->pid == 0)
	{
		setup_child_signals();
		if (setup_pipes)
			setup_pipes(node);
		execute_in_child(node, path);
	}
	free(path);
	return (1);
}
