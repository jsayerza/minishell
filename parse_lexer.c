/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void    get_expand_var(const char *input, t_collector **collector, \
    int *i, t_token **head)
{
    int     start;
    char    *var_name;
    char    *value;
    char    *expanded;

    start = ++(*i);
    //TODO: change isalnum x ft_isalnum
    while (isalnum(input[*i]) || input[*i] == '_')
        (*i)++;
    var_name = ft_strndup(input + start, *i - start);
    value = getenv(var_name);
    free(var_name);
    if (value)
        expanded = ft_strdup(value);
    else
        expanded = ft_strdup("");
    token_create(collector, TOKEN_WORD, expanded, head);
    free(expanded);
}

static void    get_quoted_str(const char *input, t_collector **collector, \
    int *i, t_token **head)
{
    int     start;
    char    *quoted;
    char    quote_type;

    quote_type = input[*i];
    start = ++(*i);
    while (input[*i] && input[*i] != quote_type)
        (*i)++;
    quoted = ft_strndup(input + start, *i - start);
    (*i)++;
    token_create(collector, TOKEN_WORD, quoted, head);
    free(quoted);
}

static void    get_operator(const char *input, t_collector **collector, \
    int i, t_token **head)
{
    t_token_type    type;
    char            op[3];

    op[0] = input[i];
    op[1] = '\0';
    op[2] = '\0';
    if ((ft_strchr("<>", input[i]) != NULL)
        && input[i + 1] == input[i])
    {
        op[1] = input[i];
        i++;
    }
    type =
        op[0] == '|' ? TOKEN_PIPE :
        (strcmp(op, ">>") == 0 ? TOKEN_APPEND :
        (strcmp(op, "<<") == 0 ? TOKEN_HEREDOC :
        (op[0] == '>' ? TOKEN_REDIRECT_OUT : TOKEN_REDIRECT_IN)));
    token_create(collector, type, op, head);
}

static void    get_word(const char *input, t_collector **collector, \
    int *i, t_token **head)
{
    int         istart;
    char        *value;

    istart = *i;
    while (input[*i]
        && (ft_strchr(" \f\r\n\t\v|<>'\"$", input[*i]) == NULL))
        (*i)++;
    value = ft_strndup(input + istart, *i - istart);
    token_create(collector, TOKEN_WORD, value, head);
    free(value);
}

t_token *lexer(const char *input, t_collector **collector, t_token **head)
{
    int         i;

    i = 0;
    while (input[i])
    {
        // Blanks
        if (ft_strchr(" \f\r\n\t\v", input[i]) != NULL)
        {
            i++;
            continue;
        }
        // Operators '|<>'
        if (ft_strchr("|<>", input[i]) != NULL)
        {
            get_operator(input, collector, i, head);
            i++;
            continue;
        }
        // Quoted Strings
        if (ft_strchr("'\"", input[i]) != NULL)
        {
            get_quoted_str(input, collector, &i, head);
            continue;
        }
        // Environment Variables
        if (input[i] == '$')
        {
            get_expand_var(input, collector, &i, head);
            continue;
        }
        // Words (commands, arguments, etc.)
        get_word(input, collector, &i, head);
    }
    token_create(collector, TOKEN_EOF, "EOF", head);
    return (*head);
}
