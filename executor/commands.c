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
		exec = construct_exec(node->shell->paths[i], node->executable[0]);
		if (exec && access(exec, X_OK) == 0)
			return (exec);
		free(exec);
		i++;
	}
	return (NULL);
}

void	close_all_pipes_except(t_constructor *node, int keep_in, int keep_out)
{
	t_constructor	*temp;

	temp = node->shell->constructor;
	while (temp && temp->prev)
		temp = temp->prev;
	while (temp)
	{
		if (temp->pipe_out == 1)
		{
			if (keep_in != 1 || temp != node->prev)
				close(temp->fd[0]);
			if (keep_out != 1 || temp != node)
				close(temp->fd[1]);
		}
		temp = temp->next;
	}
}

int	handle_command_not_found(t_constructor *node, char *path)
{
	if (!path)
	{
		printf("Command not found\n");
		node->shell->last_exit = 127;
		return (1);
	}
	return (0);
}

int	handle_fork_error(t_constructor *node, char *path)
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

void	execute_in_child(t_constructor *node, char *path)
{
	apply_all_redirections(node);
	execve(path, node->executable, node->shell->env);
	perror("Error al ejecutar el comando");
	free(path);
	exit(1);
}

void	setup_first_command_pipes(t_constructor *node)
{
	dup2(node->fd[1], STDOUT_FILENO);
	close_all_pipes_except(node, 0, 0);
}

void	setup_middle_command_pipes(t_constructor *node)
{
	dup2(node->prev->fd[0], STDIN_FILENO);
	dup2(node->fd[1], STDOUT_FILENO);
	close_all_pipes_except(node, 0, 0);
}

void	setup_last_command_pipes(t_constructor *node)
{
	dup2(node->prev->fd[0], STDIN_FILENO);
	close_all_pipes_except(node, 0, 0);
}

void	execute_command_with_path(t_constructor *node, char *path,
			void (*setup_pipes)(t_constructor *))
{
	node->pid = fork();
	if (handle_fork_error(node, path))
		return;
	if (node->pid == 0)
	{
		if (setup_pipes)
			setup_pipes(node);
		execute_in_child(node, path);
	}
	free(path);
}

void	execute_command(t_constructor *node)
{
	char	*path;

	if (node->type != TOKEN_COMMAND || !node->executable || !node->executable[0])
		return;
	path = acces_path(node);
	if (handle_command_not_found(node, path))
		return;
	execute_command_with_path(node, path, NULL);
}

void	execute_first_command(t_constructor *node)
{
	char	*path;

	if (node->type != TOKEN_COMMAND || !node->executable || !node->executable[0])
		return;

	path = acces_path(node);
	if (handle_command_not_found(node, path))
		return;
	execute_command_with_path(node, path, setup_first_command_pipes);
}

void	execute_middle_command(t_constructor *node)
{
	char	*path;

	if (node->type != TOKEN_COMMAND || !node->executable || !node->executable[0])
		return;

	path = acces_path(node);
	if (handle_command_not_found(node, path))
		return;
	execute_command_with_path(node, path, setup_middle_command_pipes);
}

void	execute_last_command(t_constructor *node)
{
	char	*path;

	if (node->type != TOKEN_COMMAND || !node->executable || !node->executable[0])
		return;

	path = acces_path(node);
	if (handle_command_not_found(node, path))
		return;
	execute_command_with_path(node, path, setup_last_command_pipes);
}

void	token_commands(t_constructor *node)
{
	if (node->type != TOKEN_COMMAND)
		return;

	if (node->pipe_in == 0 && node->pipe_out == 0)
		execute_command(node);
	else if (node->pipe_in == 0 && node->pipe_out == 1)
		execute_first_command(node);
	else if (node->pipe_in == 1 && node->pipe_out == 1)
		execute_middle_command(node);
	else if (node->pipe_in == 1 && node->pipe_out == 0)
		execute_last_command(node);
}
