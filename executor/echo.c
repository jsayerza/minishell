/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 echo.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/04/29 12:00:00 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/04/29 12:00:00 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

#include "../minishell.h"

static int	is_valid_n_flag(const char *arg)
{
    int i;

    if (!arg || arg[0] != '-')
        return (0);
    i = 1;
    while (arg[i])
    {
        if (arg[i] != 'n') // Verifica que todos los caracteres sean 'n'
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
    while (args[i] && is_valid_n_flag(args[i])) // Maneja cualquier argumento válido de tipo -n
    {
        jump = 1;
        i++;
    }
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (!jump)
        printf("\n");
}

void	redirect_echo(t_constructor *node)
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
		execute_echo(node->executable);
		exit(0);
	}
	else
	{
		close(node->fd[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			node->shell->last_exit = WEXITSTATUS(status);
	}
}

void	echo(t_constructor *node)
{
	if (node->pipe_out)
		redirect_echo(node);
	else
		execute_echo(node->executable);
}
