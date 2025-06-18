/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 display_shell.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:14:40 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:14:40 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	assign_pipes(t_shell *shell)
{
	t_const	*current;

	current = shell->constructor;
	while (current)
	{
		if (current->pipe_out == 1)
			pipe(current->fd);
		current = current->next;
	}
}

void	close_pipes(t_shell *shell)
{
	t_const	*current;

	current = shell->constructor;
	while (current)
	{
		if (current->pipe_out == 1)
		{
			close(current->fd[0]);
			close(current->fd[1]);
		}
		current = current->next;
	}
}

void	handle_command_execution(t_const *current)
{
	if (current->type == TOKEN_COMMAND)
	{
		if (current->builtin && ft_strcmp(current->executable[0],
				"env") == 0 && current->executable[1]
			&& ft_strcmp(current->executable[1], "-i") == 0
			&& current->executable[2] && ft_strcmp(current->executable[2],
				"bash") == 0)
			token_commands(current);
		else if (current->builtin)
			token_builtins(current);
		else
			token_commands(current);
	}
}

void	process_commands(t_shell *shell)
{
	t_const	*current;
	pid_t	pid;

	current = shell->constructor;
	while (current)
	{
		if (!validate_redirections(current))
		{
			shell->last_exit = 1;
			if (current->pipe_out == 1 || current->pipe_in == 1)
			{
				pid = fork();
				if (pid == 0)
					exit(1);
				current->pid = pid;
			}
			current = current->next;
		}
		else
		{
			handle_command_execution(current);
			current = current->next;
		}
	}
	close_pipes(shell);
}

void	display_shell(t_shell *shell)
{
	assign_pipes(shell);
	process_commands(shell);
	wait_for_child_processes(shell);
	close_pipes(shell);
}
