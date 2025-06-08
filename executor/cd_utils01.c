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

static char	*create_env_entry(const char *key, const char *value)
{
	char	*key_equals;
	char	*new_entry;

	key_equals = ft_strjoin(key, "=");
	if (!key_equals)
		return (NULL);
	new_entry = ft_strjoin(key_equals, value);
	free(key_equals);
	return (new_entry);
}

static int	find_env_index(t_shell *shell, const char *key)
{
	int		i;
	size_t	key_len;

	key_len = ft_strlen(key);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], key, key_len) == 0
			&& shell->env[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static void	update_existing_env(t_shell *shell, int index, char *new_entry)
{
	free(shell->env[index]);
	shell->env[index] = new_entry;
}

static void	add_new_env_entry(t_shell *shell, char *new_entry, int count)
{
	char	**new_env;
	int		j;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_entry);
		return ;
	}
	j = 0;
	while (j < count)
	{
		new_env[j] = shell->env[j];
		j++;
	}
	new_env[count] = new_entry;
	new_env[count + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
}

void	set_env_value(t_shell *shell, const char *key, const char *value)
{
	char	*new_entry;
	int		index;
	int		count;

	if (!shell || !key || !value)
		return ;
	new_entry = create_env_entry(key, value);
	if (!new_entry)
		return ;
	index = find_env_index(shell, key);
	if (index != -1)
	{
		update_existing_env(shell, index, new_entry);
		return ;
	}
	count = 0;
	while (shell->env[count])
		count++;
	add_new_env_entry(shell, new_entry, count);
}

char	*get_current_directory(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	return (cwd);
}

static char	*get_old_pwd(t_shell *shell)
{
	char	*old_pwd;

	old_pwd = get_env_value(shell, "PWD");
	if (!old_pwd)
		old_pwd = get_current_directory();
	return (old_pwd);
}

static char	*resolve_path(const char *new_path, int is_absolute)
{
	char	*resolved_path;

	if (is_absolute)
		resolved_path = ft_strdup(new_path);
	else
		resolved_path = get_current_directory();
	return (resolved_path);
}

void	refresh_directori(t_shell *shell, const char *new_path, int is_absolute)
{
	char	*resolved_path;
	char	*old_pwd;

	old_pwd = get_old_pwd(shell);
	resolved_path = resolve_path(new_path, is_absolute);
	if (!resolved_path)
		return ;
	if (old_pwd)
		set_env_value(shell, "OLDPWD", old_pwd);
	set_env_value(shell, "PWD", resolved_path);
	free(resolved_path);
	shell->last_exit = 0;
}
