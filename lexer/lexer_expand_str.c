/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expand_str.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_to_result(char *result, char *expanded,
	t_collector **collector, const char *err_msg)
{
	char	*tmp;
	char	*new_result;

	tmp = ft_strjoin(result, expanded);
	freer(expanded);
	if (!tmp)
		exit_program(collector, err_msg, true);
	freer(result);
	new_result = ft_strdup(tmp);
	freer(tmp);
	if (!new_result)
		exit_program(collector, err_msg, true);
	return (new_result);
}

static char	*expand_plain_text(const char *str, int *i,
	char *result, t_collector **collector)
{
	int		start;
	char	*expanded;

	start = *i;
	while (str[*i] && str[*i] != '$')
		(*i)++;
	expanded = ft_strndup(str + start, *i - start);
	if (!expanded)
		exit_program(collector, "Error malloc exp_str plain txt", true);
	return (append_to_result(result, expanded,
			collector, "Error malloc exp_str join plain"));
}

char	*expand_string(const char *str, t_shell *shell,
	t_collector **collector)
{
	int		i;
	char	*result;
	char	*expanded;

	i = 0;
	result = ft_strdup("");
	if (!result)
		exit_program(collector, "Error malloc expand_string init", true);
	while (str[i])
	{
		if (str[i] == '$')
		{
			expanded = expand_variable(str, &i, shell, collector);
			result = append_to_result(result, expanded,
					collector, "Error malloc expd_str join var");
		}
		else
			result = expand_plain_text(str, &i, result, collector);
	}
	return (result);
}
