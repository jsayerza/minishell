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

void	collector_remove_ptr(t_collector **collector, void *ptr)
{
	t_collector	*curr;
	t_collector	*prev;

	curr = *collector;
	prev = NULL;
	while (curr)
	{
		if (curr->ptr == ptr)
		{
			if (prev)
				prev->next = curr->next;
			else
				*collector = curr->next;
			if (curr->ptr)
				freer(curr->ptr);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	collector_cleanup(t_collector **collector)
{
	t_collector	*current;
	t_collector	*next;

	current = *collector;
	while (current)
	{
		next = current->next;
		if (current->ptr)
			freer(current->ptr);
		if (current)
			free(current);
		current = next;
	}
	*collector = NULL;
}

bool	collector_contains(t_collector *collector, void *ptr)
{
	while (collector)
	{
		if (collector->ptr == ptr)
			return (true);
		collector = collector->next;
	}
	return (false);
}

void	collector_append(t_collector **collector, void *ptr)
{
	t_collector	*new_node;

	if (!ptr || collector_contains(*collector, ptr))
		return ;
	new_node = malloc(sizeof(t_collector));
	if (!new_node)
		exit_program(collector, "Error malloc collector", EXIT_FAILURE);
	new_node->ptr = ptr;
	new_node->next = *collector;
	*collector = new_node;
}
