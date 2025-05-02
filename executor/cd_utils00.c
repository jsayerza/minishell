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

char	*extract_home_path(t_shell *shell, const char *env_entry)
{
	char	*home;
	int		len;
	int		i;

	len = ft_strlen(env_entry) - 5;
	home = malloc((len + 1) * sizeof(char));
	if (!home)
		return (NULL);
	collector_append(&shell->collector, home);
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
			return (extract_home_path(shell, shell->env[i]));
		i++;
	}
	return (NULL);
}
