/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 15:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/03/10 22:04:26 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <ctype.h>
# include "libft/libft.h"

# define RESET    "\033[0m"
# define RED      "\033[31m"
# define GREEN    "\033[32m"

typedef struct s_constructor t_constructor;
typedef struct s_shell t_shell;


// Enum representing various token error types in the minishell application
typedef enum e_token_error
{
	NO_ERROR,                      // No error occurred
	ERROR_NOT_FOUND,               // Token not found
	ERROR_NOT_ACCESS,              // Token not accessible
	ERROR_NOT_EXEC,                // Token not executable
	ERROR_NOT_PIPE,                // Pipe operation failed
	ERROR_NOT_FORK,                // Fork operation failed
	ERROR_NOT_DUP,                 // Duplication operation failed
	ERROR_NOT_CLOSE,               // Close operation failed
	ERROR_NOT_WAIT,                // Wait operation failed
	ERROR_NOT_OPEN,                // Open operation failed
	ERROR_NOT_READ,                // Read operation failed
	ERROR_NOT_WRITE,               // Write operation failed
	ERROR_NOT_MALLOC,              // Memory allocation failed
	ERROR_NOT_FREE,                // Memory deallocation failed
	ERROR_SYNTAX_ERROR,            // Syntax error in token
	ERROR_VAR_NOT_FOUND,           // Variable not found
	ERROR_REDIRECT_AMBIGUOUS,      // Ambiguous redirection
	ERROR_PIPE_INVALID,            // Invalid pipe
	ERROR_BUILTIN_ERROR,           // Built-in command error
	ERROR_EOF,                     // End of file reached
	ERROR_STDIN_CLOSED,            // Standard input closed
	ERROR_SIGNAL_FAILED            // Signal operation failed
} e_token_error;


// De moment deixo este enum perque es amb el que treballo
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_COMMAND,
    TOKEN_BUILTIN,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
    TOKEN_HEREDOC,
	TOKEN_EOF
} e_token_type;

typedef enum e_builtin
{
    BUILTIN_NONE,
    BUILTIN_ECHO,
    BUILTIN_CD,
    BUILTIN_PWD,
    BUILTIN_EXPORT,
    BUILTIN_UNSET,
    BUILTIN_ENV,
    BUILTIN_EXIT
} e_builtin;

typedef struct s_constructor
{
	char			**paths;			// Lista de paths donde buscar ejecutables
	char			**executable;   	// array de str de ejecutables
	int				size_exec;			// Elemntos  a ejecutar
	int				fd;					// File descriptor
	e_builtin		builtin;			// si es buitlin , que tipo
	e_token_type	type;				// typo de ejecutable
	e_token_error	error;				// Estado de error
	t_shell			*shell;				//enlace a shell
	t_constructor	*next;				//sigueinte nodo o ejecutable
}					t_constructor;

typedef struct s_shell
{
	char			**env;				// Variables de entorno
	char			**export;			// export
	int				last_exit;			// Último código de salida
	int				interactive;		// 1 si es interactivo, 0 si es un script
	char			*pwd;				// Directorio actual
	char			*oldpwd;			// Directorio anterior
	char			*output;			// Salida de shell
	int				node_size;
	t_constructor	*constructor;		// Estructura de ejecución
}					t_shell;

/*
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	e_token_type;
*/
typedef struct s_token
{
	e_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_collector
{
	void				*ptr;
	struct s_collector	*next;
}	t_collector;

// parse_lexer.c
t_token	*lexer(const char *input, t_collector **collector, t_token **head);

// parse_lexer_funcs.c
void	get_expand_var(const char *input, t_collector **collector, \
	int *i, t_token **head);
void	get_quoted_str(const char *input, t_collector **collector, \
	int *i, t_token **head);
void	get_operator(const char *input, t_collector **collector, \
	int *i, t_token **head);
void	get_word(const char *input, t_collector **collector, \
	int *i, t_token **head);

// collector.c
void	collector_cleanup(t_collector **collector);
void	collector_append(t_collector **collector, void *ptr);
void	exit_program(t_collector **collector, char *msg, bool exit_failure);

// tokens.c
void	token_create(t_collector **collector, e_token_type type, \
	const char *value, t_token **head);
t_token	*ft_lasttoken(t_token *lst);
void	tokens_print(t_token *token);
void	token_print(t_token *token);
void	tokens_free(t_token *head);


//init functions
void	start_shell(t_shell *shell);
t_shell			*init_shell(t_shell *shell, char **env);
t_constructor	*init_constructor(void);

//void	construct_shell_data(t_shell *shell, char **env);
void	copy_env_to_shell (t_shell *shell, char **envv);
void	env_to_export(t_shell *shell);
void	create_export(t_shell *shell);
char	*concatenate_export(char *export, char *temp, char *declare);

//manual list
t_constructor *fill_constructor_manually(t_shell *shell);

//display shell
void display_shell(t_shell *shell);

//builtins
void	token_builtins(t_constructor *node);
void	env(t_constructor *node);
void	export(t_constructor *node);
void	sort_export(t_shell *shell);
void	finish_export(t_shell *shell);
int		get_export_length(char **export);
char	*extract_var_name(char *arg);
char	**create_new_export(int len);
void	copy_elements(char **new_arr, char **old_arr, int len);
int		create_export_element(char **new_export, int len, char *var);
void	add_to_export(char ***export, char *var);
int		var_value(char *var);
char	*process_value(char *new_var, char *var);
int		find_in_env(char **env, char *var_name);
int		find_in_export(char **export, char *var_name);
int		update_existing_var(char ***env, char *var, int index);
char	**create_new_env(int len);
void	add_new_element(char **new_env, char *var, int len);
void	pwd();
void	echo(t_constructor *node);

//print functions
void	print_env(t_shell *shell);
void	print_export(t_shell *shell);
void	print_token_list(t_shell *shell);
void 	print_constructor(t_shell *shell);


//clean functions
void	clean_shell(t_shell *shell);
void	clean_constructor(t_constructor *constructor);

#endif
