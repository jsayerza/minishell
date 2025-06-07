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

static char	*expand_variable(const char *str, int *i, t_shell *shell, t_collector **collector)
{
	int		start;
	char	*var_name;
	char	*var_value;
	char	*result;

	(*i)++;
	//TODO: revisar aquest codi
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
			exit_program(collector, "Error malloc expand_variable lone $", true);
		return (result);
	}
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

static char	*expand_string(const char *str, t_shell *shell, t_collector **collector)
{
	int		i;
	char	*result;
	char	*tmp;
	char	*expanded;
	int		start;

	i = 0;
	result = ft_strdup("");
	if (!result)
		exit_program(collector, "Error malloc expand_string init", true);
	while (str[i])
	{
		if (str[i] == '$')
		{
			expanded = expand_variable(str, &i, shell, collector);
			tmp = ft_strjoin(result, expanded);
			freer(expanded);
			if (!tmp)
				exit_program(collector, "Error malloc expand_string join var", true);
			freer(result);
			result = ft_strdup(tmp);
			freer(tmp);
		}
		else
		{
			start = i;
			while (str[i] && str[i] != '$')
				i++;
			expanded = ft_strndup(str + start, i - start);
			if (!expanded)
				exit_program(collector, "Error malloc expand_string plain text", true);
			tmp = ft_strjoin(result, expanded);
			freer(expanded);
			if (!tmp)
				exit_program(collector, "Error malloc expand_string join plain", true);
			freer(result);
			result = ft_strdup(tmp);
			freer(tmp);
		}
	}
	return (result);
}

void	tokens_expand(t_token **head, t_shell *shell, t_collector **collector)
{
	t_token	*curr;
	t_token	*start;
	t_token	*next;
	t_token	*token_tmp;
	t_token *prev;
	t_token	*del;
	char	*joined;
	char	*tmp;
	char	*expanded;

	curr = *head;
	while (curr)
	{
		if ((curr->type == TOKEN_DQUOTE || curr->type == TOKEN_SQUOTE))
		{
			prev = get_prev_token(*head, curr);
			if (prev && prev->type == TOKEN_WORD && ft_strchr(prev->value, '='))
			{
				start = curr;
				curr = curr->next;
				joined = ft_strdup("");
				if (!joined)
					exit_program(collector, "Error malloc tokens_expand assign+quote", true);
				while (curr && curr->type != start->type)
				{
					if (start->type == TOKEN_DQUOTE)
						expanded = expand_string(curr->value, shell, collector);
					else
						expanded = ft_strdup(curr->value);
					if (!expanded)
						exit_program(collector, "Error strdup/expand assign+quote", true);

					tmp = ft_strjoin(joined, expanded);
					freer(expanded);
					if (!tmp)
						exit_program(collector, "Error join assign+quote", true);
					freer(joined);
					joined = tmp;
					curr = curr->next;
				}
				if (!curr || curr->type != start->type)
					exit_program(collector, "minishell: unclosed quote in assign", false);
				next = curr->next;
				//
				tmp = ft_strjoin(prev->value, joined);
				// collector_remove_ptr(collector, prev->value);  // <-- important
				if (!collector_contains(*collector, prev->value))
					freer(prev->value);
				prev->value = tmp;
				collector_append(collector, prev->value);  // <-- opcional
				//
				freer(joined);
				del = start;
				while (del != next)
				{
					token_tmp = del;
					del = del->next;
					token_remove(head, token_tmp, collector);
				}
				curr = next;
				continue;
			}
		}
		if (curr->type == TOKEN_DQUOTE || curr->type == TOKEN_SQUOTE)
		{
			start = curr;
			curr = curr->next;
			joined = ft_strdup("");
			if (!joined)
				exit_program(collector, "Error malloc tokens_expand init join", true);
			while (curr && curr->type != start->type)
			{
				if (start->type == TOKEN_DQUOTE)
				{
					expanded = expand_string(curr->value, shell, collector);
					tmp = ft_strjoin(joined, expanded);
					freer(expanded);
				}
				else
					tmp = ft_strjoin(joined, curr->value);
				if (!tmp)
					exit_program(collector, "Error join tokens_expand (quote)", true);
				freer(joined);
				joined = tmp;
				curr = curr->next;
			}
			if (!curr || curr->type != start->type)
				exit_program(collector, "minishell: unclosed quote", false);
			token_insert_before(head, start, TOKEN_WORD, joined, collector);
			freer(joined);
			next = curr->next;
			while (start != next)
			{
				token_tmp = start;
				start = start->next;
				token_remove(head, token_tmp, collector);
			}
			curr = next;
			continue;
		}
		else if (curr->type == TOKEN_WORD || curr->type == TOKEN_COMMAND)
		{
			expanded = expand_string(curr->value, shell, collector);
			if (!expanded)
				exit_program(collector, "Error in expand_string", true);
			if (curr->value)
				collector_remove_ptr(collector, curr->value);
			curr->value = ft_strdup(expanded);
			freer(expanded);
			if (!curr->value)
				exit_program(collector, "Error strdup in expansion", true);
			collector_append(collector, curr->value);
		}
		curr = curr->next;
	}
}
