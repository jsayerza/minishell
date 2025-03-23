/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 create_export.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:44 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:44 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

char	*concatenate_export(char *export, char *temp, char *declare)
{
	int	i;

	i = 0;
	copy_declare(export, declare, &i);
	copy_temp(export, temp, &i);
	export[i] = '\0';
	return (export);
}

void	finish_export(t_shell *shell)
{
	char	*declare;
	char	*temp;
	int		len;
	int		i;

	declare = "declare -x ";
	i = 0;
	while (shell->export[i])
	{
		temp = ft_strdup(shell->export[i]);
		if (!temp)
			return ;
		free(shell->export[i]);
		len = ft_strlen(temp) + ft_strlen(declare) + 3;
		shell->export[i] = malloc(len * sizeof(char));
		if (!shell->export[i])
		{
			free(temp);
			return ;
		}
		concatenate_export(shell->export[i], temp, declare);
		free(temp);
		i++;
	}
}

void	create_export(t_shell *shell)
{
	sort_export(shell);
	finish_export(shell);
}
