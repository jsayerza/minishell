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

void	display_shell(t_shell *shell)
{
	t_constructor	*current_node;

	current_node = shell->constructor;
	while (current_node)
	{
		current_node->shell = shell;
		if (current_node->type == TOKEN_BUILTIN)
			token_builtins(current_node);
		current_node = current_node->next;
	}
}
