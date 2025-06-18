/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 export_utils02.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:14:27 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:14:27 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**swap_str(char **export, int i)
{
	char	*temp;

	temp = export[i + 1];
	export[i + 1] = export[i];
	export[i] = temp;
	return (export);
}

void	sort_export(t_shell *shell)
{
	int		i;
	int		swapped;
	char	**export;

	export = shell->export;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (export[i] && export[i + 1])
		{
			if (ft_strcmp(export[i], export[i + 1]) > 0)
			{
				export = swap_str(export, i);
				swapped = 1;
			}
			i++;
		}
	}
}

void	copy_declare(char *export, char *declare, int *i)
{
	int	j;

	j = 0;
	while (declare[j])
	{
		export[*i] = declare[j];
		(*i)++;
		j++;
	}
}

void	copy_temp(char *export, char *temp, int *i)
{
	int	j;
	int	equal_found;

	j = 0;
	equal_found = 0;
	while (temp[j])
	{
		if (temp[j] == '=' && !equal_found)
		{
			export[(*i)++] = temp[j++];
			export[(*i)++] = '"';
			equal_found = 1;
			continue ;
		}
		export[(*i)++] = temp[j++];
	}
	if (equal_found)
	{
		export[(*i)++] = '"';
	}
}

void	update_export_var(t_const *node, char *new_var, int index_export)
{
	if (index_export != -1)
	{
		free(node->shell->export[index_export]);
		node->shell->export[index_export] = malloc(ft_strlen("declare -x ")
				+ ft_strlen(new_var) + 1);
		if (node->shell->export[index_export])
		{
			ft_strlcpy(node->shell->export[index_export], "declare -x ",
				ft_strlen("declare -x ") + 1);
			ft_strlcat(node->shell->export[index_export], new_var,
				ft_strlen("declare -x ") + ft_strlen(new_var) + 1);
		}
	}
	else
		add_to_export(&(node->shell->export), new_var);
}
