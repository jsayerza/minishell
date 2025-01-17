/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 15:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/20 15:50:30 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <ctype.h>
# include "libft/libft.h"

# define RESET    "\033[0m"
# define RED      "\033[31m"
# define GREEN    "\033[32m"

typedef enum    e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_EOF
}   t_token_type;

typedef struct  s_token {
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;

typedef struct  s_collector {
    void                *ptr;
    struct s_collector  *next;
}   t_collector;

// lexer.c
t_token *lexer(const char *input, t_collector **collector, t_token **head);

// collector.c
void    collector_cleanup(t_collector **collector);
void    collector_append(t_collector **collector, void *ptr);

// tokens.c
void    token_create(t_collector **collector, t_token_type type, \
    const char *value, t_token **head);
t_token *ft_lasttoken(t_token *lst);
void    tokens_print(t_token *token);
void    token_print(t_token *token);
void    tokens_free(t_token *head);

// parse_utils.c
int     is_operator(char c);
int     is_space(char c);

#endif
