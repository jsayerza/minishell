/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/04/03 20:31:59 by acarranz         ###   ########.fr       */
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
	int				interact;

	(void)argc;
	(void)argv;
	// Mode interactiu si és terminal
	interact = isatty(STDIN_FILENO);

	// Init shell
	shell = init_shell(NULL, envp);
	if (!shell)
		exit_program(NULL, "Error al inicializar shell", 1);

	collector = NULL;

	while (1)
	{
		// Leer input interactivo o desde redirección
		if (interact)
			line = readline("minishell$ ");
		else
			line = get_next_line(STDIN_FILENO);

		if (!line)
			break ;

		// Saltar líneas vacías
		if (line[0] == '\0' || is_only_whitespace(line))
		{
			free(line);
			continue ;
		}

		// Guardar en historial si readline (solo en modo interactivo)
		if (interact)
			add_history(line);

		// Tokenizar y parsear
		tokens = NULL;
		tokens = lexer(line, &collector, &tokens);
		tokens_print(tokens);

		ast = parser(&collector, tokens, interact);
		if (ast)
		{
			printf("\n=== AST ===\n");
			ast_print(ast, 0);
			constructor = ast_to_constructor(&collector, ast, shell);
			constructor_print(constructor);
			// Aquí iría start_shell(shell); si quisieras ejecutar
		}
		else
			printf("Parse failed.\n");

		//TDOD: crec q això va fora del while
		collector_cleanup(&collector);
		free(line);
	}
	return (EXIT_SUCCESS);
}
