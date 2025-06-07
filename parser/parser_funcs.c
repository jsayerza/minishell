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
	cmd_node = init_command_node(collector);
	if (!cmd_node)
		return (NULL);
	final_node = cmd_node; // este se actualizará si hay redirecciones

	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		curr = *tokens;
		if (curr->type == TOKEN_WORD)
		{
			cmd_node->args[i] = ft_strdup(curr->value);
			if (!cmd_node->args[i])
				exit_program(collector, "malloc args", EXIT_FAILURE);
			collector_append(collector, cmd_node->args[i]);
			i++;
			*tokens = curr->next;
		}
		else if (curr->type >= TOKEN_REDIRECT_IN && curr->type <= TOKEN_HEREDOC)
		{
			*tokens = curr->next;
			if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
			{
				return (NULL);
			}
			redir_node = init_redir_node(collector, curr, *tokens, final_node, interact);
			if (!redir_node)
				return (NULL);
			final_node = redir_node;
			*tokens = (*tokens)->next;
		}
		else
		{
			break;
		}
	}
	cmd_node->args[i] = NULL; // ✅ cmd_node sigue siendo el de tipo COMMAND
	return (final_node); // devolvemos el nodo raíz (puede ser una redirección)
}
