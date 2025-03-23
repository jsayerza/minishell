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

void	process_input(t_shell *shell, char *input)
{
	if (strcmp(input, "exit") == 0)
	{
		free(input);
		exit(0);
	}
	if (shell->constructor)
	{
		clean_constructor(shell->constructor);
		shell->constructor = NULL;
	}
	shell->constructor = fill_constructor_manually(shell);
	print_constructor(shell);
	display_shell(shell);
	free(input);
}

void	start_shell(t_shell *shell)
{
	char	*input;

	while (1)
	{
		shell->node_size = 0;
		printf("~/minishell: ");
		input = read_input();
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		process_input(shell, input);
	}
	clean_shell(shell);
}
