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

#include "../../includes/minishell.h"

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

static char	**expand_env_array(t_shell *shell, char *new_entry, int count)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (NULL);
	i = -1;
	while (++i < count)
		new_env[i] = shell->env[i];
	new_env[count] = new_entry;
	new_env[count + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
	return (new_env);
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
		free(shell->env[index]);
		shell->env[index] = new_entry;
		return ;
	}
	count = 0;
	while (shell->env[count])
		count++;
	if (!expand_env_array(shell, new_entry, count))
		free(new_entry);
}

void	refresh_directori(t_shell *shell, const char *new_path, int is_absolute)
{
	char	*resolved_path;
	char	*old_pwd;

	if (!shell)
		return ;
	old_pwd = get_env_value(shell, "PWD");
	if (!old_pwd)
		old_pwd = getcwd(NULL, 0);
	if (is_absolute)
		resolved_path = ft_strdup(new_path);
	else
		resolved_path = getcwd(NULL, 0);
	if (!resolved_path)
		return ;
	if (old_pwd)
		set_env_value(shell, "OLDPWD", old_pwd);
	set_env_value(shell, "PWD", resolved_path);
	free(resolved_path);
	shell->last_exit = 0;
}
