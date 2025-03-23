/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 builtins.c											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:24 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:24 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void	token_builtins(t_constructor *node)
{
	printf("Builtins\n");
	if (node->builtin == BUILTIN_EXPORT)
		export(node);
	if (node->builtin == BUILTIN_ENV)
		env(node);
	if (node->builtin == BUILTIN_PWD)
		pwd();
	if (node->builtin == BUILTIN_ECHO)
		echo(node);
	if (node->builtin == BUILTIN_UNSET)
		unset(node);
}
