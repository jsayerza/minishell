/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/04/27 21:04:18 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_collector		*collector;
	t_token			*tokens;
	t_shell			*shell;
	t_ast			*ast;
	t_constructor	*constructor;
	char			*line;
	char			*prompt;
	int				interact;

	(void)argc;
	(void)argv;
	interact = isatty(STDIN_FILENO);
	shell = init_shell(NULL, envp);
	if (!shell)
		exit_program(NULL, "Error al inicializar shell", true);
	while (1)
	{
		collector = NULL;
		line = NULL;
		if (interact)
		{
			prompt = prompt_generate(&collector);
			line = readline(prompt);
			if (!line)
			{
				collector_cleanup(&collector);
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
			collector_cleanup(&collector);
			continue ;
		}
		if (interact)
			add_history(line);
		tokens = NULL;
		tokens = lexer(line, &collector, &tokens);
		freer(line);
		if (!tokens)
		{
			collector_cleanup(&collector);
			continue ;
		}
		tokens_print(tokens);
		ast = parser(&collector, tokens, interact);
		if (!ast)
		{
			collector_cleanup(&collector);
			continue ;
		}
		printf("\n=== AST ===\n");
		ast_print(ast, 0);
		constructor = ast_to_constructor(&collector, ast, shell);
		shell->constructor = constructor;
		if (constructor)
		{
			constructor_print(constructor);
			// display_shell(shell);
		}
		else
			print_error("minishell: failed to prepare command execution");
		collector_cleanup(&collector);
	}
	collector_cleanup(&collector);
	return (EXIT_SUCCESS);
}
