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

void	display_shell(t_shell *shell)
{
	t_constructor	*current_node;

	assign_pipes(shell);
	current_node = shell->constructor;
	while (current_node)
	{
		current_node->shell = shell;
		if (current_node->builtin)
			token_builtins(current_node);
		else if (current_node->type == TOKEN_COMMAND)
			token_commands(current_node);
		current_node = current_node->next;
	}
}
