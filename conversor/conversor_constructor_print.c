/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversor_constructor_print.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acarranz <acarranz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:30:00 by jsayerza          #+#    #+#             */
/*   Updated: 2025/05/14 16:39:43 by acarranz         ###   ########.fr       */
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

void	constructor_print(t_constructor *list)
{
	int	i;
	int	index;

	index = 0;
	while (list)
	{
		printf("\n== Constructor Node %d ==\nExecutable: ", index++);
		if (list->executable)
		{
			i = 0;
			while (list->executable[i])
				printf("%s ", list->executable[i++]);
			printf("\n");
		}
		printf("Size exec : %d\n", list->size_exec);
		printf("Builtin: %s\n", get_builtin_name(list->builtin));
		if (list->redirect_in)
		{
			printf("Redirect in:\n");
			int i = 0;
			while (list->redirect_in[i])
				printf("%s\n", list->redirect_in[i++]);
		}
		if (list->redirect_out)
		{
			printf("Redirect out:\n");
			int i = 0;
			while (list->redirect_out[i])
				printf("%s\n", list->redirect_out[i++]);
		}
		if (list->redirect_append)
		{
			printf("Append:\n");
			int i = 0;
			while (list->redirect_append[i])
				printf("%s\n", list->redirect_append[i++]);
		}

		printf("Token Type: %s\n", get_token_type_name(list->type));
		printf("Error: %d\n", list->error);
		printf("Pipe in: %d  Pipe out: %d\n", list->pipe_in, list->pipe_out);
		printf("-------------------------\n");
		list = list->next;
	}
}
