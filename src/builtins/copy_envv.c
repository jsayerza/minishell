/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 copy_envv.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:39 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:39 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_envv(char **env, int count)
{
	int	i;

	i = 0;
	while (i < count)
		free(env[i++]);
	free(env);
}

char	**allocate_env(int env_count)
{
	char	**env;

	env = malloc(sizeof(char *) * (env_count + 1));
	if (!env)
		return (NULL);
	return (env);
}

int	copy_env_variable(char **env, char *envv, int index)
{
	env[index] = malloc(sizeof(char) * (ft_strlen(envv) + 1));
	if (!env[index])
		return (0);
	if (ft_strlcpy(env[index], envv, ft_strlen(envv) + 1) != ft_strlen(envv))
	{
		free(env[index]);
		return (0);
	}
	return (1);
}

int	copy_env_variables(char **env, char **envv, int env_count)
{
	int	i;

	i = 0;
	while (i < env_count)
	{
		if (!copy_env_variable(env, envv[i], i))
			return (0);
		i++;
	}
	env[i] = NULL;
	return (1);
}

void	copy_env_to_shell(t_shell *shell, char **envv)
{
	int		env_count;
	char	**env;

	env_count = 0;
	while (envv && envv[env_count])
		env_count++;
	if (env_count == 0)
	{
		ft_putstr_fd("Error: El entorno está vacío.\n", 2);
		return ;
	}
	env = allocate_env(env_count);
	if (!env)
		return ;
	if (!copy_env_variables(env, envv, env_count))
	{
		free_envv(env, env_count);
		return ;
	}
	shell->env = env;
}
