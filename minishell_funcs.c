/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 13:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/06/07 08:02:34 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_shlvl(t_shell *shell, int increment)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_shlvl;

	if (!shell)
		return ;
	shlvl_str = get_env_value(shell, "SHLVL");
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str);
	else
		shlvl = 0;
	shlvl += increment;
	if (shlvl < 0)
		shlvl = 0;
	new_shlvl = ft_itoa(shlvl);
	if (!new_shlvl)
		return ;
	add_or_update_env_var(&shell->env, "SHLVL", new_shlvl);
	add_or_update_env_var(&shell->export, "declare -x SHLVL", new_shlvl);
	free(new_shlvl);
}

void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->paths)
	{
		free_path_array(shell->paths);
		shell->paths = NULL;
	}
	if (shell->env)
	{
		free_path_array(shell->env);
		shell->env = NULL;
	}
	if (shell->export)
	{
		free_path_array(shell->export);
		shell->export = NULL;
	}
	if (shell->path)
	{
		free(shell->path);
		shell->path = NULL;
	}
}
