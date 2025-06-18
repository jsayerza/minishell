/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 echo.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/04/29 12:00:00 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/06/12 17:00:00 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	execute_echo(char **args)
{
	int	jump;
	int	i;

	jump = 0;
	i = 1;
	if (!args || !args[0])
		return ;
	while (args[i] && is_valid_n_flag(args[i]))
	{
		jump = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!jump)
		ft_putstr_fd("\n", 1);
}

void	echo_with_redirections(t_const *node)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("Error al crear el proceso hijo para echo");
		node->shell->last_exit = 1;
		return ;
	}
	if (node->pid == 0)
	{
		setup_child_signals();
		if (node->pipe_in == 0 && node->pipe_out == 1)
			setup_first_command_pipes(node);
		else if (node->pipe_in == 1 && node->pipe_out == 1)
			setup_middle_command_pipes(node);
		else if (node->pipe_in == 1 && node->pipe_out == 0)
			setup_last_command_pipes(node);
		apply_redirections(node);
		execute_echo(node->executable);
		exit (0);
	}
}

void	echo(t_const *node)
{
	if (node->pipe_in || node->pipe_out || node->redirect_out
		|| node->redirect_append || node->redirect_in)
		echo_with_redirections(node);
	else
		execute_echo(node->executable);
	node->shell->last_exit = 0;
}
