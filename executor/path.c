/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 path.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 16:19:43 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 16:19:43 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

int	len_path(char **path)
{
	int	i;

	i = 0;
	while (path[i])
		i++;
	return (i);
}

char	**allocate_path_array(int len)
{
	char	**new_path;

	new_path = malloc((len + 1) * sizeof(char *));
	if (!new_path)
		return (NULL);
	return (new_path);
}

int	copy_path_elements(char **new_path, char **path, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		new_path[i] = ft_strdup(path[i]);
		if (!new_path[i])
		{
			i--;
			while (i >= 0)
				free(new_path[i--]);
			return (0);
		}
		i++;
	}
	new_path[len] = NULL;
	return (1);
}

char	**copy_path(char **path)
{
	char	**new_path;
	int		len;

	if (!path)
		return (NULL);
	len = len_path(path);
	if (len == 0)
		return (NULL);
	new_path = allocate_path_array(len);
	if (!new_path)
		return (NULL);
	if (!copy_path_elements(new_path, path, len))
	{
		free_path_array(new_path);
		return (NULL);
	}
	return (new_path);
}
void path(t_shell *shell)
{
    if (!shell || !shell->env)
    {
        if (shell)
            shell->paths = NULL;
        return;
    }
    
    shell->paths = find_path(shell);
    if (!shell->paths || len_path(shell->paths) == 0)
    {
        free_path_array(shell->paths);
        shell->paths = try_alternative_path(shell->env);
    }
    
    // No necesitas hacer un copy_path y luego liberar el original
    // Ya que el collector se encargará de liberar la memoria
}