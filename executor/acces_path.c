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

char	*find_path_value(char **env)
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
