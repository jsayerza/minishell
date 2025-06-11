/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 commands_utils00.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:24 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/06/11 XX:XX:XX by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

int has_output_redirection(t_const *node)
{
    if (!node)
        return (0);
    
    // Verificar si hay redirección de salida (>) o append (>>)
    if ((node->redirect_out && node->redirect_out[0]) || 
        (node->redirect_append && node->redirect_append[0]))
        return (1);
    
    return (0);
}

void setup_first_command_pipes(t_const *node)
{
    printf("DEBUG: setup_first_command_pipes - pipe_out: %d\n", node->pipe_out);
    printf("DEBUG: has_output_redirection: %d\n", has_output_redirection(node));
    
    // SIEMPRE configurar el pipe si pipe_out == 1
    // Las redirecciones se aplicarán después y pueden sobrescribir
    if (node->pipe_out == 1)
    {
        printf("DEBUG: Configurando pipe de salida fd[1]: %d\n", node->fd[1]);
        dup2(node->fd[1], STDOUT_FILENO);
        printf("DEBUG: Pipe configurado\n");
    }
    
    close_all_pipes_except(node, 0, 0);
    printf("DEBUG: Pipes cerrados\n");
}

void setup_middle_command_pipes(t_const *node)
{
    // Configurar entrada del pipe anterior
    if (node->prev && node->prev->pipe_out == 1)
        dup2(node->prev->fd[0], STDIN_FILENO);
    
    // SIEMPRE configurar salida del pipe si pipe_out == 1
    if (node->pipe_out == 1)
        dup2(node->fd[1], STDOUT_FILENO);
    
    close_all_pipes_except(node, 0, 0);
}

void setup_last_command_pipes(t_const *node)
{
    if (node->prev && node->prev->pipe_out == 1)
        dup2(node->prev->fd[0], STDIN_FILENO);
    close_all_pipes_except(node, 0, 0);
}

void	handle_child_command_error(t_const *node, int error_code)
{
	struct stat	path_stat;
	
	if (error_code == 127)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(node->executable[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else if (error_code == 126)
	{
		if (stat(node->executable[0], &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(node->executable[0], STDERR_FILENO);
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		}
		else
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(node->executable[0], STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		}
	}
	else if (error_code == 2)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(node->executable[0], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	}
}

int validate_and_create_output_files(t_const *node)
{
    int fd;
    int i;
    
    // Crear archivos de redirección normal (>)
    if (node->redirect_out)
    {
        i = 0;
        while (node->redirect_out[i])
        {
            fd = open(node->redirect_out[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                ft_putstr_fd("minishell: ", STDERR_FILENO);
                ft_putstr_fd(node->redirect_out[i], STDERR_FILENO);
                ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
                return (0);
            }
            close(fd);
            i++;
        }
    }
    
    // Crear archivos de append (>>)  
    if (node->redirect_append)
    {
        i = 0;
        while (node->redirect_append[i])
        {
            fd = open(node->redirect_append[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                ft_putstr_fd("minishell: ", STDERR_FILENO);
                ft_putstr_fd(node->redirect_append[i], STDERR_FILENO);
                ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
                return (0);
            }
            close(fd);
            i++;
        }
    }
    
    return (1);
}

int validate_and_apply_redirections(t_const *node)
{
    // Validar redirecciones de entrada primero
    if (!validate_all_input_redirections(node))
    {
        node->shell->last_exit = 1;
        return (0);
    }
    
    // Validar redirecciones de salida SIN eliminar archivos
    if (!validate_all_output_redirections_no_delete(node))
    {
        node->shell->last_exit = 1;
        return (0);
    }
    
    apply_all_redirections(node);
    return (1);
}

int	execute_command_with_path(t_const *node, char *path, int error_code,
	void (*setup_pipes)(t_const *))
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("Error al crear el proceso hijo");
		free(path);
		node->shell->last_exit = 1;
		return (0);
	}
	if (node->pid == 0)
	{
		setup_child_signals();
		
		// ORDEN CRÍTICO: 
		// 1. Configurar pipes PRIMERO
		if (setup_pipes)
			setup_pipes(node);
		
		// 2. Aplicar redirecciones DESPUÉS (pueden sobrescribir pipes)
		if (!validate_and_apply_redirections(node))
			exit(1);
		
		if (!path)
		{
			handle_child_command_error(node, error_code);
			exit(error_code == 127 ? 127 : 126);
		}
		
		execute_in_child(node, path);
	}
	free(path);
	return (1);
}