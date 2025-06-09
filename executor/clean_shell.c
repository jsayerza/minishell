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

static void	clean_executable(t_constructor *current)
{
	int	i;

	if (!current || !current->executable)
		return ;
	i = 0;
	while (current->executable[i])
		free(current->executable[i++]);
	free(current->executable);
	current->executable = NULL;
}

void	clean_constructor(t_constructor *constructor)
{
	t_constructor	*current;
	t_constructor	*next;

	if (!constructor)
		return ;
	current = constructor;
	while (current)
	{
		next = current->next;
		clean_executable(current);
		free(current);
		current = next;
	}
}

void	clean_paths(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->path)
	{
		free(shell->path);
		shell->path = NULL;
	}
	if (shell->paths)
	{
		free_path_array(shell->paths);
		shell->paths = NULL;
	}
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->output)
		free(shell->output);
	free(shell);
}

void	clean_shell(t_shell *shell)
{
	int	i;

	if (!shell)
		return ;
	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
			free(shell->env[i++]);
		free(shell->env);
		shell->env = NULL;
	}
	if (shell->export)
	{
		i = 0;
		while (shell->export[i])
			free(shell->export[i++]);
		free(shell->export);
		shell->export = NULL;
	}
	clean_paths(shell);
	clean_constructor(shell->constructor);
	free_shell(shell);
}
