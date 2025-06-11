/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 command_utils01.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:24 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:24 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void	close_all_pipes_except(t_const *node, int keep_in, int keep_out)
{
	t_const	*temp;

	temp = node->shell->constructor;
	while (temp && temp->prev)
		temp = temp->prev;
	while (temp)
	{
		if (temp->pipe_out == 1)
		{
			if (!(keep_in && temp->next == node))
				close(temp->fd[0]);
			if (!(keep_out && temp == node))
				close(temp->fd[1]);
		}
		temp = temp->next;
	}
}

int	handle_command_not_found(t_const *node, char *path)
{
	if (!path)
	{
		ft_putstr_fd("command not found\n", 2);
		node->shell->last_exit = 127;
		return (1);
	}
	return (0);
}

int	handle_fork_error(t_const *node, char *path)
{
	if (node->pid == -1)
	{
		perror("Error al crear el proceso hijo");
		free(path);
		node->shell->last_exit = 1;
		return (1);
	}
	return (0);
}

void	execute_in_child(t_const *node, char *path)
{
	if (node->redirect_in_type == 6)
		check_heredoc(node);
	add_or_update_env_var(&(node->shell->env), "_", path);
	execve(path, node->executable, node->shell->env);
	perror("Error al ejecutar el comando");
	exit(1);
}