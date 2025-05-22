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

void process_command_nodes(t_shell *shell)
{
	t_constructor *current;

	current = shell->constructor;
	while (current)
	{
		if (current->type == TOKEN_COMMAND)
		{
			if (current->builtin )
				token_builtins(current);
			else
				token_commands(current);
		}
		current = current->next;
	}
}

void	display_shell(t_shell *shell)
{
	assign_pipes(shell);
	process_command_nodes(shell);
	close_remaining_pipes(shell);
	wait_for_all_processes(shell);
	printf("last exit->%d", shell->last_exit);
}
