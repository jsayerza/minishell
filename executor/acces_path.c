/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acces_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:13:24 by acarranz          #+#    #+#             */
/*   Updated: 2025/06/10 18:21:33 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*construct_exec(char *path, char *command)
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

static char	*find_path_value(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

static int	check_file_access(char *path)
{
	struct stat	path_stat;
	
	if (access(path, F_OK) != 0)
		return (127);
	if (stat(path, &path_stat) != 0)
		return (127);
	if (S_ISDIR(path_stat.st_mode))
		return (126);
	if (access(path, X_OK) != 0)
		return (126);
	return (0);
}

static char	*search_executable_with_error(char **paths,
			char *command, int *error_code)
{
	char	*exec;
	int		i;
	int		access_result;

	*error_code = 0;
	if (command[0] == '/' || command[0] == '.')
	{
		access_result = check_file_access(command);
		if (access_result == 0)
			return (ft_strdup(command));
		*error_code = access_result;
		return (NULL);
	}
	if (!paths)
	{
		*error_code = 127;
		return (NULL);
	}
	i = 0;
	while (paths[i])
	{
		exec = construct_exec(paths[i], command);
		if (exec)
		{
			access_result = check_file_access(exec);
			if (access_result == 0)
				return (exec);
			if (*error_code == 0)
				*error_code = access_result;
			free(exec);
		}
		i++;
	}
	if (*error_code == 0)
		*error_code = 127;
	return (NULL);
}

void	check_path(t_shell *shell)
{
	char	*path_value;

	path_value = find_path_value(shell->env);
	if (!path_value || ft_strlen(path_value) == 0)
	{
		if (shell->paths)
		{
			free_path_array(shell->paths);
			shell->paths = NULL;
		}
		return ;
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

char	*acces_path_with_error(t_const *node, int *error_code)
{
	check_path(node->shell);
	return (search_executable_with_error(node->shell->paths,
			node->executable[0], error_code));
}

char	*acces_path(t_const *node)
{
	int	error_code;
	
	return (acces_path_with_error(node, &error_code));
}
