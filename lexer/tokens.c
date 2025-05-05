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
	printf("Type: %d, Value: '%s'\n", token->type, token->value);
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

t_token	*ft_lasttoken(t_token *lst)
{
	while (lst)
	{
		if (lst->next == NULL)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	token_insert_after(t_token *prev, t_token *new_token)
{
	if (!prev || !new_token)
		return ;
	new_token->next = prev->next;
	prev->next = new_token;
}

void	token_remove(t_token **head, t_token *target, t_collector **collector)
{
	t_token *prev;

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
		freer(target->value);
		collector_remove_ptr(collector, target->value);
	}
	collector_remove_ptr(collector, target);
	free(target);
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
	token_new->value = ft_strdup(value);
	if (!token_new->value)
		exit_program(collector, "Error malloc token value", EXIT_FAILURE);
	collector_append(collector, token_new->value);
	token_new->next = NULL;
	if (*head == NULL)
		*head = token_new;
	else
	{
		token_last = ft_lasttoken(*head);
		token_last->next = token_new;
	}
}
