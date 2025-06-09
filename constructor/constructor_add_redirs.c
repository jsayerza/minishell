/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor_add_redirs.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 21:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/29 21:07:20 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_redirections(t_collector **collector, t_ast *ast,
	t_const *cmd)
{
	if (ast->type == TOKEN_REDIRECT_IN && ast->file)
		add_redirect_file_in(collector, cmd, ast->file);
	if (ast->type == TOKEN_REDIRECT_OUT && ast->file)
		add_redirect_file_out(collector, cmd, ast->file);
	if (ast->type == TOKEN_APPEND && ast->file)
		add_redirect_file_append(collector, cmd, ast->file);
	if (ast->type == TOKEN_HEREDOC && ast->heredoc_content)
		add_heredoc(collector, cmd, ast->heredoc_content);
}
