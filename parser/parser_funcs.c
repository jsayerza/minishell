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

static int	parse_cmd_handle_word(t_ast *cmd_node, t_token **tokens,
	t_collector **collector, int *i)
{
	char	*arg_val;
	t_token	*curr;

	curr = *tokens;
	arg_val = ft_strdup(curr->value);
	if (!arg_val)
		exit_program(collector, "Error malloc parse t_word args", EXIT_FAILURE);
	collector_append(collector, arg_val);
	cmd_node->args[*i] = arg_val;
	(*i)++;
	*tokens = curr->next;
	return (false);
}

static int	parse_cmd_hle_redir(t_token **tokens, t_token *curr, \
	t_ast **final_node, t_collector **collector)
{
	t_ast	*redir_node;
	t_token	*target;

	*tokens = curr->next;
	target = curr->next;
	if (!target || target->type != TOKEN_WORD)
	{
		printf("minishell: syntax error near unexpected token `%s`\n",
			curr->value);
		return (true);
	}
	redir_node = init_redir_node(collector, curr, target);
	if (!redir_node)
		return (true);
	redir_node->left = *final_node;
	*final_node = redir_node;
	*tokens = (*tokens)->next;
	return (false);
}

static t_ast	*parse_command_init_cmd_node(t_collector **collector, \
	t_ast **final_node)
{
	t_ast	*cmd_node;

	cmd_node = init_command_node(collector);
	if (!cmd_node)
		return (NULL);
	*final_node = cmd_node;
	return (cmd_node);
}

t_ast	*parse_command(t_collector **collector, t_token **tokens)
{
	t_ast	*cmd_node;
	t_ast	*final_node;
	t_token	*curr;
	int		i;

	cmd_node = parse_command_init_cmd_node(collector, &final_node);
	if (!cmd_node)
		return (NULL);
	i = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		curr = *tokens;
		if (curr->type == TOKEN_WORD)
			parse_cmd_handle_word(cmd_node, tokens, collector, &i);
		else if (curr->type >= TOKEN_REDIRECT_IN && curr->type <= TOKEN_HEREDOC)
		{
			if (parse_cmd_hle_redir(tokens, curr, &final_node, collector))
				return (NULL);
		}
		else
			break ;
	}
	cmd_node->args[i] = NULL;
	return (final_node);
}
