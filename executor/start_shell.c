/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 main.c												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/01/09 09:55:22 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/01/09 09:55:22 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

void	start_shell(t_shell *shell)
{

	// inicia bucle minishell
	// aqui pleno de forma manual la llista
	t_constructor *constructor = fill_constructor_manually();
	if (!constructor)
	{
		fprintf(stderr, "Error al crear el constructor.\n");
		clean_shell(shell);
		exit(1);
	}
	shell->constructor = constructor;
	print_constructor(shell);
	display_shell(shell);
	clean_shell(shell);
}
