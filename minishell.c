/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char *av[])
{
	t_collector	*collector;
	t_token		*tokens;
	char	*input;

	if (ac == 2)
		input = av[1];
	else
	{
		printf("Posa params!!!!\n");
		return (EXIT_SUCCESS);	
	}
	collector = NULL;
	tokens = NULL;
	tokens = lexer(input, &collector, &tokens);
	tokens_print(tokens);
	collector_cleanup(&collector);
	return (EXIT_SUCCESS);
}
