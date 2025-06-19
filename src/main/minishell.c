/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/06/19 19:14:47 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_line(char *line, t_shell *shell, t_collector **cycle)
{
	t_token			*tokens;
	t_ast			*ast;
	t_const			*constructor;

	add_history(line);
	tokens = NULL;
	tokens = lexer(line, cycle, &tokens, shell);
	freer((void **)&line);
	if (!tokens)
		return ;
	ast = parser(cycle, tokens);
	if (!ast)
		return ;
	constructor = ast_to_constructor(cycle, ast, shell);
	shell->constructor = constructor;
	if (constructor)
		display_shell(shell);
	else
		print_error("minishell: failed to prepare command execution");
}

static int	handle_line_errors(char *line, t_collector **cycle)
{
	if (line[0] == '\0' || is_only_whitespace(line))
	{
		freer((void **)&line);
		collector_cleanup(cycle);
		return (1);
	}
	if (has_unclosed_quotes(line))
	{
		print_error("minishell: unclosed quotes");
		freer((void **)&line);
		collector_cleanup(cycle);
		return (1);
	}
	return (0);
}

static int	minishell_cycle(t_shell *shell, t_collector **cycle)
{
	char	*line;
	char	*prompt;

	*cycle = NULL;
	line = NULL;
	prompt = prompt_generate(cycle);
	line = readline(prompt);
	if (!line)
	{
		collector_cleanup(cycle);
		return (0);
	}
	if (handle_line_errors(line, cycle))
		return (1);
	process_line(line, shell, cycle);
	return (1);
}

static void	minishell_loop(t_shell *shell)
{
	t_collector	*cycle;

	cycle = NULL;
	while (1)
	{
		if (!minishell_cycle(shell, &cycle))
			break ;
		collector_cleanup(&cycle);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_collector	*collector;
	t_shell		*shell;

	(void )argc;
	(void )argv;
	collector = NULL;
	shell = init_shell(NULL, envp, &collector);
	if (!shell->env || !shell->env[0])
		minimal_env(shell, &collector);
	if (!shell)
		exit_program(&collector, "Error initializing shell", true);
	g_shell = shell;
	setup_signals();
	update_shlvl(shell, 1);
	minishell_loop(shell);
	cleanup_shell(shell);
	collector_cleanup(&collector);
	return (EXIT_SUCCESS);
}
