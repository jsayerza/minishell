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
		{
			pipe(current->fd);
			printf("Creacion de pipe\n");
		}
		current = current->next;
	}
}

void	wait_for_all_processes(t_shell *shell)
{
	t_constructor	*current;
	int				status;

	current = shell->constructor;
	while (current)
	{
		if (current->pid > 0)
		{
			waitpid(current->pid, &status, 0);
			if (WIFEXITED(status) && current->next == NULL)  // Get status from last command
				shell->last_exit = WEXITSTATUS(status);
		}
		current = current->next;
	}
}

void	display_shell(t_shell *shell)
{
	t_constructor	*current_node;

	assign_pipes(shell);
	current_node = shell->constructor;
	
	// First, launch all processes
	while (current_node)
	{
		current_node->shell = shell;
		if (current_node->builtin)
			token_builtins(current_node);
		else if (current_node->type == TOKEN_COMMAND)
			token_commands(current_node);
		current_node = current_node->next;
	}
	
	// Then, wait for all processes to complete
	wait_for_all_processes(shell);
}