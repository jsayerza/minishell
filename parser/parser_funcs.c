/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:20:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/22 17:43:20 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast	*parse_redirection(t_collector **collector, \
	t_token **tokens, t_ast *cmd_node, int interact)
{
	t_token	*curr;
	t_ast	*redir_node;

	while (*tokens && (*tokens)->type >= TOKEN_REDIRECT_IN)
	{
		curr = *tokens;
		*tokens = curr->next;
		if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
		{
			printf("minishell: syntax error expected file after `%s`\n",\
				curr->value);
			return (NULL);
		}
		redir_node = init_redir_node(collector, curr, *tokens, \
			cmd_node, interact);
		if (!redir_node)
			return (NULL);
		cmd_node = redir_node;
		*tokens = (*tokens)->next;
	}
	return (cmd_node);
}

t_ast	*parse_command(t_collector **collector, t_token **tokens, int interact)
{
	t_ast	*node;
	int		i;

	if (!(*tokens))
		return (NULL);

	// Procesar asignaciones iniciales
	node = init_command_node(collector);
	if (!node)
		return (NULL);
	i = 0;
	/*
	// Guardar asignaciones previas en un array temporal
	while (*tokens && (*tokens)->type == TOKEN_WORD && is_assignment((*tokens)->value))
	{
		node->envp[i] = ft_strdup((*tokens)->value);
		if (!node->envp[i])
			exit_program(collector, "Error malloc envp assignment", EXIT_FAILURE);
		collector_append(collector, node->envp[i]);
		i++;
		*tokens = (*tokens)->next;
	}
	node->envp[i] = NULL;
	*/
	if (!*tokens)
	{
		// Solo hay asignaciones, no comando real
		node->type = TOKEN_ENV_ASSIGN;
		return (node);
	}

	i = 0;
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		node->args[i] = ft_strdup((*tokens)->value);
		if (!node->args[i])
			exit_program(collector, \
				"Error malloc parser command args node", EXIT_FAILURE);
		collector_append(collector, node->args[i]);
		i++;
		*tokens = (*tokens)->next;
	}
	node->args[i] = NULL;
	collector_append(collector, node);
	return (parse_redirection(collector, tokens, node, interact));
}
