/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 path_utils00.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 16:19:47 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 16:19:47 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

int	process_segment(char **result, int i, char **temp)
{
	char	*next_colon;
	int		len;

	next_colon = strchr(*temp, ':');
	if (next_colon)
	{
		len = next_colon - *temp;
		result[i] = malloc(len + 1);
		if (!result[i])
			return (0);
		strncpy(result[i], *temp, len);
		result[i][len] = '\0';
		*temp = next_colon + 1;
	}
	else
	{
		result[i] = strdup(*temp);
		if (!result[i])
			return (0);
	}
	return (1);
}

char	**manual_split_path(char *path_str)
{
	int		count;
	int		i;
	char	*temp;
	char	**result;

	i = 0;
	temp = path_str;
	count = count_segments(path_str);
	if (!allocate_result(&result, count))
		return (NULL);
	temp = path_str;
	i = 0;
	while (i < count)
	{
		if (!process_segment(result, i, &temp))
		{
			handle_allocation_error(result, i);
			return (NULL);
		}
		i++;
	}
	result[count] = NULL;
	return (result);
}

void	free_path_array(char **path)
{
	int	i;

	i = 0;
	if (!path)
		return ;
	while (path[i])
		free(path[i++]);
	free(path);
}

char	*get_path_value(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char	**try_alternative_path(char **env)
{
	char	*path_value;

	path_value = get_path_value(env);
	if (path_value)
		return (manual_split_path(path_value));
	return (NULL);
}
