/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 export_utils01.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:14:19 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:14:19 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_identifier(char *str)
{
	int		i;
	char	*equal_pos;

	equal_pos = ft_strchr(str, '=');
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i] && (equal_pos == NULL || &str[i] < equal_pos))
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	add_to_export(char ***export, char *var)
{
	int		len;
	char	**new_export;

	len = get_export_length(*export);
	new_export = create_new_export(len);
	if (!new_export)
		return ;
	copy_elements(new_export, *export, len);
	if (!create_export_element(new_export, len, var))
		return ;
	new_export[len + 1] = NULL;
	free(*export);
	*export = new_export;
}

int	var_value(char *var)
{
	int	i;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	if (var[i] == '=')
		return (1);
	else
		return (-1);
}

char	*process_value(char *new_var, char *var)
{
	int	len;
	int	i;
	int	j;

	len = ft_strlen(var);
	new_var = (char *)malloc(sizeof(char) * (len + 3));
	if (!new_var)
		return (NULL);
	i = 0;
	j = 0;
	while (var[i] && var[i] != '=')
		new_var[j++] = var[i++];
	if (var[i] == '=')
	{
		new_var[j++] = var[i++];
		new_var[j++] = '\"';
		while (var[i])
			new_var[j++] = var[i++];
		new_var[j++] = '\"';
	}
	new_var[j] = '\0';
	return (new_var);
}
