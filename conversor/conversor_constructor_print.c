/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversor_constructor_print.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:30:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*get_builtin_name(t_builtin builtin)
{
	if (builtin == BUILTIN_ECHO) return "echo";
	if (builtin == BUILTIN_CD) return "cd";
	if (builtin == BUILTIN_PWD) return "pwd";
	if (builtin == BUILTIN_EXPORT) return "export";
	if (builtin == BUILTIN_UNSET) return "unset";
	if (builtin == BUILTIN_ENV) return "env";
	if (builtin == BUILTIN_EXIT) return "exit";
	return "none";
}

static const char	*get_token_type_name(t_token_type type)
{
	if (type == TOKEN_COMMAND) return "COMMAND";
	if (type == TOKEN_PIPE) return "PIPE";
	if (type == TOKEN_REDIRECT_IN) return "REDIRECT_IN";
	if (type == TOKEN_REDIRECT_OUT) return "REDIRECT_OUT";
	if (type == TOKEN_APPEND) return "APPEND";
	if (type == TOKEN_HEREDOC) return "HEREDOC";
	if (type == TOKEN_BUILTIN) return "BUILTIN";
	return "OTHER";
}

void	constructor_print(t_constructor *list)
{
	int	i;
	int	index = 0;

	while (list)
	{
		printf("\n== Constructor Node %d ==\n", index++);
		printf("Executable: ");
		if (list->executable)
		{
			i = 0;
			while (list->executable[i])
			{
				printf("%s ", list->executable[i]);
				i++;
			}
			printf("\n");
		}
		else
			printf("(null)\n");

		printf("Builtin: %s\n", get_builtin_name(list->builtin));
		printf("Read FD: %d\n", list->read_fd);
		printf("Write FD: %d\n", list->write_fd);
		printf("Token Type: %s\n", get_token_type_name(list->type));
		printf("Error: %d\n", list->error);
		list = list->next;
	}
}
