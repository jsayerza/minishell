/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 env_to_export.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:59 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:59 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void	free_exp(char **exp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(exp[i]);
		i++;
	}
	free(exp);
}

char	**allocate_exp(int env_count)
{
	char	**exp;

	exp = malloc(sizeof(char *) * (env_count + 1));
	if (!exp)
		return (NULL);

	return (exp);
}

int	copy_exp_variable(char **exp, char *envv, int index)
{
	exp[index] = malloc(sizeof(char) * (ft_strlen(envv) + 1));
	if (!exp[index])
		return (0);
	if (ft_strlcpy(exp[index], envv, ft_strlen(envv) + 1) != ft_strlen(envv))
	{
		free(exp[index]);
		return (0);
	}
	return (1);
}

void	env_to_export(t_shell *shell)
{
	int		i;
	int		env_count;
	char	**exp;

	env_count = 0;
	while (shell->env && shell->env[env_count])
		env_count++;
	exp = allocate_exp(env_count);
	if (!exp)
		return ;
	i = 0;
	while (i < env_count)
	{
		if (!copy_exp_variable(exp, shell->env[i], i))
		{
			free_exp(exp, i);
			return ;
		}
		i++;
	}
	exp[i] = NULL;
	shell->export = exp;
}
