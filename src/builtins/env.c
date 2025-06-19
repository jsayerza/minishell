/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 env.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:55 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:55 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**copy_env_excluding_var(char **env,
		const char *var_name, int env_len)
{
	char	**new_env;
	int		i;
	int		j;

	new_env = malloc(sizeof(char *) * env_len);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, ft_strlen(var_name)) != 0
			|| env[i][ft_strlen(var_name)] != '=')
		{
			new_env[j] = ft_strdup(env[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

void	remove_env_var(char ***env, const char *var_name)
{
	int		len;
	int		i;
	char	**new_env;

	len = 0;
	while ((*env)[len])
		len++;
	new_env = copy_env_excluding_var(*env, var_name, len);
	if (!new_env)
		return ;
	i = 0;
	while ((*env)[i])
	{
		free((*env)[i]);
		i++;
	}
	free(*env);
	*env = new_env;
}

int	size_exe(t_const *node)
{
	int	i;

	i = 0;
	while (node->executable[i])
		i++;
	return (i);
}

void	env(t_const *node)
{
	int	size;

	size = 0;
	if (!node || !node->executable || !node->shell)
		return ;
	size = size_exe(node);
	if (size != 1)
	{
		ft_putstr_fd("No such file or directory", 2);
		ft_putstr_fd("\n", 2);
		node->shell->last_exit = 127;
		return ;
	}
	if (node->pipe_out)
	{
		redirect_builtin(node, node->shell->env);
		node->shell->last_exit = 0;
	}
	else
		print_builtin(node, node->shell->env);
}
