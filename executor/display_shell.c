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
		if (current->pid > 0)
		{
			waitpid(current->pid, &status, 0);
			if (WIFEXITED(status) && current->next == NULL)
				shell->last_exit = WEXITSTATUS(status);
		}
		current = current->next;
	}
}

void	display_shell(t_shell *shell)
{
	t_constructor	*current_node;

	printf("IN display_shell\n");
	assign_pipes(shell);
	printf("  1 display_shell\n");
	current_node = shell->constructor;
	while (current_node && current_node->prev)
		current_node = current_node->prev;
	printf("  2 display_shell\n");
	while (current_node)
	{
		current_node->shell = shell;
		printf("  3 display_shell\n");
		if (current_node->builtin)
		{
			printf("  4.1 display_shell\n");
			token_builtins(current_node);
			printf("  4.2 display_shell\n");
		}
		else if (current_node->type == TOKEN_COMMAND)
		{
			printf("  5.1 display_shell\n");
			token_commands(current_node);
			printf("  5.2 display_shell\n");
		}
		current_node = current_node->next;
	}
	printf("  6 display_shell\n");
	close_remaining_pipes(shell);
	printf("  7 display_shell\n");
	wait_for_all_processes(shell);
	printf("OUT display_shell\n");
}