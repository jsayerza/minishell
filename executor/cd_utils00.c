/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 cd_utils.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/04/03 20:06:41 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/04/03 20:06:41 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"



char	*get_home(t_shell *shell)
{
	int		i;

	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp("HOME=", shell->env[i], 5) == 0)
			return (extract_home_path(shell->env[i]));
		i++;
	}
	return (NULL);
}
