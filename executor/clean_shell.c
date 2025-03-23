/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 clean_shell.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:30 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:30 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void	clean_path(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->paths)
	{
		while (shell->paths[i])
		{
			free(shell->paths[i]);
			i++;
		}
		free(shell->paths);
	}
}

void	clean_executbale(t_constructor *current)
{
	int	i;

	if (current->executable)
	{
		i = 0;
		while (current->executable[i])
		{
			free(current->executable[i]);
			i++;
		}
		free(current->executable);
	}
}

void	clean_constructor(t_constructor *constructor)
{
	t_constructor	*current;
	t_constructor	*next;

	current = constructor;
	while (current)
	{
		next = current->next;
		clean_executbale(current);
		free(current);
		current = next;
	}
}

void	clean_all(t_shell *shell)
{
	free(shell->pwd);
	free(shell->oldpwd);
	free(shell->output);
	clean_constructor(shell->constructor);
	free(shell);
}

void	clean_shell(t_shell *shell)
{
	int	i;

	if (!shell)
		return ;
	if (shell->paths)
		free_path_array(shell->paths);
	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
		{
			free(shell->env[i]);
			i++;
		}
		free(shell->env);
	}
	if (shell->export)
	{
		i = 0;
		while (shell->export[i])
		{
			free(shell->export[i]);
			i++;
		}
		free(shell->export);
	}
	clean_path(shell);
}
