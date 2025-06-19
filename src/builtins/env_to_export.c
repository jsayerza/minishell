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

#include "../../includes/minishell.h"

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

static int	count_valid_env_vars(char **env)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (env && env[i])
	{
		if (!(env[i][0] == '_' && env[i][1] == '='))
			count++;
		i++;
	}
	return (count);
}

void	env_to_export(t_shell *shell)
{
	int		i;
	int		j;
	char	**exp;

	exp = allocate_exp(count_valid_env_vars(shell->env));
	if (!exp)
		return ;
	i = 0;
	j = 0;
	while (shell->env && shell->env[i])
	{
		if (shell->env[i][0] == '_' && shell->env[i][1] == '=')
		{
			i++;
			continue ;
		}
		if (!copy_exp_variable(exp, shell->env[i], j++))
		{
			free_exp(exp, j - 1);
			return ;
		}
		i++;
	}
	exp[j] = NULL;
	shell->export = exp;
}
