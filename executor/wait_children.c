/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 wait_children.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:24 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:24 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

static t_const	*find_last_command(t_shell *shell)
{
	t_const	*current;
	t_const	*last_command;

	current = shell->constructor;
	last_command = NULL;
	while (current)
	{
		if (current->type == TOKEN_COMMAND && current->pid > 0)
			last_command = current;
		current = current->next;
	}
	return (last_command);
}

static void	set_exit_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGPIPE)
			shell->last_exit = 0;
		else
			shell->last_exit = 128 + WTERMSIG(status);
	}
}

void	wait_for_child_processes(t_shell *shell)
{
	t_const	*current;
	t_const	*last_command;
	int		status;

	last_command = find_last_command(shell);
	current = shell->constructor;
	while (current && current->prev)
		current = current->prev;
	while (current)
	{
		if (current->pid > 0)
		{
			waitpid(current->pid, &status, 0);
			if (current == last_command)
				set_exit_status(shell, status);
		}
		current = current->next;
	}
}
