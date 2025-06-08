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

char	*get_env_value(t_shell *shell, const char *key)
{
	int		i;
	size_t	key_len;

	if (!shell || !shell->env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], key, key_len) == 0 
			&& shell->env[i][key_len] == '=')
			return (shell->env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

void	set_env_value(t_shell *shell, const char *key, const char *value)
{
	int		i;
	size_t	key_len;
	char	*new_entry;
	char	*key_equals;

	if (!shell || !key || !value)
		return ;
	key_len = ft_strlen(key);
	key_equals = ft_strjoin(key, "=");
	if (!key_equals)
		return ;
	new_entry = ft_strjoin(key_equals, value);
	free(key_equals);
	if (!new_entry)
		return ;
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], key, key_len) == 0 && shell->env[i][key_len] == '=')
		{
			free(shell->env[i]);
			shell->env[i] = new_entry;
			return;
		}
		i++;
	}
	char	**new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
	{
		free(new_entry);
		return;
	}
	int	j = 0;
	while (j < i)
	{
		new_env[j] = shell->env[j];
		j++;
	}
	new_env[i] = new_entry;
	new_env[i + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
}

char	*get_current_directory(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	return (cwd);
}

void	refresh_directori(t_shell *shell, const char *new_path, int is_absolute)
{
	char	*resolved_path;
	char	*old_pwd;

	old_pwd = get_env_value(shell, "PWD");
	if (!old_pwd)
		old_pwd = get_current_directory();
	if (is_absolute)
		resolved_path = ft_strdup(new_path);
	else
		resolved_path = get_current_directory();
	if (!resolved_path)
		return ;
	if (old_pwd)
		set_env_value(shell, "OLDPWD", old_pwd);
	set_env_value(shell, "PWD", resolved_path);
	free(resolved_path);
	shell->last_exit = 0;
}
