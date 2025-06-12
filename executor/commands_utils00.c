/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils_pipes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 12:13:24 by acarranz          #+#    #+#             */
/*   Updated: 2025/06/11 XX:XX:XX by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	has_output_redirection(t_const *node)
{
	if (!node)
		return (0);
	if ((node->redirect_out && node->redirect_out[0])
		|| (node->redirect_append && node->redirect_append[0]))
		return (1);
	return (0);
}

void	setup_first_command_pipes(t_const *node)
{
	if (node->pipe_out == 1)
		dup2(node->fd[1], STDOUT_FILENO);
	close_all_pipes_except(node, 0, 0);
}

void	setup_middle_command_pipes(t_const *node)
{
	if (node->prev && node->prev->pipe_out == 1)
		dup2(node->prev->fd[0], STDIN_FILENO);
	if (node->pipe_out == 1)
		dup2(node->fd[1], STDOUT_FILENO);
	close_all_pipes_except(node, 0, 0);
}

void	setup_last_command_pipes(t_const *node)
{
	if (node->prev && node->prev->pipe_out == 1)
		dup2(node->prev->fd[0], STDIN_FILENO);
	close_all_pipes_except(node, 0, 0);
}
