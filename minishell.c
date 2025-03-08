/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/03/08 11:37:29 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc,char **argv, char **env)
{
	t_collector	*collector;
	t_token		*tokens;
	const char	*input;
	t_shell *shell;

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
	input = " % echo \"$USER\" 'hello world' | wc -l >> output.txt";
	collector = NULL;
	tokens = NULL;
	tokens = lexer(input, &collector, &tokens);
	tokens_print(tokens);
	collector_cleanup(&collector);
	start_shell(shell);
	return (EXIT_SUCCESS);
}
