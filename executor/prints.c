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

#include "../minishell.h"

void	print_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i])
		printf("%s\n", shell->env[i++]);
}

void	print_export(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->export[i] != NULL)
		printf("%s\n", shell->export[i++]);
}
