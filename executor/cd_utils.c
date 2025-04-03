/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 cd_utils.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/04/03 20:06:41 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/04/03 20:06:41 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

char	*extract_home_path(const char *env_entry)
{
	char	*home;
	int		len;
	int		i;

	len = ft_strlen(env_entry) - 5;
	home = malloc((len + 1) * sizeof(char));
	if (!home)
		return (NULL);
	i = 0;
	while (env_entry[i + 5])
	{
		home[i] = env_entry[i + 5];
		i++;
	}
	home[i] = '\0';
	return (home);
}

char	*get_home(t_shell *shell)
{
	int		i;

	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp("HOME=", shell->env[i], 5) == 0)
			return (extract_home_path(shell->env[i]));
		i++;
	}
	return (NULL);
}

void	refresh_directori(t_shell *shell, char *pwd)
{
	char	*new_oldpwd;
	int		i;

	new_oldpwd = malloc((ft_strlen(shell->pwd) + 1) * sizeof(char));
	if (!new_oldpwd)
		return ;
	i = 0;
	while (shell->pwd[i])
	{
		new_oldpwd[i] = shell->pwd[i];
		i++;
	}
	new_oldpwd[i] = '\0';
	free(shell->oldpwd);
	shell->oldpwd = new_oldpwd;
	shell->pwd = pwd;
	printf("pwd->%s\n", shell->pwd);
	printf("oldpwd->%s\n", shell->oldpwd);
}
