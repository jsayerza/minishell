/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:14:31 by acarranz          #+#    #+#             */
/*   Updated: 2025/05/29 20:35:11 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_constructor_fields(t_constructor *new)
{
	if (!new)
		return ;
	new->executable = NULL;
	new->size_exec = 0;
	new->redirect_in = NULL;
	new->redirect_out = NULL;
	new->redirect_append = NULL;
	new->heredoc = NULL;
	new->redirect_in_type = 0;
	new->redirect_out_type = 0;
	new->pipe_in = -1;
	new->pipe_out = -1;
	new->builtin = 0;
	new->type = 0;
	new->error = NO_ERROR;
	new->shell = NULL;
	new->next = NULL;
	new->prev = NULL;
}

t_constructor	*init_constructor(t_collector **collector)
{
	t_constructor	*new;

	new = (t_constructor *)malloc(sizeof(t_constructor));
	if (!new)
		return (NULL);
	init_constructor_fields(new);
	if (collector)
		collector_append(collector, new);
	return (new);
}

static int	init_shell_fields(t_shell *shell, t_collector **collector)
{
	if (!shell)
		return (0);
	shell->last_exit = 0;
	shell->interactive = 1;
	shell->output = NULL;
	shell->node_size = 0;
	shell->collector = collector ? *collector : NULL;
	shell->home = get_home(shell);
	shell->pwd = getcwd(NULL, 0);
	shell->oldpwd = shell->pwd ? ft_strdup(shell->pwd) : NULL;
	if (collector)
	{
		if (shell->home)
			collector_append(collector, shell->home);
		if (shell->pwd)
			collector_append(collector, shell->pwd);
		if (shell->oldpwd)
			collector_append(collector, shell->oldpwd);
	}
	shell->constructor = init_constructor(collector);
	if (!shell->constructor)
		return (0);
	return (1);
}

t_shell	*init_shell(t_shell *shell, char **env, t_collector **collector)
{
	if (shell)
		clean_shell(shell);
	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	ft_memset(shell, 0, sizeof(t_shell));
	if (collector)
	{
		collector_append(collector, shell);
		shell->collector = *collector;
	}
	copy_env_to_shell(shell, env);
	env_to_export(shell);
	create_export(shell);
	if (shell->export && collector)
		collector_append(collector, shell->export);
	path(shell);
	if (!init_shell_fields(shell, collector))
	{
		clean_shell(shell);
		return (NULL);
	}
	return (shell);
}
