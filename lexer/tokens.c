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


t_token *get_prev_token(t_token *head, t_token *target)
{
	t_token *curr = head;

	while (curr && curr->next != target)
		curr = curr->next;
	return curr;
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

void	token_insert_before(t_token **head, t_token *pos, \
	t_token_type type, const char *value, t_collector **collector)
{
	t_token	*new_token;
	t_token	*prev;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		exit_program(collector, "Error malloc token_insrt_bfore", EXIT_FAILURE);
	collector_append(collector, new_token);

	new_token->type = type;
	new_token->value = ft_strdup(value);
	if (!new_token->value)
		exit_program(collector, "Error malloc tokenInsBfore val", EXIT_FAILURE);
	collector_append(collector, new_token->value);
	new_token->next = pos;

	if (*head == pos)
		*head = new_token;
	else
	{
		prev = *head;
		while (prev && prev->next != pos)
			prev = prev->next;
		if (!prev)
			exit_program(collector, "toknInsrtBfore:posNotFound", EXIT_FAILURE);
		prev->next = new_token;
	}
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
		collector_remove_ptr(collector, target->value);
		// freer(target->value);
	}
	collector_remove_ptr(collector, target);
	// free(target);
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
		// ft_putstr_fd("ARA HE CREAT EL VALUE %s I ENVIO EL PUNTER %p AL COLLECTOR\n", token_new->value, token_new->value);
		collector_append(collector, token_new->value);
	}
	token_new->next = NULL;
	if (*head == NULL)
		*head = token_new;
	else
	{
		token_last = ft_lasttoken(*head);
		token_last->next = token_new;
	}
}
