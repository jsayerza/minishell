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

void    tokens_free(t_token *head)
{
    t_token *next;

    while (head)
    {
        next = head->next;
        free(head);
        head = next;
    }
}

void    token_print(t_token *token)
{
        printf("Type: %d, Value: '%s'\n", token->type, token->value);
}

void    tokens_print(t_token *token)
{
    while (token)
    {
        token_print(token);
        token = token->next;
    }
}

t_token *ft_lasttoken(t_token *lst)
{
     while (lst)
     {
          if (lst->next == NULL)
               return (lst);
          lst = lst->next;
     }
     return (lst);
}

void    token_create(t_collector **collector, t_token_type type, \
    const char *value, t_token **head)
{
    t_token *token_new;
    t_token *token_last;

    token_new = malloc(sizeof(t_token));
    if (!token_new)
    {
        //TODO: clean collector before exit
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    collector_append(collector, token_new);
    token_new->type = type;
    token_new->value = ft_strdup(value);
    if (!token_new->value)
    {
        //TODO: clean collector before exit
        perror("strdup");
        exit(EXIT_FAILURE);
    }
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
