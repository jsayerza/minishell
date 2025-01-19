/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/01/16 17:44:28 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	t_collector	*collector;
	t_token		*tokens;
	const char	*input;

	input = "echo \"$USER\" 'hello world' | grep test >> output.txt";
	collector = NULL;
	tokens = NULL;
	tokens = lexer(input, &collector, &tokens);
	tokens_print(tokens);
	collector_cleanup(&collector);
	return (EXIT_SUCCESS);
}
