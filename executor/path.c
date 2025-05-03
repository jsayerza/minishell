/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:19:43 by acarranz          #+#    #+#             */
/*   Updated: 2025/03/23 16:19:43 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	clean_existing_paths(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->path)
	{
		freer(shell->path);
		shell->path = NULL;
	}
	if (shell->paths)
	{
		free_path_array(shell->paths);
		shell->paths = NULL;
	}
}

static char	**create_paths_array(const char *path_value)
{
	char	**paths;

	paths = ft_split(path_value, ':');
	if (!paths || !paths[0])
	{
		free_path_array(paths);
		return (NULL);
	}
	return (paths);
}

void	path(t_shell *shell)
{
	char	*path_value;

	if (!shell || !shell->env)
		return ;
	clean_existing_paths(shell);
	path_value = get_path_value(shell->env);
	if (!path_value || !*path_value)
	{
		shell->paths = try_alternative_path(shell->env);
		if (shell->paths && shell->collector)
			collector_append(&shell->collector, shell->paths);
		return ;
	}
	shell->path = ft_strdup(path_value);
	if (!shell->path)
		return ;
	shell->paths = create_paths_array(path_value);
}