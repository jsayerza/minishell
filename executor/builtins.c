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

void file_out_builtin(t_constructor *node, char *buitlin)
{
	int fd;
	int size;
	int i;
	int original_stdout;

	i = 0;
	size = 0;
	while (node->redirect_out[size])
		size++;
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdout < 0)
	{
		perror("Error duplicando stdout");
		return;
	}
	while (node->redirect_out[i])
	{
		fd = open(node->redirect_out[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("Error open");
			close(original_stdout);
			return;
		}
		if (i == size - 1)
		{
			if (dup2(fd, STDOUT_FILENO) < 0)
			{
				perror("Error dup2");
				close(fd);
				close(original_stdout);
				return;
			}
		}
		close(fd);
		i++;
	}
	if (ft_strcmp("env", buitlin) == 0)
		env(node);
	if (ft_strcmp("export", buitlin) == 0)
		export(node);
	if (ft_strcmp("echo", buitlin) == 0)
		echo(node);
	if (ft_strcmp("pwd", buitlin) == 0)
		pwd(node);
	if (dup2(original_stdout, STDOUT_FILENO) < 0)
	{
		perror("Error restaurando stdout");
	}
	close(original_stdout);
}

void file_append_builtin(t_constructor *node, char *buitlin)
{
	int fd;
	int size;
	int i;
	int original_stdout;

	i = 0;
	size = 0;
	while (node->redirect_append[size])
		size++;
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdout < 0)
	{
		perror("Error duplicando stdout");
		return;
	}
	while (node->redirect_append[i])
	{
		fd = open(node->redirect_append[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("Error open");
			close(original_stdout);
			return;
		}
		if (i == size - 1)
		{
			if (dup2(fd, STDOUT_FILENO) < 0)
			{
				perror("Error dup2");
				close(fd);
				close(original_stdout);
				return;
			}
		}
		close(fd);
		i++;
	}
	if (ft_strcmp("env", buitlin) == 0)
		env(node);
	if (ft_strcmp("export", buitlin) == 0)
		export(node);
	if (ft_strcmp("echo", buitlin) == 0)
		echo(node);
	if (ft_strcmp("pwd", buitlin) == 0)
		pwd(node);
	if (dup2(original_stdout, STDOUT_FILENO) < 0)
	{
		perror("Error restaurando stdout");
	}
	close(original_stdout);
}

void	token_builtins(t_constructor *node)
{
	if (node->redirect_out)
	{
		file_out_builtin(node, node->executable[0]);
		return ;
	}
	if (node->redirect_append)
	{
		file_append_builtin(node, node->executable[0]);
		return ;
	}
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
	if (node->builtin == BUILTIN_EXIT)
		process_exit(node);
}
