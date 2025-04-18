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

char	**find_path(t_shell *shell)
{
	int		i;
	char	**result;
	char	*path_value;

	i = 0;
	if (!shell->env)
		return (NULL);
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	if (!shell->env[i])
		return (NULL);
	path_value = shell->env[i] + 5;
	if (!*path_value)
		return (NULL);
	shell->path = malloc((ft_strlen(path_value) + 1) * sizeof(char));
	shell->path = ft_strdup(path_value);
	printf("%s\n",shell->path);
	result = ft_split(path_value, ':');
	if (!result || !result[0])
	{
		free_path_array(result);
		return (NULL);
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
