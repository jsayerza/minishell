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

int	find_env_index(t_shell *shell, const char *key)
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

char	*extract_home_path(t_shell *shell, const char *env_entry)
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
	collector_append(&shell->collector, home);
	return (home);
}

char	*get_home(t_shell *shell)
{
	int		i;

	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp("HOME=", shell->env[i], 5) == 0)
			return (extract_home_path(shell, shell->env[i]));
		i++;
	}
	return (NULL);
}
