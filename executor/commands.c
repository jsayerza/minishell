/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 commands.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/04/05 23:19:27 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/04/05 23:19:27 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_command(t_const *node)
{
	char	*path;

	if (node->type != TOKEN_COMMAND || !node->executable
		|| !node->executable[0])
		return ;
	path = acces_path(node);
	if (handle_command_not_found(node, path))
		return ;
	execute_command_with_path(node, path, NULL);
}

void	execute_first_command(t_const *node)
{
	char	*path;

	if (node->type != TOKEN_COMMAND || !node->executable
		|| !node->executable[0])
		return ;
	path = acces_path(node);
	if (handle_command_not_found(node, path))
		return ;
	execute_command_with_path(node, path, setup_first_command_pipes);
}

void	execute_middle_command(t_const *node)
{
	char	*path;

	if (node->type != TOKEN_COMMAND || !node->executable
		|| !node->executable[0])
		return ;
	path = acces_path(node);
	if (handle_command_not_found(node, path))
		return ;
	execute_command_with_path(node, path, setup_middle_command_pipes);
}

void	execute_last_command(t_const *node)
{
	char	*path;

	if (node->type != TOKEN_COMMAND || !node->executable
		|| !node->executable[0])
		return ;
	path = acces_path(node);
	if (handle_command_not_found(node, path))
		return ;
	execute_command_with_path(node, path, setup_last_command_pipes);
}

void	token_commands(t_const *node)
{
	if (node->type != TOKEN_COMMAND)
		return ;
	if (node->pipe_in == 0 && node->pipe_out == 0)
		execute_command(node);
	else if (node->pipe_in == 0 && node->pipe_out == 1)
		execute_first_command(node);
	else if (node->pipe_in == 1 && node->pipe_out == 1)
		execute_middle_command(node);
	else if (node->pipe_in == 1 && node->pipe_out == 0)
		execute_last_command(node);
}
