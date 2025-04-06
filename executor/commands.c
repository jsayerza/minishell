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

char	*construct_exec(char *path, char *command)
{
	char	*exec;
	size_t	total_len;

	total_len = ft_strlen(path) + ft_strlen(command) + 2;
	exec = malloc(total_len * sizeof(char));
	if (!exec)
		return (NULL);
	ft_strlcpy(exec, path, total_len);
	ft_strlcat(exec, "/", total_len);
	ft_strlcat(exec, command, total_len);
	return (exec);
}

char	*acces_path(t_constructor *node)
{
	char	*exec;
	int		i;

	i = 0;
	while (node->shell->paths[i])
	{
		exec = (construct_exec(node->shell->paths[i],
					node->shell->constructor->executable[0]));
		if (exec && access(exec, X_OK) == 0)
			return (exec);
		free(exec);
		i++;
	}
	return (NULL);
}

void token_commands(t_constructor *node)
{
	pid_t pid;
	int status;
	char *path;

	path = acces_path(node);
	if (!path)
	{
		printf("Command not found\n");
		return;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Error al crear el proceso hijo");
		free(path);
		return;
	}
	if (pid == 0)
	{
		execve(path, node->executable, node->shell->env);
		perror("Error al ejecutar el comando");
		free(path);
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
		free(path);
	}
}
