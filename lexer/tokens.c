/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*token_get_prev(t_token *head, t_token *target)
{
	t_token	*curr;

	curr = head;
	while (curr && curr->next != target)
		curr = curr->next;
	return (curr);
}

t_token	*token_goto_last(t_token *lst)
{
	while (lst)
	{
		if (lst->next == NULL)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	token_create(t_collector **collector, t_token_type type, \
	const char *value, t_token **head)
{
	t_token	*token_new;
	t_token	*token_last;

	token_new = malloc(sizeof(t_token));
	if (!token_new)
		exit_program(collector, "Error malloc token", EXIT_FAILURE);
	collector_append(collector, token_new);
	token_new->type = type;
	token_new->value = NULL;
	if (value)
	{
		token_new->value = ft_strdup(value);
		if (!token_new->value)
			exit_program(collector, "Error malloc token value", EXIT_FAILURE);
		collector_append(collector, token_new->value);
	}
	token_new->next = NULL;
	if (*head == NULL)
		*head = token_new;
	else
	{
		token_last = token_goto_last(*head);
		token_last->next = token_new;
	}
}
