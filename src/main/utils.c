/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ptr_print(void **ptr, char *msg)
{
	if (!ptr || !*ptr)
		return ;
	fprintf(stderr, "%s %p: %s\n", msg, *ptr, (char *)*ptr);
}

void	freer(void **ptr)
{
	if (!ptr || !*ptr)
		return ;
	if (*ptr == (void *)0xDEADBEEF)
	{
		return ;
	}
	free(*ptr);
	*ptr = (void *)0xDEADBEEF;
}

void	print_error(const char *msg)
{
	if (msg && *msg)
		write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

void	exit_program(t_collector **collector, const char *msg, bool should_exit)
{
	collector_cleanup(collector);
	if (msg && *msg)
		print_error(msg);
	if (should_exit)
		exit(EXIT_FAILURE);
}

void	minimal_env(t_shell *shell, t_collector **collector)
{
	static char	*hard_env[] = {
		"PWD=/home/delregne/BCN42/minishell",
		"SHLVL=1",
		"PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
		NULL
	};
	int			i;
	int			count;

	i = 0;
	count = 0;
	while (hard_env[count])
		count++;
	shell->env = malloc(sizeof(char *) * (count + 1));
	if (!shell->env)
	{
		exit_program(collector, "Error allocating env", true);
	}
	while (i < count)
	{
		shell->env[i] = ft_strdup(hard_env[i]);
		i++;
	}
	shell->env[count] = NULL;
}
