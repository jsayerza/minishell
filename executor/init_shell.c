/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 init_shell.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:14:31 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:14:31 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

t_constructor	*init_constructor(void)
{
	t_constructor	*new;

	new = (t_constructor *)malloc(sizeof(t_constructor));
	if (!new)
		return (NULL);
	new->executable = NULL;
	new->size_exec = 0;
	new->fd = 0;
	new->builtin = 0;
	new->type = 0;
	new->error = NO_ERROR;
	new->shell = NULL;
	new->next = NULL;
	return (new);
}

t_shell	*init_shell(t_shell *shell, char **env)
{
	if (shell != NULL)
		clean_shell(shell);
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	copy_env_to_shell(shell, env);
	env_to_export(shell);
	create_export(shell);
	path(shell);
	shell->last_exit = 0;
	shell->interactive = 1;
	shell->home = get_home(shell);
	shell->pwd = getcwd(NULL, 0);
	shell->oldpwd = shell->pwd;
	shell->output = NULL;
	shell->node_size = 0;
	shell->constructor = init_constructor();
	printf("%s\n", shell->home);
	printf("%s\n", shell->pwd);
	printf("%s\n", shell->oldpwd);
	return (shell);
}
