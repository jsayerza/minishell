/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 acces_path_utils00.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:14:40 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:14:40 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static int	check_file_access(char *path)
{
	struct stat	path_stat;
	int			fd;

	if (stat(path, &path_stat) != 0)
		return (127);
	if (S_ISDIR(path_stat.st_mode))
		return (126);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (126);
	close(fd);
	return (0);
}

static char	*try_single_path(char *path, char *command, int *error_code)
{
	char	*exec;
	int		access_result;

	exec = construct_exec(path, command);
	if (exec)
	{
		access_result = check_file_access(exec);
		if (access_result == 0)
			return (exec);
		if (*error_code == 0)
			*error_code = access_result;
		free(exec);
	}
	return (NULL);
}

static char	*search_in_paths(char **paths, char *command, int *error_code)
{
	char	*result;
	int		i;

	if (!paths)
	{
		*error_code = 127;
		return (NULL);
	}
	i = 0;
	while (paths[i])
	{
		result = try_single_path(paths[i], command, error_code);
		if (result)
			return (result);
		i++;
	}
	if (*error_code == 0)
		*error_code = 127;
	return (NULL);
}

char	*search_executable_with_error(char **paths,
			char *command, int *error_code)
{
	int	access_result;

	*error_code = 0;
	if (command[0] == '/' || command[0] == '.')
	{
		access_result = check_file_access(command);
		if (access_result == 0)
			return (ft_strdup(command));
		*error_code = access_result;
		return (NULL);
	}
	return (search_in_paths(paths, command, error_code));
}
