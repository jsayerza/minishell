/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <bits/local_lim.h>

static char	*get_short_pwd(char *cwd, t_collector **collector)
{
	char	*home;
	char	*rel;
	char	*short_pwd;

	home = getenv("HOME");
	if (home && ft_strncmp(cwd, home, ft_strlen(home)) == 0)
	{
		rel = cwd + ft_strlen(home);
		short_pwd = ft_strjoin("~", rel);
		collector_append(collector, short_pwd);
	}
	else
		short_pwd = ft_strdup(cwd);
	if (!short_pwd)
		exit_program(collector, "Error short_pwd", true);
	return (short_pwd);
}

static void	*safe_join(char **dst, char *s1, char *s2, t_collector **collector)
{
	*dst = ft_strjoin(s1, s2);
	if (!*dst)
		exit_program(collector, "Error joining strings in prompt", true);
	return (*dst);
}

static char	*join_prompt_parts(char *user, char *short_pwd,\
	t_collector **collector)
{
	char	*part1;
	char	*part2;
	char	*part3;
	char	*prompt;

	safe_join(&part1, BOLD GREEN "", user, collector);
	safe_join(&part2, part1, RESET "@", collector);
	freer(part1);
	safe_join(&part1, part2, CYAN "minishell" RESET ":", collector);
	freer(part2);
	safe_join(&part2, part1, YELLOW, collector);
	freer(part1);
	safe_join(&part3, part2, short_pwd, collector);
	freer(part2);
	safe_join(&prompt, part3, RESET "$ ", collector);
	freer(part3);
	return (prompt);
}

char	*prompt_generate(t_collector **collector)
{
	char	cwd[PATH_MAX];
	char	*user;
	char	*short_pwd;
	char	*prompt;

	if (!getcwd(cwd, sizeof(cwd)))
		exit_program(collector, "Error getcwd", true);
	user = getenv("USER");
	if (!user)
		user = "unknown";
	short_pwd = get_short_pwd(cwd, collector);
	prompt = join_prompt_parts(user, short_pwd, collector);
	collector_append(collector, prompt);
	return (prompt);
}
