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
	close(node->fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		node->shell->last_exit = WEXITSTATUS(status);
}

static void	process_output_file(t_constructor *node, int i, int size, int *orig)
{
	int	fd;

	fd = open(node->redirect_out[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Error open");
		close(*orig);
		return ;
	}
	if (i == size - 1)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("Error dup2");
			close(fd);
			close(*orig);
			return ;
		}
	}
	close(fd);
}

static void	open_output_files(t_constructor *node, int *original_stdout)
{
	int	size;
	int	i;

	i = 0;
	size = 0;
	while (node->redirect_out[size])
		size++;
	while (node->redirect_out[i])
	{
		process_output_file(node, i, size, original_stdout);
		i++;
	}
}

void	file_out_builtin(t_constructor *node, char *builtin)
{
	int	original_stdout;

	original_stdout = dup(STDOUT_FILENO);
	if (original_stdout < 0)
	{
		perror("Error duplicando stdout");
		return ;
	}
	open_output_files(node, &original_stdout);
	if (ft_strcmp("env", builtin) == 0)
		env(node);
	if (ft_strcmp("export", builtin) == 0)
		export(node);
	if (ft_strcmp("echo", builtin) == 0)
		echo(node);
	if (ft_strcmp("pwd", builtin) == 0)
		pwd(node);
	if (dup2(original_stdout, STDOUT_FILENO) < 0)
		perror("Error restaurando stdout");
	close(original_stdout);
}

void	token_builtins(t_constructor *node)
{
	if (node->redirect_out)
	{
		file_out_builtin(node, node->executable[0]);
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
