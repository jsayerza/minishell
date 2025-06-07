/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 15:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/06/07 10:23:12 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <signal.h>
# include <ctype.h>
# include <pwd.h>
# include <limits.h>
# include <sys/stat.h>
# include <errno.h>       // Para errno, ENOENT, EACCES
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include "gnl/get_next_line.h"

# define RESET    "\001\033[0m\002"
# define RED      "\001\033[31m\002"
# define GREEN    "\001\033[32m\002"
# define YELLOW   "\001\033[33m\002"
# define BLUE     "\001\033[34m\002"
# define MAGENTA  "\001\033[35m\002"
# define CYAN     "\001\033[36m\002"
# define BOLD     "\001\033[1m\002"

# define MAX_CMD_ARGS 256		//Permet fins 256 args per a un command

extern int interact;			// 1 = mode interactiu, 0 = no interactiu

typedef enum e_token_type
{
	TOKEN_EOF,			// 0
	TOKEN_WORD,			// 1
	TOKEN_PIPE,			// 2
	TOKEN_REDIRECT_IN,	// 3
	TOKEN_REDIRECT_OUT,	// 4
	TOKEN_APPEND,		// 5
	TOKEN_HEREDOC,		// 6
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_WILDCARD,
	TOKEN_DOLLAR,
	TOKEN_ESCAPE,
	TOKEN_WHITESPACE,
	TOKEN_COMMAND,
	TOKEN_DQUOTE,
	TOKEN_SQUOTE,
    TOKEN_BUILTIN,
	TOKEN_ENV_ASSIGN,
}	t_token_type;

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
}	t_builtin;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_ast {
	t_token_type	type;
	char			**args;	// Stores command arguments.
	char			*file;	// Stores file names for redirections.
	char			*heredoc_content;
	struct s_ast	*left;	// Represent child nodes for pipes & redirections
	struct s_ast	*right;	// Represent child nodes for pipes & redirections
	//char			**envp;
}	t_ast;

typedef struct s_collector
{
	void				*ptr;
	struct s_collector	*next;
}	t_collector;

typedef struct s_constructor	t_constructor;

typedef struct s_shell
{
	char			**env;				// Variables de entorno
	char			**export;			// export
	char			*path;				// path donde buscar ejecutables
	char			**paths;			// Lista de paths donde buscar ejecutables
	int				last_exit;			// Último código de salida
	int				interactive;		// 1 si es interactivo, 0 si es un script
	char			*output;			// Salida de shell
	int				node_size;			// Tamanyo lista de nodos a ejecutar
	t_constructor	*constructor;		// Estructura de ejecución
	t_collector		*collector;
}	t_shell;

typedef struct s_constructor
{
	char			**executable;   	// array de str de ejecutables
	int				size_exec;			// Elemntos a ejecutar
	int				fd[2];				// File descriptor
	char			**redirect_in;		// archivos de entrda
	char			**redirect_out;		// archivos de entrda
	char			**redirect_append;	// archivos de entrda
	char			**heredoc;			// archivos de entrda
	t_token_type	redirect_in_type;
	t_token_type	redirect_out_type;
	int				pipe_in;			// Flag ->Pipe izquierda
	int				pipe_out;			// Flag ->Pipe derecha
	pid_t			pid;				// fork()
	t_builtin		builtin;			// si es buitlin , que tipo
	t_token_type	type;				// typo de ejecutable
	t_shell			*shell;				// Enlace a shell
	t_constructor	*next;				// Sigueinte nodo
	t_constructor	*prev;				// Anterior nodo
}	t_constructor;

extern t_shell *g_shell;  // ✅ CORRECTO - es una DECLARACIÓN
// utils.c
void    freer(char *ptr);
bool	has_unclosed_quotes(const char *line);
int		is_only_whitespace(const char *str);
void	print_error(const char *msg);
void	exit_program(t_collector **collector, const char *msg,\
	bool should_exit);

// prompt.c
char	*prompt_generate(t_collector **collector);

// collector.c
bool	collector_contains(t_collector *collector, void *ptr);
void	collector_remove_ptr(t_collector **collector, void *ptr);
void	collector_cleanup(t_collector **collector);
void	collector_append(t_collector **collector, void *ptr);

// lexer/lexer.c
t_token	*lexer(const char *input, t_collector **collector, t_token **head, t_shell *shell);

// lexer/lexer_funcs.c
int		handle_invalidchars(const char *input, int i);
void	get_quoted_str(const char *input, t_collector **collector, int *i, t_token **head);
void	get_word(const char *input, t_collector **collector, int *i, t_token **head);

// lexer/lexer_funcs_handle.c
int	handle_whitespace(const char *input, int *i);
int	handle_operator(const char *input, t_collector **collector, \
	int *i, t_token **head);
int	handle_quotes(const char *input, t_collector **collector, \
	int *i, t_token **head);

// lexer/lexer_funcs_operator.c
void	get_operator(const char *input, t_collector **collector, \
	int *i, t_token **head);

// lexer/tokens.c
t_token *token_get_prev(t_token *head, t_token *target);
t_token	*token_goto_last(t_token *lst);
void	token_create(t_collector **collector, t_token_type type, \
	const char *value, t_token **head);

// lexer/tokens_funcs.c
void	tokens_free(t_token *head);
void	token_print(t_token *token);
void	tokens_print(t_token **head);
void	token_remove(t_token **head, t_token *target, t_collector **collector);

// lexer/tokens_funcs_insert.c
void	token_inserted_fill(t_token *new_token, t_token_type type, \
	const char *value, t_collector **collector);
t_token	*token_insert_before(t_token **head, t_token *pos, \
	t_collector **collector);
void	token_insert_after(t_token *prev, t_token *new_token);

// lexer/lexer_expand.c
void	tokens_expand(t_token **head, t_shell *shell, t_collector **collector);

// lexer/lexer_expand_var.c
char	*expand_variable(const char *str, int *i, t_shell *shell, \
	t_collector **collector);

// lexer/lexer_expand_str.c
char	*expand_string(const char *str, t_shell *shell, \
	t_collector **collector);

// lexer/lexer_expand_quotes.c
void	expand_quotes(t_token **head, t_token *curr, t_shell *shell, \
	t_collector **collector);
	
// lexer/lexer_expand_ass_quote.c
void	expand_assignment_quote(t_token **head, t_token *curr, \
	t_shell *shell, t_collector **collector);

// parser/parser.c // Abstract Syntax Tree (AST)
t_ast	*parser(t_collector **collector, t_token *tokens, int interact);

// parser/parser_nodes.c
t_ast	*init_redir_node(t_collector **collector, \
	t_token *curr, t_token *next, t_ast *cmd_node, int interact);
t_ast	*init_command_node(t_collector **collector);
t_ast	*init_word_node(t_collector **collector, const char *value);

// parser/parser_funcs.c
t_ast	*parse_command(t_collector **collector, t_token **tokens, int interact);

// parser/parser_test.c
t_token	*build_test_tokens(void);

// parser/parser_heredoc.c
char	*heredoc_read(const char *delim, int interact, t_collector **collector);

// parser/parser_ast_print.c
void	ast_print(t_ast *root, int depth);

// parser/parser_utils.c
int		tokens_validate(t_token *tokens);
void	remove_trailing_newline(char *line);
int		is_assignment(const char *str);

// conversor/conversor.c
t_constructor	*ast_to_constructor(t_collector **collector, \
	t_ast *ast, t_shell *shell);

// conversor/conversor_constructor_print.c
void	constructor_print(t_constructor *list);

//init functions
void	start_shell(t_shell *shell);
void			start_shell(t_shell *shell);
t_shell			*init_shell(t_shell *shell, char **env, t_collector **collector);
t_constructor *init_constructor(t_collector **collector);
//void	construct_shell_data(t_shell *shell, char **env);
void			copy_env_to_shell (t_shell *shell, char **envv);
void			env_to_export(t_shell *shell);
void			create_export(t_shell *shell);
char			*concatenate_export(char *export, char *temp, char *declare);

//manual list
t_constructor *fill_constructor_manually(t_shell *shell);

//display shell
void display_shell(t_shell *shell);

//builtins
void	redirect_builtin(t_constructor *node, char **builtin);
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
void	unset(t_constructor *node);
void	unset_export(t_constructor *node);
void	finish_export(t_shell *shell);
char	*get_var_name(char *env_var);
int		should_delete(char *env_var, char **cmds);
int		len(char **arr);
void	free_env(char **env);
void	copy_declare(char *export, char *declare, int *i);
void	copy_temp(char *export, char *temp, int *i);
void	update_export_var(t_constructor *node, char *new_var, int index_export);
void	path(t_shell *shell);
char	*get_home(t_shell *shell);
char	*get_pwd(t_shell *shell);
char	*get_oldpwd(t_shell *shell);
char	*extract_home_path(t_shell *shell, const char *env_entry);
char	*extract_pwd_path(const char *env_entry);
char	*extract_oldpwd_path(const char *env_entry);
void	refresh_directori(t_shell *shell, const char *pwd, int type);
void	refresh_var(t_shell *shell);
void	cd(t_constructor *node);
char	*get_env_value(t_shell *shell, const char *var_name);
void	process_exit(t_constructor *node);
void add_or_update_env_var(char ***env, char *key, char *value);

//funcions fd

void    read_fd(t_constructor *node);


//Funciones comandos
void	token_commands(t_constructor *node);
char	*acces_path(t_constructor *node);
void	execute_first_command(t_constructor *node);
void	execute_command(t_constructor *node);

//funciones de archivos
void check_redirect_in_file_exists(t_constructor *node);
void apply_redirect_in(t_constructor *node);
void apply_all_redirections(t_constructor *node);

/* Función principal exportada */
void path(t_shell *shell);

/* Funciones estáticas internas */
int		len_path(char **path);
char	**find_path(t_shell *shell);
char	**copy_path(char **path);
int		count_segments(char *path_str);
int		allocate_result(char ***result, int count);
int		handle_allocation_error(char **result, int i);
int		process_segment(char **result, int i, char **temp);
char	**manual_split_path(char *path_str);
void	free_path_array(char **path);
char	*get_path_value(char **env);
char	**try_alternative_path(char **env);

// Configuración de señales
void setup_signals(void);
void setup_child_signals(void);

// Handlers de señales
void signal_handler(int sig);
void signal_handler_child(int sig);
void wait_for_child_processes(t_constructor *node);

//print functions
void	print_builtin(char **builtin);
void	print_token_list(t_shell *shell);
void	print_constructor(t_shell *shell);
void	print_path(t_shell *shell);

//clean functions
void	clean_shell(t_shell *shell);
void	clean_constructor(t_constructor *constructor);
void free_path_array(char **path);

#endif
