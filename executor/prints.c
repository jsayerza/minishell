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

void print_env(t_shell *shell)
{
	printf("Print Env\n");
	int	i;

	i = 0;
	while (shell->env[i])
		printf("%s\n", shell->env[i++]);
}

void	print_path(t_shell *shell)
{
	int i;

	i = 0;
	if (!shell || !shell->paths)
	{
		printf("No hay paths para imprimir\n");
		return;
	}
	printf("DEBUG - Contenido de shell->paths:\n");
	while (shell->paths[i])
	{
		printf("[%d]: '%s'\n", i, shell->paths[i]);
		i++;
	}
	printf("Total de paths: %d\n", i);
}

void print_export(t_shell *shell)
{
	int	i;


	i = 0;
	while (shell->export[i] != NULL)
		printf("%s\n", shell->export[i++]);
}

void print_constructor(t_shell *shell)
{
	if (!shell || !shell->constructor)
	{
		fprintf(stderr, "Error: Constructor no inicializado.\n");
		return;
	}
	t_constructor *current = shell->constructor;
	int node_number = 0;
	while (current)
	{
		printf("\n----- Nodo %d -----\n", node_number);
		printf("Tipo de token: %d\n", current->type);
		printf("Tipo de builtin: %d\n", current->builtin);
		printf("Estado de error: %d\n", current->error);
		printf("Número de ejecutables: %d\n", current->size_exec); // Mostrar el número de ejecutables
		printf("Ejecutables: ");
		for (int i = 0; current->executable[i]; i++)
			printf("\"%s\" ", current->executable[i]);
		printf("\n========================\n");
		current = current->next;
		node_number++;
	}
}
