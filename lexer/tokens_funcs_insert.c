/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_funcs_insert.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_inserted_fill(t_token *new_token, t_token_type type,
	const char *value, t_collector **collector)
{
	new_token->type = type;
	new_token->value = ft_strdup(value);
	if (!new_token->value)
		exit_program(collector, "Error malloc tokenInsBfore val", EXIT_FAILURE);
	collector_append(collector, new_token->value);
}

t_token	*token_insert_before(t_token **head, t_token *pos,
	t_collector **collector)
{
	t_token	*new_token;
	t_token	*prev;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		exit_program(collector, "Error malloc token_insrt_bfore", EXIT_FAILURE);
	collector_append(collector, new_token);
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
	return (new_token);
}

void	token_insert_after(t_token *prev, t_token *new_token)
{
	if (!prev || !new_token)
		return ;
	new_token->next = prev->next;
	prev->next = new_token;
}
