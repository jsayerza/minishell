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

void	print_builtin(char **builtin)
{
	int	i;

	i = 0;
	while (builtin[i])
		printf("%s\n", builtin[i++]);
}

void print_constructor(t_shell *shell)
{
    t_constructor *current = shell->constructor;
    int node_count = 0;

    if (!current)
    {
        printf("Constructor list is empty.\n");
        return;
    }

    printf("\n" CYAN "=== CONSTRUCTOR LIST ===\n" RESET);
    
    // Recorrer hacia atrás hasta el primer nodo si es necesario
    while (current && current->prev)
        current = current->prev;

    // Recorrer todos los nodos desde el principio
    while (current)
    {
        printf("\n" YELLOW "--- Node %d ---\n" RESET, ++node_count);
        
        // Información básica del nodo
        printf("Type: ");
        switch (current->type)
        {
            case TOKEN_COMMAND: printf("COMMAND"); break;
            case TOKEN_BUILTIN: printf("BUILTIN"); break;
            case TOKEN_PIPE: printf("PIPE"); break;
            case TOKEN_REDIRECT_IN: printf("REDIRECT_IN"); break;
            case TOKEN_REDIRECT_OUT: printf("REDIRECT_OUT"); break;
            case TOKEN_APPEND: printf("APPEND"); break;
            case TOKEN_HEREDOC: printf("HEREDOC"); break;
            default: printf("UNKNOWN (%d)", current->type);
        }
        printf("\n");

        // Información de builtin si es aplicable
        if (current->builtin != BUILTIN_NONE)
        {
            printf("Builtin: ");
            switch (current->builtin)
            {
                case BUILTIN_ECHO: printf("echo"); break;
                case BUILTIN_CD: printf("cd"); break;
                case BUILTIN_PWD: printf("pwd"); break;
                case BUILTIN_EXPORT: printf("export"); break;
                case BUILTIN_UNSET: printf("unset"); break;
                case BUILTIN_ENV: printf("env"); break;
                case BUILTIN_EXIT: printf("exit"); break;
                default: printf("UNKNOWN (%d)", current->builtin);
            }
            printf("\n");
        }

        // Executable y argumentos
        if (current->executable)
        {
            printf("Executable: %s\n", current->executable[0]);
            printf("Arguments: ");
            for (int i = 1; current->executable[i]; i++)
                printf("[%s] ", current->executable[i]);
            printf("\n");
        }

        // Flags y descriptores de archivo
        printf("Flags: ");
        if (current->pipe_in) printf("PIPE_IN ");
        if (current->pipe_out) printf("PIPE_OUT ");
        if (current->read_fd) printf("READ_FD ");
        if (current->write_fd) printf("WRITE_FD ");
        if (current->append) printf("APPEND ");
        if (current->expand) printf("EXPAND ");
        printf("\n");

        printf("File descriptors: fd[0]=%d, fd[1]=%d\n", current->fd[0], current->fd[1]);
        printf("PID: %d\n", current->pid);
        
        // Información de error
        if (current->error != NO_ERROR)
        {
            printf("Error: ");
            switch (current->error)
            {
                case ERROR_NOT_FOUND: printf("NOT_FOUND"); break;
                case ERROR_NOT_ACCESS: printf("NOT_ACCESS"); break;
                // ... otros casos de error ...
                default: printf("UNKNOWN_ERROR (%d)", current->error);
            }
            printf("\n");
        }

        current = current->next;
    }
    
    printf("\n" CYAN "=== END OF LIST (%d nodes) ===\n\n" RESET, node_count);
}
