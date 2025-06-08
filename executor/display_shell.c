/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 display_shell.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:48 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:48 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void	assign_pipes(t_shell *shell)
{
	t_constructor	*current;

	current = shell->constructor;
	while (current)
	{
		if (current->pipe_out == 1)
			pipe(current->fd);
		current = current->next;
	}
}

void	close_remaining_pipes(t_shell *shell)
{
	t_constructor	*current;

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

void	wait_for_all_processes(t_shell *shell)
{
	t_constructor	*current;
	int				status;

	current = shell->constructor;
	while (current && current->prev)
		current = current->prev;
	while (current)
	{
		check_redirect_in_file_exists(current);
		if (current->pid > 0)
		{
			waitpid(current->pid, &status, 0);
			if (WIFEXITED(status) && current->next == NULL)
				shell->last_exit = WEXITSTATUS(status);
		}
		current = current->next;
	}
}

void	close_used_pipes(t_shell *shell)
{
	t_constructor	*current;

	current = shell->constructor;
	while (current && current->prev)
		current = current->prev;
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

void	process_command_nodes(t_shell *shell)
{
	t_constructor	*current;

	current = shell->constructor;
	while (current)
	{
		if (current->type == TOKEN_COMMAND)
		{
			if (current->builtin
				&& ft_strcmp(current->executable[0], "env") == 0)
			{
				if (current->executable[1]
					&& ft_strcmp(current->executable[1], "-i") == 0
					&& current->executable[2]
					&& ft_strcmp(current->executable[2], "bash") == 0)
				{
					current->builtin = 0;
					token_commands(current);
				}
				else
					token_builtins(current);
			}
			else if (current->builtin)
				token_builtins(current);
			else
				token_commands(current);
		}
		current = current->next;
	}
	close_used_pipes(shell);
}

void	display_shell(t_shell *shell)
{
	t_constructor	*current;

	assign_pipes(shell);
	process_command_nodes(shell);
	current = shell->constructor;
	while (current && current->prev)
		current = current->prev;
	while (current)
	{
		wait_for_child_processes(current);
		current = current->next;
	}
	close_remaining_pipes(shell);
}
