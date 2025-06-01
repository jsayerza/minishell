/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/30 19:14:58 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_collector		*collector;
	t_collector		*cycle_collector;
	t_token			*tokens;
	t_shell			*shell;
	t_ast			*ast;
	t_constructor	*constructor;
	char			*line;
	char			*prompt;
	int				interact;

	(void)argc;
	(void)argv;
	collector = NULL;
	interact = isatty(STDIN_FILENO);
	shell = init_shell(NULL, envp, &collector);
	if (!shell)
		exit_program(&collector, "Error al inicializar shell", true);
	while (1)
	{
		cycle_collector = NULL;
		line = NULL;
		if (interact)
		{
			prompt = prompt_generate(&cycle_collector);
			line = readline(prompt);
			if (!line)
			{
				collector_cleanup(&cycle_collector);
				break ;
			}
		}
		else
			line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (line[0] == '\0' || is_only_whitespace(line))
		{
			freer(line);
			collector_cleanup(&cycle_collector);
			continue ;
		}
		if (has_unclosed_quotes(line))
		{
			print_error("minishell: unclosed quotes");
			freer(line);
			collector_cleanup(&cycle_collector);
			continue ;
		}		
		if (interact)
			add_history(line);
		tokens = NULL;
		tokens = lexer(line, &cycle_collector, &tokens, shell);
		freer(line);
		if (!tokens)
		{
			collector_cleanup(&cycle_collector);
			continue ;
		}
		// tokens_print(tokens);
		ast = parser(&cycle_collector, tokens, interact);
		if (!ast)
		{
			collector_cleanup(&cycle_collector);
			continue ;
		}
		// printf("\n=== AST ===\n");
		// ast_print(ast, 0);
		constructor = ast_to_constructor(&cycle_collector, ast, shell);
		printf("OUT ast_to_constructor\n");
		shell->constructor = constructor;
		if (constructor)
		{
			constructor_print(constructor);
			printf("\n=== Execute shell ===\n");
			display_shell(shell);
		}
		else
			print_error("minishell: failed to prepare command execution");
		collector_cleanup(&cycle_collector);
	}
	free_path_array(shell->paths);
	free_path_array(shell->env);
	free_path_array(shell->export);
	free(shell->path);
	collector_cleanup(&collector);
	return (EXIT_SUCCESS);
}
