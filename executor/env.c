/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 env.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/23 12:13:55 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/03/23 12:13:55 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void env(t_constructor *node)
{
	if (!node || !node->executable || !node->shell)
		return;
	if (node->executable[1] && ft_strcmp(node->executable[1], "-i") == 0 && node->executable[2] && ft_strcmp(node->executable[2], "bash") == 0)
		printf("env -i bash \n");
	if (node->pipe_out)
		redirect_builtin(node, node->shell->env);
	else if (!node->executable[1])
		print_builtin(node->shell->env);
}
