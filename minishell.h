/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 15:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/06/10 18:22:56 by acarranz         ###   ########.fr       */
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
# include <errno.h>
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

# define MAX_CMD_ARGS 256

typedef enum e_token_type
{
	TOKEN_EOF,
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
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

typedef struct s_ast
{
	t_token_type	type;
	char			**args;
	char			*file;
	char			*heredoc_content;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_collector
{
	void				*ptr;
	struct s_collector	*next;
}	t_collector;

typedef struct s_constructor	t_const;

typedef struct s_shell
{
	char			**env;
	char			**export;
	char			*path;
	char			**paths;
	int				last_exit;
	int				interactive;
	char			*output;
	int				node_size;
	int				redirect_error;
	t_const			*constructor;
	t_collector		*collector;
}	t_shell;

extern t_shell					*g_shell;

typedef struct s_constructor
{
	char			**executable;
	int				size_exec;
	int				fd[2];
	char			**redirect_in;
	char			**redirect_out;
	char			**redirect_append;
	char			**heredoc;
	t_token_type	redirect_in_type;
	t_token_type	redirect_out_type;
	int				pipe_in;
	int				pipe_out;
	pid_t			pid;
	t_builtin		builtin;
	t_token_type	type;
	t_shell			*shell;
	t_const			*next;
	t_const			*prev;
}	t_const;

// minishell_funcs.c
void	update_shlvl(t_shell *shell, int increment);
void	cleanup_shell(t_shell *shell);

// utils.c
void	freer(char *ptr);
bool	has_unclosed_quotes(const char *line);
int		is_only_whitespace(const char *str);
void	print_error(const char *msg);
void	exit_program(t_collector **collector, const char *msg,
			bool should_exit);

// prompt.c
char	*prompt_generate(t_collector **collector);

// collector.c
bool	collector_contains(t_collector *collector, void *ptr);
void	collector_remove_ptr(t_collector **collector, void *ptr);
void	collector_cleanup(t_collector **collector);
void	collector_append(t_collector **collector, void *ptr);

// lexer/lexer.c
t_token	*lexer(const char *input, t_collector **collector,
			t_token **head, t_shell *shell);

// lexer/lexer_funcs.c
int		handle_invalidchars(const char *input, int i);
void	get_quoted_str(const char *input,
			t_collector **collector, int *i, t_token **head);
void	get_word(const char *input, t_collector **collector,
			int *i, t_token **head);

// lexer/lexer_funcs_handle.c
int		handle_whitespace(const char *input, int *i);
int		handle_operator(const char *input, t_collector **collector,
			int *i, t_token **head);
int		handle_quotes(const char *input, t_collector **collector,
			int *i, t_token **head);

// lexer/lexer_funcs_operator.c
void	get_operator(const char *input, t_collector **collector,
			int *i, t_token **head);

// lexer/tokens.c
t_token	*token_get_prev(t_token *head, t_token *target);
t_token	*token_goto_last(t_token *lst);
void	token_create(t_collector **collector, t_token_type type,
			const char *value, t_token **head);

// lexer/tokens_funcs.c
void	tokens_free(t_token *head);
void	token_print(t_token *token);
void	tokens_print(t_token **head);
void	token_remove(t_token **head, t_token *target, t_collector **collector);

// lexer/tokens_funcs_insert.c
void	token_inserted_fill(t_token *new_token, t_token_type type,
			const char *value, t_collector **collector);
t_token	*token_insert_before(t_token **head, t_token *pos,
			t_collector **collector);
void	token_insert_after(t_token *prev, t_token *new_token);

// lexer/lexer_expand.c
void	tokens_expand(t_token **head, t_shell *shell, t_collector **collector);

// lexer/lexer_expand_var.c
char	*expand_variable(const char *str, int *i, t_shell *shell,
			t_collector **collector);

// lexer/lexer_expand_str.c
char	*expand_string(const char *str, t_shell *shell,
			t_collector **collector);

// lexer/lexer_expand_quotes.c
t_token	*expand_quotes(t_token **head, t_token *curr, t_shell *shell,
			t_collector **collector);
// lexer/lexer_expand_ass_quote.c
t_token	*expand_assignment_quote(t_token **head, t_token *curr,
			t_shell *shell, t_collector **collector);

// parser/parser.c // Abstract Syntax Tree (AST)
t_ast	*parser(t_collector **collector, t_token *tokens);

// parser/parser_pipeline.c
t_ast	*parse_pipeline(t_collector **collector, t_token **tokens);

// parser/parser_nodes.c
void	init_redir_node_fill_left(t_ast *redir_node,
			t_ast **final_node, t_token **tokens);
t_ast	*init_redir_node(t_collector **collector,
			t_token *curr, t_token *next);
t_ast	*init_command_node(t_collector **collector);
t_ast	*init_word_node(t_collector **collector, const char *value);

// parser/parser_funcs.c
t_ast	*parse_command(t_collector **collector, t_token **tokens);

// parser/parser_test.c
t_token	*build_test_tokens(void);

// parser/parser_heredoc.c
char	*heredoc_read(const char *delim, t_collector **collector);

// parser/parser_heredoc_should_break.c
int		heredoc_should_break(char *line, const char *delim);

// parser/parser_ast_print.c
void	ast_print_indent(int depth);
void	ast_print(t_ast *root, int depth);

// parser/parser_ast_print_type.c
void	ast_print_type(t_ast *root, int depth);

// parser/parser_utils.c
int		tokens_validate(t_token *tokens);
void	remove_trailing_newline(char *line);
int		is_assignment(const char *str);

// parser/parser_utils_token_invalid.c
int		is_token_invalid(t_token *prev, t_token *curr);

// constructor/constructor.c
t_const	*ast_to_constructor(t_collector **collector, t_ast *ast,
			t_shell *shell);
t_const	*find_or_create_node(t_collector **collector, t_ast *ast,
			t_shell *shell, t_const **first_node);
t_const	*process_ast_node(t_collector **collector, t_ast *ast,
			t_shell *shell);

// constructor/constructor_funcs.c
void	add_redirect_file_in(t_collector **collector,
			t_const *node, char *file);
void	add_redirect_file_out(t_collector **collector,
			t_const *node, char *file);
void	add_redirect_file_append(t_collector **collector,
			t_const *node, char *file);
void	add_heredoc(t_collector **collector, t_const *node,
			char *file);
void	set_pipe_flags_and_link(t_const *left, t_const *right);

// constructor/constructor_add_redirs.c
void	add_redirections(t_collector **collector, t_ast *ast,
			t_const *cmd);

// constructor/constructor_node_create.c
t_const	*constructor_node_create(t_collector **collector,
			t_ast *ast, t_shell *shell);

// constructor/constructor_cmd_node_create.c
t_const	*find_or_create_command_node(t_collector **collector,
			t_ast *ast, t_shell *shell, t_const **first_node);

// constructor/constructor_print.c
void	constructor_print(t_const *list);

//init functions
void	start_shell(t_shell *shell);
void	start_shell(t_shell *shell);
t_shell	*init_shell(t_shell *shell,
			char **env, t_collector **collector);
t_const	*init_const(t_collector **collector);

//void	construct_shell_data(t_shell *shell, char **env);
void	copy_env_to_shell(t_shell *shell, char **envv);
void	env_to_export(t_shell *shell);
void	create_export(t_shell *shell);
char	*concatenate_export(char *export, char *temp, char *declare);

//manual list
t_const	*fill_constructor_manually(t_shell *shell);

//display shell
void	display_shell(t_shell *shell);

//builtins
void	redirect_builtin(t_const *node, char **builtin);
void	token_builtins(t_const *node);
void	env(t_const *node);
void	export(t_const *node);
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
void	pwd(void);
void	echo(t_const *node);
void	unset(t_const *node);
void	unset_export(t_const *node);
void	finish_export(t_shell *shell);
char	*get_var_name(char *env_var);
int		should_delete(char *env_var, char **cmds);
int		len(char **arr);
void	free_env(char **env);
void	copy_declare(char *export, char *declare, int *i);
void	copy_temp(char *export, char *temp, int *i);
void	update_export_var(t_const *node, char *new_var, int index_export);
void	path(t_shell *shell);
char	*get_home(t_shell *shell);
char	*get_pwd(t_shell *shell);
char	*get_oldpwd(t_shell *shell);
char	*extract_home_path(t_shell *shell, const char *env_entry);
char	*extract_pwd_path(const char *env_entry);
char	*extract_oldpwd_path(const char *env_entry);
void	refresh_directori(t_shell *shell, const char *pwd, int type);
void	refresh_var(t_shell *shell);
void	cd(t_const *node);
char	*get_env_value(t_shell *shell, const char *var_name);
void	process_exit(t_const *node);
void	add_or_update_env_var(char ***env, char *key, char *value);
int		find_env_index(t_shell *shell, const char *key);
char	**copy_env_excluding_var(char **env,
			const char *var_name, int env_len);
void	remove_env_var(char ***env, const char *var_name);
void	add_or_update_env_var(char ***env, char *key, char *value);
void	check_heredoc(t_const *node);
char	*acces_path(t_const *node);
void	wait_for_child_processes(t_const *node);
void	setup_last_command_pipes(t_const *node);
void	setup_middle_command_pipes(t_const *node);
void	setup_first_command_pipes(t_const *node);
int		handle_fork_error(t_const *node, char *path);
int		handle_command_not_found(t_const *node, char *path);
void	close_all_pipes_except(t_const *node, int keep_in, int keep_out);
void	execute_in_child(t_const *node, char *path);
int		is_valid_identifier(char *str);
char	*acces_path_with_error(t_const *node, int *error_code);

//funcions fd
void	read_fd(t_const *node);

//Funciones comandos
void	token_commands(t_const *node);
char	*acces_path(t_const *node);
void	execute_first_command(t_const *node);
void	execute_command(t_const *node);

//funciones de archivos
void	check_redirect_in_file_exists(t_const *node);
void	apply_redirect_in(t_const *node);
void	apply_all_redirections(t_const *node);
void	create_append_files(t_const *node);
void	create_output_files(t_const *node);
int		validate_all_output_redirections(t_const *node);
int		validate_all_input_redirections(t_const *node);
int		validate_pipeline_redirections(t_shell *shell);
void	create_all_redirect_files(t_shell *shell);
// Nuevas funciones para manejar errores en el proceso hijo
void	handle_child_command_error(t_const *node, int error_code);
int		validate_and_apply_redirections(t_const *node);

// Nueva función para validar permisos de salida
int		validate_output_permissions(char **files);

// Modificar la signatura de execute_command_with_path:
int		execute_command_with_path(t_const *node, char *path, int error_code,
			void (*setup_pipes)(t_const *));
int		validate_all_output_redirections_no_delete(t_const *node);
int		is_empty_or_whitespace_command(char *cmd);
// Nueva función para manejar wait processes con SIGPIPE
void	wait_for_child_processes_fixed(t_shell *shell);

// Función para validar sin eliminar archivos
int		validate_output_permissions_no_delete(char **files);
// En minishell.h, agregar estas declaraciones:
void    apply_input_redirection(t_const *node);
void    apply_output_redirection(t_const *node);
void    apply_append_redirection(t_const *node);

/* Función principal exportada */
void	path(t_shell *shell);

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
void	setup_signals(void);
void	setup_child_signals(void);

// Handlers de señales
void	signal_handler(int sig);
void	signal_handler_child(int sig);
void	wait_for_child_processes(t_const *node);

//print functions
void	print_builtin(char **builtin);
void	print_token_list(t_shell *shell);
void	print_const(t_shell *shell);
void	print_path(t_shell *shell);

//clean functions
void	clean_shell(t_shell *shell);
void	clean_constructor(t_const *constructor);
void	free_path_array(char **path);

#endif
