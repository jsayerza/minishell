/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 unset_export.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:15:03 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:15:03 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

char	*extract_export_name(char *export_str)
{
	char	*var_name;
	char	*export_name;

	if (ft_strncmp(export_str, "declare -x ", 11) != 0)
		return (NULL);
	export_name = ft_strdup(export_str + 11);
	if (!export_name)
		return (NULL);
	var_name = get_var_name(export_name);
	free(export_name);
	return (var_name);
}

int	should_keep_export(char *export_str, char **cmds)
{
	char	*var_name;
	int		keep;

	var_name = extract_export_name(export_str);
	if (!var_name)
		return (1);
	keep = !should_delete(var_name, cmds);
	free(var_name);
	return (keep);
}

void	unset_export(t_const *node)
{
	char	**new_export;
	int		export_len;
	int		i;
	int		j;

	export_len = len(node->shell->export);
	new_export = malloc(sizeof(char *) * (export_len + 1));
	if (!new_export)
		return ;
	i = 0;
	j = 0;
	while (node->shell->export[i])
	{
		if (should_keep_export(node->shell->export[i], node->executable))
		{
			new_export[j++] = ft_strdup(node->shell->export[i]);
		}
		i++;
	}
	new_export[j] = NULL;
	free_env(node->shell->export);
	node->shell->export = new_export;
}
