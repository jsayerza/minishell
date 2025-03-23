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

#include "../minishell.h"

int	get_export_length(char **export)
{
	int	len;

	len = 0;
	while (export[len])
		len++;
	return (len);
}

char	*extract_var_name(char *arg)
{
	char	*var_name;
	int		i;

	var_name = strdup(arg);
	i = 0;
	while (var_name[i] && var_name[i] != '=')
		i++;
	var_name[i] = '\0';
	return (var_name);
}

char	**create_new_export(int len)
{
	char	**new_export;

	new_export = malloc((len + 2) * sizeof(char *));
	if (!new_export)
	{
		perror("malloc");
		return (NULL);
	}
	return (new_export);
}

void	copy_elements(char **new_arr, char **old_arr, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		new_arr[i] = old_arr[i];
		i++;
	}
}

int	create_export_element(char **new_export, int len, char *var)
{
	new_export[len] = malloc(ft_strlen("declare -x ") + ft_strlen(var) + 1);
	if (!new_export[len])
	{
		perror("malloc");
		free(new_export);
		return (0);
	}
	ft_strlcpy(new_export[len], "declare -x ", ft_strlen("declare -x ") + 1);
	ft_strlcat(new_export[len], var, ft_strlen("declare -x ")
		+ ft_strlen(var) + 1);
	return (1);
}
