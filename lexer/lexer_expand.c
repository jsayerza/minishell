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
	//TODO: revisar aquest codi, hem de ctrlar '?'?
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
		result = ft_strdup(var_value);
	else
		result = ft_strdup("");
	if (!result)
		exit_program(collector, "Error malloc expand_variable result", true);
	// collector_append(collector, result);
	printf("OUT expand_variable\n");
	return (result);
}

static char	*expand_string(const char *str, int exit_status, t_collector **collector)
{
	int		i = 0;
	char	*result;
	char	*tmp;
	char	*expanded;
	int		start;

	printf("IN expand_string\n");
	result = ft_strdup("");
	if (!result)
		exit_program(collector, "Error malloc expand_string init", true);
	//collector_append(collector, result);
	while (str[i])
	{
		printf(" IN expand_string-str[%d]:%c\n", i, str[i]);
		if (str[i] == '\'')
		{
			start = ++i;
			while (str[i] && str[i] != '\'')
				i++;
			if (!str[i] || str[i] == '\'')
				return (freer(result), NULL);
			tmp = ft_strndup(str + start, i - start);
			if (!tmp)
				exit_program(collector, "Error malloc expand_string single quotes", true);
			expanded = ft_strjoin(result, tmp);
			freer(tmp);
			if (!expanded)
				exit_program(collector, "Error malloc expand_string join", true);
			// freer(result);
			result = expanded;
			// collector_append(collector, result);
			freer(expanded);
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
					start = i;
					while (str[i] && str[i] != '"' && str[i] != '$')
						i++;
					expanded = ft_strndup(str + start, i - start);
					if (!expanded)
						exit_program(collector, "Error malloc expand_string double quotes", true);
					// collector_append(collector, expanded);
				}
				tmp = ft_strjoin(result, expanded);
				freer(expanded);
				if (!tmp)
					exit_program(collector, "Error malloc expand_string join dq", true);
				result = ft_strdup(tmp);
				freer(tmp);
			}
			if (str[i] == '"')
				i++;
		}
		else if (str[i] == '$')
		{
			printf(" IN expand_string-$\n");
			expanded = expand_variable(str, &i, exit_status, collector);
			tmp = ft_strjoin(result, expanded);
			freer(expanded);
			if (!tmp)
				exit_program(collector, "Error malloc expand_string join var", true);
			result = ft_strdup(tmp);
			freer(tmp);
			printf(" OUT expand_string-$\n");
		}
		else
		{
			printf(" IN expand_string-A\n");
			start = i;
			while (str[i] && str[i] != '$' && str[i] != '\'' && str[i] != '"')
				i++;
			expanded = ft_strndup(str + start, i - start);
			if (!expanded)
				exit_program(collector, "Error malloc expand_string plain text", true);
			tmp = ft_strjoin(result, expanded);
			freer(expanded);
			if (!tmp)
				exit_program(collector, "Error malloc expand_string join plain", true);
			result = ft_strdup(tmp);
			printf("result: %s\n", result);
			freer(tmp);
			printf(" OUT expand_string-A\n");
		}
	}
	printf("OUT expand_string\n");
	return (result);
}

void	tokens_expand(t_token **head, int exit_status, t_collector **collector)
{
	t_token	*curr;
	t_token	*start;
	t_token	*next;
	t_token	*token_tmp;
	char	*joined;
	char	*tmp;
	char	*expanded;

	printf("IN tokens_expand\n");
	curr = *head;
	while (curr)
	{
		if (curr->type == TOKEN_DQUOTE)
		{
			start = curr;
			curr = curr->next;
			joined = ft_strdup("");
			if (!joined)
				exit_program(collector, "Error malloc tokens_expand init join", true);
			// collector_append(collector, joined);
			while (curr && curr->type != TOKEN_DQUOTE)
			{
				expanded = expand_string(curr->value, exit_status, collector);
				tmp = ft_strjoin(joined, expanded);
				freer(expanded);
				if (!tmp)
					exit_program(collector, "Error join tokens_expand", true);
				// freer(joined);
				joined = tmp;
				// collector_append(collector, joined);
				curr = curr->next;
			}
			if (!curr || curr->type != TOKEN_DQUOTE)
				exit_program(collector, "minishell: unclosed double quote", false);
			token_create(collector, TOKEN_WORD, joined, head);
			freer(joined);
			next = curr->next;
			while (start != next)
			{
				token_tmp = start;
				start = start->next;
				token_remove(head, token_tmp, collector);
			}
			curr = next;
			continue ;
		}
		//TODO: revisar aquest elseif: jo crec q no cal fer res si no s'ha de fer expand...
		else if (curr->type == TOKEN_WORD || curr->type == TOKEN_COMMAND)
		{
			expanded = expand_string(curr->value, exit_status, collector);
			curr->value = ft_strdup(expanded);
			freer(expanded);
			collector_append(collector, curr->value);
		}
		curr = curr->next;
	}
	printf("OUT tokens_expand\n");
}