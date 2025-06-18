/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 prints.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:14:36 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:14:36 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_builtin(t_const *node, char **builtin)
{
	int	i;

	i = 0;
	while (builtin[i])
	{
		ft_putstr_fd(builtin[i++], 1);
		ft_putstr_fd("\n", 1);
	}
	node->shell->last_exit = 0;
}
