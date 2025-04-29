/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 builtins.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:24 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:24 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void	redirect_builtin(t_constructor *node, char **builtin)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("Error al crear el proceso hijo");
		node->shell->last_exit = 1;
		return ;
	}
	if (pid == 0)
	{
		dup2(node->fd[1], STDOUT_FILENO);
		close(node->fd[0]);
		close(node->fd[1]);
		print_builtin(builtin);
		exit(1);
	}
	else
	{
		close(node->fd[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			node->shell->last_exit = WEXITSTATUS(status);
	}
}

void	token_builtins(t_constructor *node)
{
	printf("Builtins\n");
	if (node->builtin == BUILTIN_EXPORT)
		export(node);
	if (node->builtin == BUILTIN_ENV)
		env(node);
	if (node->builtin == BUILTIN_PWD)
		pwd();
	if (node->builtin == BUILTIN_ECHO)
		echo(node);
	if (node->builtin == BUILTIN_UNSET)
		unset(node);
	if (node->builtin == BUILTIN_CD)
		cd(node);
}
