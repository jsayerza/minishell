/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:30:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/15 19:10:42 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	create_constructor_node_builtin(t_constructor *node)
{
	if (node->executable && node->executable[0])
	{
		if (!ft_strcmp(node->executable[0], "echo"))
			node->builtin = BUILTIN_ECHO;
		else if (!ft_strcmp(node->executable[0], "cd"))
			node->builtin = BUILTIN_CD;
		else if (!ft_strcmp(node->executable[0], "pwd"))
			node->builtin = BUILTIN_PWD;
		else if (!ft_strcmp(node->executable[0], "export"))
			node->builtin = BUILTIN_EXPORT;
		else if (!ft_strcmp(node->executable[0], "unset"))
			node->builtin = BUILTIN_UNSET;
		else if (!ft_strcmp(node->executable[0], "env"))
			node->builtin = BUILTIN_ENV;
		else if (!ft_strcmp(node->executable[0], "exit"))
			node->builtin = BUILTIN_EXIT;
	}
}

static void	add_redirect_file_in(t_collector **collector, t_constructor *node, char *file)
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

static void	add_redirect_file_out(t_collector **collector, t_constructor *node, char *file)
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

static void	add_redirect_file_append(t_collector **collector, t_constructor *node, char *file)
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

static void	add_heredoc(t_collector **collector, t_constructor *node, char *file)
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
t_constructor	*create_constructor_node(t_collector **collector, \
	t_ast *ast, t_shell *shell)
{
	t_constructor	*node;

	node = malloc(sizeof(t_constructor));
	if (!node)
		exit_program(collector, "Error malloc constructor node", EXIT_FAILURE);
	node->executable = ast->args;
	node->size_exec = 0;
	while (node->executable && node->executable[node->size_exec])
		node->size_exec++;
	node->redirect_in = NULL;
	node->redirect_out = NULL;
	node->redirect_append = NULL;
	node->heredoc = NULL;
	node->pipe_in = 0;
	node->pipe_out = 0;
	node->shell = shell;
	node->pid = -1;
	node->builtin = BUILTIN_NONE;
	node->type = ast->type;
	node->error = 0;
	node->next = NULL;
	node->prev = NULL;
	create_constructor_node_builtin(node);
	collector_append(collector, node);
	return (node);
}

static void	set_pipe_flags_and_link(t_constructor *left, t_constructor *right)
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

static t_constructor	*find_or_create_command_node(t_collector **collector, \
	t_ast *ast, t_shell *shell, t_constructor **first_node)
{
	t_constructor	*cmd_node;
	t_constructor	*curr;

	curr = *first_node;
	while (curr)
	{
		if (curr->type == TOKEN_COMMAND)
			return (curr);
		curr = curr->next;
	}
	cmd_node = malloc(sizeof(t_constructor));
	if (!cmd_node)
		exit_program(collector, "Error malloc command node", EXIT_FAILURE);
	if (ast && ast->args)
 	   cmd_node->executable = ast->args;
	else
   		cmd_node->executable = NULL;
	cmd_node->size_exec = 0;
	while (cmd_node->executable && cmd_node->executable[cmd_node->size_exec])
		cmd_node->size_exec++;
	cmd_node->redirect_in = NULL;
	cmd_node->redirect_out = NULL;
	cmd_node->redirect_append = NULL;
	cmd_node->heredoc = NULL;
	cmd_node->pipe_in = 0;
	cmd_node->pipe_out = 0;
	cmd_node->shell = shell;
	cmd_node->pid = -1;
	cmd_node->builtin = BUILTIN_NONE;
	cmd_node->type = TOKEN_COMMAND;
	cmd_node->error = 0;
	cmd_node->next = *first_node;
	cmd_node->prev = NULL;
	if (*first_node)
		(*first_node)->prev = cmd_node;
	*first_node = cmd_node;
	collector_append(collector, cmd_node);
	return (cmd_node);
}

static t_constructor	*process_ast_node(t_collector **collector, t_ast *ast, t_shell *shell)
{
	t_constructor	*first_node;
	t_constructor	*left_nodes;
	t_constructor	*right_nodes;
	t_constructor	*cmd_node;
	t_constructor	*curr;

	if (!ast)
		return (NULL);
	if (ast->type == TOKEN_WORD)
		return (create_constructor_node(collector, ast, shell));
	if (ast->type == TOKEN_COMMAND)
		return (create_constructor_node(collector, ast, shell));
	left_nodes = process_ast_node(collector, ast->left, shell);
	right_nodes = process_ast_node(collector, ast->right, shell);
	if (ast->type == TOKEN_PIPE)
	{
		if (!left_nodes)
    		return right_nodes;
		if (!right_nodes)
    		return left_nodes;
		curr = left_nodes;
		while (curr->next)
			curr = curr->next;

		set_pipe_flags_and_link(curr, right_nodes);
		return (left_nodes);
	}
	first_node = left_nodes;
	cmd_node = find_or_create_command_node(collector, ast, shell, &first_node);
	if (ast->type == TOKEN_REDIRECT_IN && ast->file)
			add_redirect_file_in(collector, cmd_node, ast->file);
	if (ast->type == TOKEN_REDIRECT_OUT && ast->file)
			add_redirect_file_out(collector, cmd_node, ast->file);
	if (ast->type == TOKEN_APPEND && ast->file)
			add_redirect_file_append(collector, cmd_node, ast->file);
	if (ast->type == TOKEN_HEREDOC && ast->heredoc_content)
			add_heredoc(collector, cmd_node, ast->heredoc_content);
	if (right_nodes)
	{
		curr = first_node;
		while (curr->next)
			curr = curr->next;
		curr->next = right_nodes;
		right_nodes->prev = curr;
	}
	return (first_node);
}

t_constructor	*ast_to_constructor(t_collector **collector, t_ast *ast, t_shell *shell)
{
	printf("IN ast_to_constructor\n");
	return (process_ast_node(collector, ast, shell));
}
