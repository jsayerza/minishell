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

int	main(int argc, char **argv, char **env)
{
	t_collector	*collector;
	t_token		*tokens;
	const char	*input;
	t_shell		*shell;
	t_ast		*ast;

	(void)argv;
	if (argc != 1)
		return (-1);
	shell = NULL;
	shell = init_shell(shell, env);
	if (!shell)
	{
		perror("Error al inicializar la shell.\n");
		exit(1);
	}
	collector = NULL;
	//// Usarem build_test_toks() temporalm. x enter tokens manualm dins token
	// tokens = build_test_tokens();
	//// Usarem aquest imput temporalment per simular l'entrada des de shell
	input = "echo hello | grep h > output.txt";
	tokens = NULL;
	tokens = lexer(input, &collector, &tokens);
	tokens_print(tokens);
	ast = parser(&collector, tokens);
	if (ast)
	{
		printf("\n=== AST ===\n");
		ast_print(ast, 0);
	}
	else
		printf("Parse failed.\n");
	// start_shell(shell);
	collector_cleanup(&collector);
	return (EXIT_SUCCESS);
}
