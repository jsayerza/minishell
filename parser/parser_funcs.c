/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:20:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/22 17:43:20 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*parse_command(t_collector **collector, t_token **tokens, int interact)
{
	t_ast	*cmd_node;
	t_ast	*final_node;
	t_ast	*redir_node;
	t_token	*curr;
	int		i;

	i = 0;
	printf("IN parse_command\n");
	cmd_node = init_command_node(collector);
	if (!cmd_node)
		return (NULL);
	final_node = cmd_node; // este se actualizará si hay redirecciones

	while (*tokens && (*tokens)->type != TOKEN_PIPE)
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
