/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:14:31 by acarranz          #+#    #+#             */
/*   Updated: 2025/06/07 08:13:05 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	constructor_fields_init(t_const *new)
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
	new->shell = NULL;
	new->next = NULL;
	new->prev = NULL;
}

t_const	*init_const(t_collector **collector)
{
	t_const	*new;

	new = (t_const *)malloc(sizeof(t_const));
	if (!new)
		return (NULL);
	constructor_fields_init(new);
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
	if (collector)
		shell->collector = *collector;
	else
		shell->collector = NULL;
	shell->constructor = init_const(collector);
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
	path(shell);
	if (!init_shell_fields(shell, collector))
	{
		clean_shell(shell);
		return (NULL);
	}
	return (shell);
}
