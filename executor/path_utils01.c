/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 path_utils01.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 16:19:52 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 16:19:52 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_value(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char **find_path(t_shell *shell)
{
    char **result;
    char *path_value;
    int i;

    if (!shell->env)
        return (NULL);
    path_value = get_value(shell->env);
    if (!path_value || !*path_value)
        return (NULL);
    shell->path = ft_strdup(path_value);
    collector_append(&shell->collector, shell->path);
    result = ft_split(path_value, ':');
    if (!result || !result[0])
    {
        free_path_array(result);
        return (NULL);
    }
    
    collector_append(&shell->collector, result);
    i = 0;
    while (result[i])
    {
        collector_append(&shell->collector, result[i]);
        i++;
    }
    
    return (result);
}

int	count_segments(char *path_str)
{
	int		count;
	char	*temp;

	count = 1;
	temp = path_str;
	while (*temp)
	{
		if (*temp == ':')
			count++;
		temp++;
	}
	return (count);
}

int	allocate_result(char ***result, int count)
{
	*result = malloc((count + 1) * sizeof(char *));
	if (!*result)
		return (0);
	return (1);
}

int	handle_allocation_error(char **result, int i)
{
	while (--i >= 0)
		free(result[i]);
	free(result);
	return (0);
}
