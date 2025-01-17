/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 09:05:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/////////////////////////////////////////////////////////////////////////

void add_to_collector(t_collector **collector, void *ptr) {
    t_collector *new_node = malloc(sizeof(t_collector));
    if (!new_node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_node->ptr = ptr;
    new_node->next = *collector;
    *collector = new_node;
}

void cleanup_collector(t_collector **collector) {
    t_collector *current = *collector;
    while (current) {
        t_collector *next = current->next;
        free(current->ptr);
        free(current);
        current = next;
    }
    *collector = NULL;
}

/////////////////////////////////////////////////////////////////////////

void add_token(t_token **tokens, int *count, const char *value, t_mem_collector *collector) {
    t_token *new_tokens = collector_malloc(collector, (*count + 1) * sizeof(t_token));
    if (!new_tokens) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    if (*tokens) {
        memcpy(new_tokens, *tokens, *count * sizeof(t_token));
    }

    new_tokens[*count].value = collector_malloc(collector, strlen(value) + 1);
    if (!new_tokens[*count].value) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    strcpy(new_tokens[*count].value, value);

    *tokens = new_tokens;
    (*count)++;
}

t_token *lexer(const char *input, int *token_count, t_mem_collector *collector) {
    t_token *tokens = NULL;
    *token_count = 0;

    const char *start = input;
    while (*start) {
        while (*start && isspace(*start)) start++; // Skip whitespace

        if (*start == '\0') break;

        const char *end = start;
        while (*end && !isspace(*end)) end++; // Find end of the token

        size_t len = end - start;
        char *token = collector_malloc(collector, len + 1);
        if (!token) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        strncpy(token, start, len);
        token[len] = '\0';

        add_token(&tokens, token_count, token, collector);

        start = end;
    }

    return tokens;
}

int main() {
    t_mem_collector *collector = init_collector();
    if (!collector) {
        fprintf(stderr, "Failed to initialize memory collector\n");
        return 1;
    }

    const char *input = "this is a test lexer";
    int token_count = 0;

    t_token *tokens = lexer(input, &token_count, collector);

    printf("Tokens:\n");
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: %s\n", i + 1, tokens[i].value);
    }

    cleanup_collector(collector);
    return 0;
}