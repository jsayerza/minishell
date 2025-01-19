/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	collector_cleanup(t_collector **collector)
{
	t_collector	*current;
	t_collector	*next;

	current = *collector;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	*collector = NULL;
}

void	collector_append(t_collector **collector, void *ptr)
{
	t_collector	*new_node;

	new_node = malloc(sizeof(t_collector));
	if (!new_node)
		exit_program(collector, "Error malloc collector", true);
	new_node->ptr = ptr;
	new_node->next = *collector;
	*collector = new_node;
}

void	exit_program(t_collector **collector, char *msg, bool exit_failure)
{
	collector_cleanup(collector);
	if (msg)
		perror(msg);
	if (exit_failure)
		exit(EXIT_FAILURE);
}
