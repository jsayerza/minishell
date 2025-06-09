/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 env_utils00.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:24 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:24 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

static char	*create_env_string(char *key, char *value)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, value);
	free(temp);
	return (result);
}

static int	find_env_var(char **env, char *key, size_t key_len)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static void	expand_env_array(char ***env, char *new_var)
{
	char	**new_env;
	int		i;
	int		count;

	count = 0;
	while ((*env)[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_var);
		return ;
	}
	i = 0;
	while (i < count)
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(*env);
	*env = new_env;
}

void	add_or_update_env_var(char ***env, char *key, char *value)
{
	int		i;
	char	*new_var;
	size_t	key_len;

	if (!env || !*env || !key || !value)
		return ;
	key_len = ft_strlen(key);
	new_var = create_env_string(key, value);
	if (!new_var)
		return ;
	i = find_env_var(*env, key, key_len);
	if (i >= 0)
	{
		free((*env)[i]);
		(*env)[i] = new_var;
		return ;
	}
	expand_env_array(env, new_var);
}
