/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freer(char *ptr)
{
	if (!ptr)
		return ;
	free(ptr);
	ptr = NULL;
}

int	is_only_whitespace(const char *str)
{
	while (*str)
	{
		if (!ft_strchr(" \t\n\r\v\f", *str))
			return (0);
		str++;
	}
	return (1);
}

bool	has_unclosed_quotes(const char *line)
{
	int		i;
	bool	in_single;
	bool	in_double;

	i = 0;
	in_single = false;
	in_double = false;
	while (line[i])
	{
		if (line[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}

void	print_error(const char *msg)
{
	if (msg && *msg)
		write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

void	exit_program(t_collector **collector, const char *msg, bool should_exit)
{
	collector_cleanup(collector);
	if (msg && *msg)
		print_error(msg);
	if (should_exit)
		exit(EXIT_FAILURE);
}
