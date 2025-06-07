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

void add_or_update_env_var(char ***env, char *key, char *value)
{
    int i;
    char *temp;
    char *new_var;
    size_t key_len;

    if (!env || !*env || !key || !value)
        return;
    key_len = ft_strlen(key);
    temp = ft_strjoin(key, "=");
    if (!temp)
        return;
    new_var = ft_strjoin(temp, value);
    free(temp);
    if (!new_var)
        return;
    i = 0;
    while ((*env)[i])
    {
        if (ft_strncmp((*env)[i], key, key_len) == 0 &&
            (*env)[i][key_len] == '=')
        {
            free((*env)[i]);
            (*env)[i] = new_var;
            return;
        }
        i++;
    }
    char **new_env = malloc(sizeof(char *) * (i + 2));
    if (!new_env)
    {
        free(new_var);
        return;
    }
    i = 0;
    while ((*env)[i])
    {
        new_env[i] = (*env)[i];
        i++;
    }
    new_env[i] = new_var;
    new_env[i + 1] = NULL;
    free(*env);
    *env = new_env;
}

void	check_heredoc(t_constructor *node)
{
	t_constructor	*current;
	int				pipefd[2];
	int				i;
	ssize_t			bytes_written;

	if (!node->heredoc)
		return;

	// Configurar señales para heredoc (debe poder ser interrumpido)
	signal(SIGINT, SIG_DFL);  // Permitir interrupción con Ctrl+C

	i = 0;
	current = node;
	while (current->heredoc[i])
		i++;
	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe creation failed");
		exit(1);
	}
	bytes_written = write(pipefd[1], current->heredoc[i - 1],
		strlen(current->heredoc[i - 1]));
	if (bytes_written == -1)
	{
		perror("minishell: write to pipe failed");
		close(pipefd[0]);
		close(pipefd[1]);
		exit(1);
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 heredoc");
		close(pipefd[0]);
		exit(1);
	}
	close(pipefd[0]);
}

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

void check_path(t_shell *shell)
{
	char *path_value;
	int i;

	path_value = NULL;
	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], "PATH=", 5) == 0)
		{
			path_value = shell->env[i] + 5;
			break;
		}
		i++;
	}
	if (!path_value || ft_strlen(path_value) == 0)
	{
		if (shell->paths)
		{
			free_path_array(shell->paths);
			shell->paths = NULL;
		}
		return;
	}
	if (shell->paths)
		free_path_array(shell->paths);
	shell->paths = ft_split(path_value, ':');
	if (!shell->paths)
	{
		perror("minishell: error actualizando PATH");
		exit(1);
	}
}

char *acces_path(t_constructor *node) {
    char *exec;
    int i;

    if (node->executable[0][0] == '/' || node->executable[0][0] == '.') {
        if (access(node->executable[0], X_OK) == 0)
            return (ft_strdup(node->executable[0])); // Retorna la ruta tal cual
        return (NULL); // No tiene permisos o no existe
    }
    check_path(node->shell);
    i = 0;
    while (node->shell->paths && node->shell->paths[i]) {
        exec = construct_exec(node->shell->paths[i], node->executable[0]);
        if (exec && access(exec, X_OK) == 0)
            return (exec);
        free(exec);
        i++;
    }
    return (NULL);
}

void close_all_pipes_except(t_constructor *node, int keep_in, int keep_out)
{
	t_constructor *temp;

	temp = node->shell->constructor;
	while (temp && temp->prev)
		temp = temp->prev;
	while (temp)
	{
		if (temp->pipe_out == 1)
		{
			if (!(keep_in && temp->next == node))
				close(temp->fd[0]);
			if (!(keep_out && temp == node))
				close(temp->fd[1]);
		}
		temp = temp->next;
	}
}

int	handle_command_not_found(t_constructor *node, char *path)
{
	if (!path)
	{
		ft_putstr_fd("Command not found\n", 2);
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

void execute_in_child(t_constructor *node, char *path)
{
	setup_child_signals();
    apply_all_redirections(node);
    if (node->redirect_in_type == 6)
        check_heredoc(node);

    // Actualizar la variable `_` en shell->env
    add_or_update_env_var(&(node->shell->env), "_", path);

    // Ejecutar el comando
    execve(path, node->executable, node->shell->env);
    perror("Error al ejecutar el comando");
    // NO liberar path aquí - ya se liberó en el padre
    exit(1);
}

void setup_first_command_pipes(t_constructor *node)
{
    if (node->pipe_out == 1)
        dup2(node->fd[1], STDOUT_FILENO);
    close_all_pipes_except(node, 0, 0); // Cerrar todos los pipes
}

void setup_middle_command_pipes(t_constructor *node)
{
    if (node->prev && node->prev->pipe_out == 1)
        dup2(node->prev->fd[0], STDIN_FILENO);
    if (node->pipe_out == 1)
        dup2(node->fd[1], STDOUT_FILENO);
    close_all_pipes_except(node, 0, 0);
}

void setup_last_command_pipes(t_constructor *node)
{
    if (node->prev && node->prev->pipe_out == 1)
        dup2(node->prev->fd[0], STDIN_FILENO);
    close_all_pipes_except(node, 0, 0); // Cerrar todos los pipes
}

void execute_command_with_path(t_constructor *node, char *path,
                             void (*setup_pipes)(t_constructor *))
{
    node->pid = fork();
    if (handle_fork_error(node, path))
        return;

    if (node->pid == 0) { // Proceso hijo
        // Configurar señales ANTES de hacer cualquier otra cosa
        setup_child_signals();

        if (setup_pipes)
            setup_pipes(node);
        execute_in_child(node, path);
    }
    // En el proceso padre liberamos path después del fork
    free(path);
}


void wait_for_child_processes(t_constructor *node)
{
    int status;
    t_constructor *current = node;

    // Ir al primer nodo
    while (current && current->prev)
        current = current->prev;

    while (current)
    {
        if (current->type == TOKEN_COMMAND && current->pid > 0)
        {
            waitpid(current->pid, &status, 0);
            if (WIFEXITED(status))
            {
                int exit_code = WEXITSTATUS(status);
                current->shell->last_exit = exit_code;
            }
            else if (WIFSIGNALED(status))
            {
                int sig = WTERMSIG(status);

                if (sig == SIGINT)  // Señal 2
                {
                    current->shell->last_exit = 130;
                }
                else if (sig == SIGQUIT)  // Señal 3
                {
                    current->shell->last_exit = 131;
                    if (WCOREDUMP(status))
                        ft_putstr_fd("Quit (core dumped)\n", 2);
                    else
                        ft_putstr_fd("Quit\n", 2);
                }
                else
                {
                    current->shell->last_exit = 128 + sig;
                }
            }
            else
            {
                ft_putstr_fd("DEBUG: Estado de terminación desconocido\n", 2); // DEBUG
            }

        }
        current = current->next;
    }
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
