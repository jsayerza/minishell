/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor_funcs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:30:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/29 21:07:20 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_redirect_file_in(t_collector **collector, \
	t_constructor *node, char *file)
{
	char	**new_array;
	int		i;
	int		size;

	if (!file)
		return ;
	size = 0;
	while (node->redirect_in && node->redirect_in[size])
		size++;
	new_array = malloc(sizeof(char *) * (size + 2));
	if (!new_array)
		exit_program(collector, "Error malloc redirect_in array", EXIT_FAILURE);
	node->redirect_in_type = TOKEN_REDIRECT_IN;
	i = 0;
	while (i < size)
	{
		new_array[i] = node->redirect_in[i];
		i++;
	}
	new_array[i] = file;
	new_array[i + 1] = NULL;
	node->redirect_in = new_array;
	collector_append(collector, new_array);
}

void	add_redirect_file_out(t_collector **collector, \
	t_constructor *node, char *file)
{
	char	**new_array;
	int		i;
	int		size;

	if (!file)
		return ;
	size = 0;
	while (node->redirect_out && node->redirect_out[size])
		size++;
	new_array = malloc(sizeof(char *) * (size + 2));
	node->redirect_out_type = TOKEN_REDIRECT_OUT;
	if (!new_array)
		exit_program(collector, "Error malloc redirect_in array", EXIT_FAILURE);
	i = 0;
	while (i < size)
	{
		new_array[i] = node->redirect_out[i];
		i++;
	}
	new_array[i] = file;
	new_array[i + 1] = NULL;
	node->redirect_out = new_array;
	collector_append(collector, new_array);
}

void	add_redirect_file_append(t_collector **collector, \
	t_constructor *node, char *file)
{
	char	**new_array;
	int		i;
	int		size;

	if (!file)
		return ;
	size = 0;
	while (node->redirect_append && node->redirect_append[size])
		size++;
	new_array = malloc(sizeof(char *) * (size + 2));
	node->redirect_out_type = TOKEN_APPEND;
	if (!new_array)
		exit_program(collector, "Error malloc redirect_in array", EXIT_FAILURE);
	i = 0;
	while (i < size)
	{
		new_array[i] = node->redirect_append[i];
		i++;
	}
	new_array[i] = file;
	new_array[i + 1] = NULL;
	node->redirect_append = new_array;
	collector_append(collector, new_array);
}

void	add_heredoc(t_collector **collector, t_constructor *node, \
	char *file)
{
	char	**new_array;
	int		i;
	int		size;

	if (!file)
		return ;
	size = 0;
	while (node->heredoc && node->heredoc[size])
		size++;
	new_array = malloc(sizeof(char *) * (size + 2));
	node->redirect_in_type = TOKEN_HEREDOC;
	if (!new_array)
		exit_program(collector, "Error malloc redirect_in array", EXIT_FAILURE);
	i = 0;
	while (i < size)
	{
		new_array[i] = node->heredoc[i];
		i++;
	}
	new_array[i] = file;
	new_array[i + 1] = NULL;
	node->heredoc = new_array;
	collector_append(collector, new_array);
}

void	set_pipe_flags_and_link(t_constructor *left, t_constructor *right)
{
	if (left)
	{
		left->pipe_out = (right != NULL);
		left->next = right;
	}
	if (right)
	{
		right->pipe_in = (left != NULL);
		right->prev = left;
	}
}
