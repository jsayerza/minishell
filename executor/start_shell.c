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

#include "minishell.h"

char	*read_input(void)
{
	char	*input;
	size_t	len;
	ssize_t	read;

	input = NULL;
	len = 0;
	read = getline(&input, &len, stdin);
	if (read == -1)
	{
		free(input);
		return (NULL);
	}
	if (input[read - 1] == '\n')
		input[read - 1] = '\0';
	return (input);
}

void	start_shell(t_shell *shell)
{
	char			*input;
	t_constructor	*constructor;

	while (1)
	{
		printf("~/minishell: ");
		input = read_input();
		if (!input)
		{
			printf("\n");
			break ;
		}
		if (strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		constructor = fill_constructor_manually(input);
		if (!constructor)
		{
			fprintf(stderr, "Error al crear el constructor.\n");
			free(input);
			clean_shell(shell);
			exit(1);
		}
		shell->constructor = constructor;
		print_constructor(shell);
		display_shell(shell);
		free(input);
	}
	clean_shell(shell);
}
