/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 cd.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/04/03 20:02:57 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/04/03 20:02:57 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

char *get_env_value(t_shell *shell, const char *key)
{
	int i;
	size_t key_len;

	if (!shell || !shell->env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], key, key_len) == 0 && shell->env[i][key_len] == '=')
			return (shell->env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

void cd_home(t_constructor *node)
{
	char *home;

	home = get_env_value(node->shell, "HOME");
	if (!home)
	{
		printf("cd: HOME not set\n");
		node->shell->last_exit = 1;
		return;
	}
	printf("%s\n", home);
	if (chdir(home) == -1)
	{
		perror("cd");
		node->shell->last_exit = 1;
		return;
	}
	refresh_directori(node->shell, home, 1);
}

void cd_oldpwd(t_constructor *node)
{
	char *oldpwd;

	oldpwd = get_env_value(node->shell, "OLDPWD");
	if (!oldpwd)
	{
		printf("cd: OLDPWD not set\n");
		node->shell->last_exit = 1;
		return;
	}
	printf("%s\n", oldpwd);
	if (chdir(oldpwd) == -1)
	{
		printf("cd: %s: No such file or directory\n", oldpwd);
		node->shell->last_exit = 1;
		return;
	}
	refresh_directori(node->shell, oldpwd, 1);
}

void cd_directori(t_constructor *node)
{
	if (chdir(node->executable[1]) == -1)
	{
		printf("cd: %s: No such file or directory\n", node->executable[1]);
		node->shell->last_exit = 1;
		return;
	}
	refresh_directori(node->shell, node->executable[1], 0);
}

void cd(t_constructor *node)
{
	if (node->size_exec == 1)
		cd_home(node);
	else if (node->size_exec == 2 && ft_strncmp(node->executable[1], "-", 1) == 0)
		cd_oldpwd(node);
	else
		cd_directori(node);
}
