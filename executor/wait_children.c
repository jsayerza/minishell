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

static void	handle_normal_exit(t_shell *shell, int status)
{
	int	exit_code;

	exit_code = WEXITSTATUS(status);
	shell->last_exit = exit_code;
}

static void	handle_signal_exit(t_shell *shell, int status)
{
	int	sig;

	sig = WTERMSIG(status);
	if (sig == SIGINT)
		shell->last_exit = 130;
	else if (sig == SIGQUIT)
	{
		shell->last_exit = 131;
		if (WCOREDUMP(status))
			ft_putstr_fd("Quit (core dumped)\n", 2);
		else
			ft_putstr_fd("Quit\n", 2);
	}
	else
		shell->last_exit = 128 + sig;
}

static void	process_child_status(t_constructor *current, int status)
{
	if (WIFEXITED(status))
		handle_normal_exit(current->shell, status);
	else if (WIFSIGNALED(status))
		handle_signal_exit(current->shell, status);
	else
		ft_putstr_fd("DEBUG: Error terminacion\n", 2);
}

void	wait_for_child_processes(t_constructor *node)
{
	int				status;
	t_constructor	*current;

	current = node;
	while (current && current->prev)
		current = current->prev;
	while (current)
	{
		if (current->type == TOKEN_COMMAND && current->pid > 0)
		{
			waitpid(current->pid, &status, 0);
			process_child_status(current, status);
		}
		current = current->next;
	}
}
