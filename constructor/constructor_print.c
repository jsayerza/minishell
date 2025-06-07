/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor_print.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:30:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/29 20:41:04 by acarranz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*get_builtin_name(t_builtin builtin)
{
	if (builtin == BUILTIN_ECHO)
		return ("echo");
	if (builtin == BUILTIN_CD)
		return ("cd");
	if (builtin == BUILTIN_PWD)
		return ("pwd");
	if (builtin == BUILTIN_EXPORT)
		return ("export");
	if (builtin == BUILTIN_UNSET)
		return ("unset");
	if (builtin == BUILTIN_ENV)
		return ("env");
	if (builtin == BUILTIN_EXIT)
		return ("exit");
	return ("none");
}

static const char	*get_token_type_name(t_token_type type)
{
	if (type == TOKEN_COMMAND)
		return ("COMMAND");
	if (type == TOKEN_PIPE)
		return ("PIPE");
	if (type == TOKEN_REDIRECT_IN)
		return ("REDIRECT_IN");
	if (type == TOKEN_REDIRECT_OUT)
		return ("REDIRECT_OUT");
	if (type == TOKEN_APPEND)
		return ("APPEND");
	if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	if (type == TOKEN_BUILTIN)
		return ("BUILTIN");
	return ("OTHER");
}

void	print_redirect_array(char **array, char *label)
{
	int	i;

	if (!array)
		return ;
	printf("%s\n", label);
	i = 0;
	while (array[i])
	{
		printf("  %s\n", array[i]);
		i++;
	}
}

void	print_executable(t_constructor *node)
{
	int	i;

	i = 0;
	if (!node->executable)
	{
		printf("(null)\n");
		return ;
	}
	while (node->executable[i])
	{
		printf("%s ", node->executable[i]);
		i++;
	}
	printf("\n");
}

void	constructor_print(t_constructor *list)
{
	int	index;

	index = 0;
	while (list)
	{
		printf("\n== Constructor Node %d ==\nExecutable: ", index);
		index++;
		print_executable(list);
		printf("Size exec : %d\n", list->size_exec);
		printf("Builtin: %s\n", get_builtin_name(list->builtin));
		print_redirect_array(list->redirect_in, "Redirect in:");
		print_redirect_array(list->redirect_out, "Redirect out:");
		print_redirect_array(list->redirect_append, "Append:");
		print_redirect_array(list->heredoc, "Heredoc:");
		printf("Redirect in -->%d\n", list->redirect_in_type);
		printf("Redirect out -->%d\n", list->redirect_out_type);
		printf("Token Type: %s\n", get_token_type_name(list->type));
		printf("Pipe in: %d  Pipe out: %d\n", list->pipe_in, list->pipe_out);
		printf("-------------------------\n");
		list = list->next;
	}
}
