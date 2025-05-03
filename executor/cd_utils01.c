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

void	copy_pwd_env(t_shell *shell, char *new_pwd, char *new_oldpwd)
{
	int		i;

	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], "PWD=", 4) == 0)
		{
			free(shell->env[i]);
			shell->env[i] = new_pwd;
		}
		else if (ft_strncmp(shell->env[i], "OLDPWD=", 7) == 0)
		{
			free(shell->env[i]);
			shell->env[i] = new_oldpwd;
		}
		i++;
	}
}

void	update_environment(t_shell *shell)
{
	char	*new_pwd;
	char	*new_oldpwd;

	new_pwd = ft_strjoin("PWD=", shell->pwd);
	new_oldpwd = ft_strjoin("OLDPWD=", shell->oldpwd);
	if (!new_pwd || !new_oldpwd)
	{
		free(new_pwd);
		free(new_oldpwd);
		return ;
	}
	copy_pwd_env(shell, new_pwd, new_oldpwd);
}

void	append_pwd_up(t_shell *shell)
{
	int		len;
	char	*new_pwd;

	len = ft_strlen(shell->pwd);
	while (len > 0 && shell->pwd[len - 1] != '/')
		len--;
	if (len > 1)
		len--;
	new_pwd = malloc((len + 1) * sizeof(char));
	if (!new_pwd)
		return ;
	ft_strlcpy(new_pwd, shell->pwd, len + 1);
	free(shell->oldpwd);
	shell->oldpwd = shell->pwd;
	shell->pwd = new_pwd;
	printf("pwd->%s\n", shell->pwd);
	printf("oldpwd->%s\n", shell->oldpwd);
	update_environment(shell);
}

void	append_pwd(t_shell *shell, char *new_dir)
{
	char	*new_pwd;
	char	*slash;

	if (ft_strcmp(new_dir, "..") == 0)
	{
		append_pwd_up(shell);
		return ;
	}
	slash = ft_strjoin(shell->pwd, "/");
	if (!slash)
		return ;
	new_pwd = ft_strjoin(slash, new_dir);
	if (!new_pwd)
		return ;
	free(shell->oldpwd);
	shell->oldpwd = shell->pwd;
	shell->pwd = new_pwd;
	collector_append(&shell->collector, new_pwd);
	collector_append(&shell->collector, slash);
	printf("pwd->%s\n", shell->pwd);
	printf("oldpwd->%s\n", shell->oldpwd);
	update_environment(shell);
}

void	refresh_directori(t_shell *shell, char *pwd, int type)
{
	char	*new_oldpwd;
	int		i;

	new_oldpwd = malloc((ft_strlen(shell->pwd) + 1) * sizeof(char));
	if (!new_oldpwd)
		return ;
	collector_append(&shell->collector, new_oldpwd);
	if (!type)
		append_pwd(shell, pwd);
	else
	{
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
		update_environment(shell);
		printf("pwd->%s\n", shell->pwd);
		printf("oldpwd->%s\n", shell->oldpwd);
	}
}
