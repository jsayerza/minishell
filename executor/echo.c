/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 echo.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:51 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:51 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void	echo(t_constructor *node)
{
	char	**args;
	int		jump;
	int		i;

	jump = 0;
	i = 1;
	args = node->executable;
	if (!args || !args[0])
		return ;
	while (args[i] && strcmp(args[i], "-n") == 0)
	{
		jump = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!jump)
		printf("\n");
}
