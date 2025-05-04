/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:00:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_variable(const char *str, int *i, int exit_status, t_collector **collector)
{
	int		start;
	char	*var_name;
	char	*var_value;
	char	*result;

	printf("IN expand_variable\n");
	(*i)++; // Skip $
	if (str[*i] == '?')
	{
		(*i)++;
		result = ft_itoa(exit_status);
		if (!result)
			exit_program(collector, "Error malloc expand_variable itoa", true);
		collector_append(collector, result);
		return (result);
	}
	start = *i;
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	var_name = ft_strndup(str + start, *i - start);
	if (!var_name)
		exit_program(collector, "Error malloc expand_variable varname", true);
	var_value = getenv(var_name);
	freer(var_name);
	if (var_value)
	{
		result = ft_strdup(var_value);
		if (!result)
			exit_program(collector, "Error malloc expand_variable value", true);
	}
	else
	{
		result = ft_strdup("");
		if (!result)
			exit_program(collector, "Error malloc expand_variable empty", true);
	}
	collector_append(collector, result);
	printf("OUT expand_variable\n");
	return (result);
}

static char	*expand_string(const char *str, int exit_status, t_collector **collector)
{
	int		i;
	char	*result;
	char	*tmp;
	char	*expanded;
	char	*joined;

	printf("IN expand_string\n");
	i = 0;
	result = ft_strdup("");
	if (!result)
		exit_program(collector, "Error malloc expand_string init", true);
	collector_append(collector, result);
	while (str[i])
	{
		if (str[i] == '\'')
		{
			int start = ++i;
			while (str[i] && str[i] != '\'')
				i++;
			if (!str[i]) // No s'ha trobat "'"
			{
				exit_program(collector, "minishell: unclosed single quotes", false);
				return (NULL);
			}				
			tmp = ft_strndup(str + start, i - start);
			if (!tmp)
				exit_program(collector, "Error malloc expand_string single quotes", true);
			joined = ft_strjoin(result, tmp);
			freer(tmp);
			if (!joined)
				exit_program(collector, "Error malloc expand_string join", true);
			result = joined;
			freer(joined);
			//TODO: No se si es correcte fer append aquí, ja que ja s'ha afegit abans
			collector_append(collector, result);
			if (str[i])
				i++;
		}
		else if (str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
			{
				if (str[i] == '$')
					expanded = expand_variable(str, &i, exit_status, collector);
				else
				{
					int start = i;
					while (str[i] && str[i] != '"' && str[i] != '$')
						i++;
					if (!str[i]) // No s'ha trobat """
					{
						exit_program(collector, "minishell: unclosed double quotes", false);
						return (NULL);
					}				
					expanded = ft_strndup(str + start, i - start);
					if (!expanded)
						exit_program(collector, "Error malloc expand_string double quotes", true);
					collector_append(collector, expanded);
				}
				tmp = ft_strjoin(result, expanded);
				if (!tmp)
					exit_program(collector, "Error malloc expand_string join inside double quotes", true);
				result = tmp;
				freer(tmp);
				collector_append(collector, result);
			}
			if (str[i])
				i++;
		}
		else if (str[i] == '$')
		{
			expanded = expand_variable(str, &i, exit_status, collector);
			tmp = ft_strjoin(result, expanded);
			if (!tmp)
				exit_program(collector, "Error malloc expand_string join var", true);
			freer(result);
			result = tmp;
			collector_append(collector, result);
		}
		else
		{
			int start = i;
			while (str[i] && str[i] != '$' && str[i] != '\'' && str[i] != '"')
				i++;
			expanded = ft_strndup(str + start, i - start);
			if (!expanded)
				exit_program(collector, "Error malloc expand_string plain text", true);
			collector_append(collector, expanded);
			tmp = ft_strjoin(result, expanded);
			if (!tmp)
				exit_program(collector, "Error malloc expand_string join plain", true);
			freer(result);
			result = tmp;
			collector_append(collector, result);
		}
	}
	printf("OUT expand_string\n");
	return (result);
}

void tokens_expand(t_token *tokens, int exit_status, t_collector **collector)
{
	char	*expanded;

	printf("IN tokens_expand\n");
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_COMMAND)
		{
			expanded = expand_string(tokens->value, exit_status, collector);
			freer(tokens->value);
			tokens->value = expanded;
		}
		tokens = tokens->next;
	}
	printf("OUT tokens_expand\n");
}
