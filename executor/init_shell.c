/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:14:31 by acarranz          #+#    #+#             */
/*   Updated: 2025/05/03 18:00:00 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void init_constructor_fields(t_constructor *new)
{
    new->executable = NULL;
    new->size_exec = 0;
    new->input_file = NULL;
    new->output_file = NULL;
    new->pipe_in = -1;
    new->pipe_out = -1;
    new->append = -1;
    new->expand = -1;
    new->builtin = 0;
    new->type = 0;
    new->error = NO_ERROR;
    new->shell = NULL;
    new->next = NULL;
    new->prev = NULL;
}

t_constructor *init_constructor(t_collector **collector)
{
    t_constructor *new;

    new = (t_constructor *)malloc(sizeof(t_constructor));
    if (!new)
        return (NULL);
    if (collector)
        collector_append(collector, new);
    init_constructor_fields(new);
    return (new);
}

static int init_shell_fields(t_shell *shell, t_collector **collector)
{
    shell->last_exit = 0;
    shell->interactive = 1;
    shell->output = NULL;
    shell->node_size = 0;
    shell->collector = *collector;
    
    shell->home = get_home(shell);
    if (shell->home && collector)
        collector_append(collector, shell->home);
    
    shell->pwd = getcwd(NULL, 0);
    if (shell->pwd && collector)
        collector_append(collector, shell->pwd);
    
    shell->oldpwd = ft_strdup(shell->pwd);
    if (shell->oldpwd && collector)
        collector_append(collector, shell->oldpwd);
    
    shell->constructor = init_constructor(collector);
    if (!shell->constructor)
        return (0);
    
    return (1);
}

t_shell *init_shell(t_shell *shell, char **env, t_collector **collector)
{
    if (shell)
        clean_shell(shell);
    
    shell = (t_shell *)malloc(sizeof(t_shell));
    if (!shell)
        return (NULL);
    
    if (collector)
        collector_append(collector, shell);
    
    // Call void functions without checking return values
    copy_env_to_shell(shell, env);
    collector_append(&shell->collector, shell->env);
    env_to_export(shell);
    create_export(shell);
    collector_append(&shell->collector, shell->export);
    path(shell);
    if (!init_shell_fields(shell, collector))
    {
        clean_shell(shell);
        return (NULL);
    }
    return (shell);
}