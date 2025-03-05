/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 pipex.h											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: acarranz <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/01/09 09:57:33 by acarranz		   #+#	  #+#			  */
/*	 Updated: 2025/01/09 09:57:33 by acarranz		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#ifndef MINNISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft.h"

typedef enum e_token_error
{
	CMD_NO_ERROR,
	CMD_NOT_FOUND,			// 1
	CMD_NOT_ACCESS,			// 2
	CMD_NOT_EXEC,			// 3
	CMD_NOT_PIPE,			// 4
	CMD_NOT_FORK,			// 5
	CMD_NOT_DUP,			// 6
	CMD_NOT_CLOSE,			// 7
	CMD_NOT_WAIT,			// 8
	CMD_NOT_OPEN,			// 9
	CMD_NOT_READ,			// 10
	CMD_NOT_WRITE,			// 11
	CMD_NOT_MALLOC,			// 12
	CMD_NOT_FREE,			// 13
	CMD_SYNTAX_ERROR,		// 14
	CMD_VAR_NOT_FOUND,		// 15
	CMD_REDIRECT_AMBIGUOUS, // 16
	CMD_PIPE_INVALID,		// 17
	CMD_BUILTIN_ERROR,		// 18
	CMD_EOF,				// 19
	CMD_STDIN_CLOSED,		// 20
	CMD_SIGNAL_FAILED		// 21
} t_token_error;

typedef enum e_token_type
{
	TOKEN_COMMAND,		// 0
	TOKEN_ARGUMENT,		// 1
	TOKEN_REDIRECT_IN,	// 2
	TOKEN_REDIRECT_OUT, // 3
	TOKEN_BUILTIN,		// 4
} t_token_type;

typedef struct s_command_data
{
	char	*cmd;						// Nombre del comando
	char	*builtin;					// Nombre del builtin
	char	**flags;					// Flags para el comando
	char	**args;						// Argumentos del comando
}			t_command_data;

typedef struct s_token_list
{
	t_command_data		*command_data;	// Datos del comando
	t_token_type		type;			// Tipo de token
	t_token_error		error;			// Estado de error
	struct s_token_list	*next;			// Siguiente comando en la lista
}						t_token_list;

typedef struct s_constructor
{
	char			**paths;			// Lista de paths donde buscar ejecutables
	t_token_list	*tokens;			// Lista encadenada de tokens
}					t_constructor;

typedef struct s_last_input
{
	char				*input;			// Entrada de shell
	struct s_last_input	*next;			// Siguiente entrada en la lista
}						t_last_input;

typedef struct s_shell
{
	char			**env;				// Variables de entorno
	int				last_exit;			// Último código de salida
	int				interactive;		// 1 si es interactivo, 0 si es un script
	char			*pwd;				// Directorio actual
	char			*oldpwd;			// Directorio anterior
	char			*output;			// Salida de shell
	t_last_input	*last_input;		// Historial de entradas
	t_constructor	constructor;		// Estructura de ejecución
}					t_shell;

#endif
