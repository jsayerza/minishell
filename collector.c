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

	// ft_putstr_fd("\nIN collector_remove_ptr\n");
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
			{
				// ft_putstr_fd("  Freer removed ptr: %p\n", curr->ptr);
				freer(curr->ptr); // 👈 Evitas el leak
			}
			free(curr);
			// ft_putstr_fd("  Removed from collector: %p\n", ptr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
	// ft_putstr_fd("OUT collector_remove_ptr\n");
}


void	collector_cleanup(t_collector **collector)
{
	t_collector	*current;
	t_collector	*next;

	// ft_putstr_fd("\nIN collector_cleanup\n");
	current = *collector;
	while (current)
	{
		next = current->next;
		// ft_putstr_fd("  Free pointer:  %p\n", current);
		// ft_putstr_fd("    Free pointer value: %s\n", (char *)current->ptr);
		if (current->ptr)
			freer(current->ptr);
		if (current)
			free(current);
		current = next;
	}
	*collector = NULL;
	// ft_putstr_fd("OUT collector_cleanup\n\n");
}

bool	collector_contains(t_collector *collector, void *ptr)
{
	while (collector)
	{
		if (collector->ptr == ptr)
		{
			// ft_putstr_fd("      YURATENXIONPLIS!!!!!!!!! Pointer already in collector: %p\n", ptr);
			return (true);
		}
		collector = collector->next;
	}
	return (false);
}

void	collector_append(t_collector **collector, void *ptr)
{
	t_collector	*new_node;

	// ft_putstr_fd("  Appending to collector: %p\n", ptr);
	if (!ptr || collector_contains(*collector, ptr))
		return ;
	new_node = malloc(sizeof(t_collector));
	if (!new_node)
		exit_program(collector, "Error malloc collector", EXIT_FAILURE);
	new_node->ptr = ptr;
	new_node->next = *collector;
	*collector = new_node;
	// if (ptr)
	// 	ft_putstr_fd("    Pointer value: %s\n", (char *)ptr);
	// else
	// 	ft_putstr_fd("    Pointer value: NULL\n");
}
