/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expand_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_value_lexer(const char *name, t_shell *shell)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(name);
	while (shell->env && shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], name, len) && shell->env[i][len] == '=')
			return (shell->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*expand_special_cases(const char *str, int *i,
	t_shell *shell, t_collector **collector)
{
	char	*result;

	if (str[*i] == '?')
	{
		(*i)++;
		result = ft_itoa(shell->last_exit);
		if (!result)
			exit_program(collector, "Error malloc expand_variable itoa", true);
		return (result);
	}
	if (!ft_isalnum(str[*i]) && str[*i] != '_')
	{
		result = ft_strdup("$");
		if (!result)
			exit_program(collector, "Error malloc expand_var lone $", true);
		return (result);
	}
	return (NULL);
}

static char	*expand_env_variable(const char *str, int *i,
	t_shell *shell, t_collector **collector)
{
	int		start;
	char	*var_name;
	char	*var_value;
	char	*result;

	start = *i;
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	var_name = ft_strndup(str + start, *i - start);
	if (!var_name)
		exit_program(collector, "Error malloc expand_variable varname", true);
	var_value = get_env_value_lexer(var_name, shell);
	freer(var_name);
	if (var_value)
		result = ft_strdup(var_value);
	else
		result = ft_strdup("");
	if (!result)
		exit_program(collector, "Error malloc expand_variable result", true);
	return (result);
}

char	*expand_variable(const char *str, int *i,
	t_shell *shell, t_collector **collector)
{
	char	*result;

	(*i)++;
	result = expand_special_cases(str, i, shell, collector);
	if (result)
		return (result);
	return (expand_env_variable(str, i, shell, collector));
}
