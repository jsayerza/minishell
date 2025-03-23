/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 pwd.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:14:40 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:14:40 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
		printf("%s\n", pwd);
	else
		printf("minishell: pwd: PWD not set\n");
}
