/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokens_free(t_token *head)
{
	t_token	*next;

	while (head)
	{
		next = head->next;
		free(head);
		head = next;
	}
}

void	token_print(t_token *token)
{
	printf("Type: %d, Value: %s\n", token->type, token->value);
}

void	tokens_print(t_token **head)
{
	t_token	*token;

	token = *head;
	printf("\n=== Tokens ===\n");
	while (token)
	{
		token_print(token);
		token = token->next;
	}
	printf("\n");
}

void	token_remove(t_token **head, t_token *target, t_collector **collector)
{
	t_token	*prev;

	if (!head || !*head || !target)
		return ;
	if (*head == target)
		*head = target->next;
	else
	{
		prev = *head;
		while (prev && prev->next != target)
			prev = prev->next;
		if (prev && prev->next == target)
			prev->next = target->next;
	}
	if (target->value)
	{
		collector_remove_ptr(collector, target->value);
	}
	collector_remove_ptr(collector, target);
}
