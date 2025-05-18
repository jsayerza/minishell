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

static char	*get_env_value(const char *name, t_shell *shell)
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

	printf("     IN expand_variable\n");
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
	var_value = get_env_value(var_name, shell);
	freer(var_name);
	if (var_value)
		result = ft_strdup(var_value);
	else
		result = ft_strdup("");
	if (!result)
		exit_program(collector, "Error malloc expand_variable result", true);
	printf("     OUT expand_variable\n");
	return (result);
}

static char	*expand_string(const char *str, t_shell *shell, t_collector **collector)
{
	int		i;
	char	*result;
	char	*tmp;
	char	*expanded;
	int		start;

	printf("   IN expand_string\n");
	i = 0;
	result = ft_strdup("");
	if (!result)
		exit_program(collector, "Error malloc expand_string init", true);
	//collector_append(collector, result);
	while (str[i])
	{
		printf("    IN expand_string-str[%d]:%c\n", i, str[i]);
		if (str[i] == '$')
		{
			printf("    IN expand_string-$\n");
			expanded = expand_variable(str, &i, shell, collector);
			tmp = ft_strjoin(result, expanded);
			freer(expanded);
			if (!tmp)
				exit_program(collector, "Error malloc expand_string join var", true);
			result = ft_strdup(tmp);
			freer(tmp);
			printf("    OUT expand_string-$\n");
		}
		else
		{
			printf("    IN expand_string-A\n");
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
			result = ft_strdup(tmp);
			printf("     -->result: %s\n", result);
			freer(tmp);
			printf("    OUT expand_string-A\n");
		}
	}
	printf("   OUT expand_string\n");
	return (result);
}

void	tokens_expand(t_token **head, t_shell *shell, t_collector **collector)
{
	t_token	*curr;
	t_token	*start;
	t_token	*next;
	t_token	*token_tmp;
	t_token *prev;
	char	*joined;
	char	*tmp;
	char	*expanded;

	printf("IN tokens_expand\n");
	curr = *head;
	while (curr)
	{
		// ✨ Manejo especial: asignación con comillas (ej: a="ABC")
		if ((curr->type == TOKEN_DQUOTE || curr->type == TOKEN_SQUOTE))
		{
			prev = get_prev_token(*head, curr);
			if (prev && prev->type == TOKEN_WORD && ft_strchr(prev->value, '='))
			{
<<<<<<< HEAD
				printf("    IN tokens_expand fusion asignación + comillas\n");

				start = curr;
=======
				printf("     curr->value: %s\n", curr->value);
				expanded = expand_string(curr->value, shell, collector);
				printf("     expanded: %s joined: %s\n", expanded, joined);
				tmp = ft_strjoin(joined, expanded);
				printf("     tmp: %s\n", tmp);
				freer(expanded);
				if (!tmp)
					exit_program(collector, "Error join tokens_expand", true);
				freer(joined);
				joined = tmp;
>>>>>>> d9063bd (actualitzacio)
				curr = curr->next;

				joined = ft_strdup("");
				if (!joined)
					exit_program(collector, "Error malloc tokens_expand assign+quote", true);
				// collector_append(collector, joined);

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

				// curr ahora está en comilla de cierre
				next = curr->next; // ⚠️ Importante: guardar el siguiente token para continuar

				tmp = ft_strjoin(prev->value, joined); // prev = token 'a='
				freer(prev->value);
				prev->value = tmp;
				// collector_append(collector, tmp);
				freer(joined);

				// Eliminar tokens: comilla apertura, contenido, comilla cierre
				t_token *del = start;
				while (del != next)
				{
					token_tmp = del;
					del = del->next;
					token_remove(head, token_tmp, collector);
				}

				curr = next;
				printf("    OUT tokens_expand fusion asignación + comillas\n");
				continue;
			}
		}

		// ✨ Expansión de comillas dobles/simples estándar
		if (curr->type == TOKEN_DQUOTE || curr->type == TOKEN_SQUOTE)
		{
			printf(" IN tokens_expand TOKEN_%sQUOTE\n", curr->type == TOKEN_DQUOTE ? "D" : "S");
			start = curr;
			curr = curr->next;

			joined = ft_strdup("");
			if (!joined)
				exit_program(collector, "Error malloc tokens_expand init join", true);
			// collector_append(collector, joined);

			while (curr && curr->type != start->type)
			{
				if (start->type == TOKEN_DQUOTE)
				{
					expanded = expand_string(curr->value, shell, collector);
					tmp = ft_strjoin(joined, expanded);
					freer(expanded);
				}
				else
				{
					tmp = ft_strjoin(joined, curr->value);
				}
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

			tokens_print(head);
			printf(" OUT tokens_expand TOKEN_%sQUOTE\n\n", curr && curr->type == TOKEN_DQUOTE ? "D" : "S");
			continue;
		}
		else if (curr->type == TOKEN_WORD || curr->type == TOKEN_COMMAND)
		{
			printf(" IN tokens_expand TOKEN_WORD/COMMAND\n");
			expanded = expand_string(curr->value, shell, collector);
			if (!expanded)
				exit_program(collector, "Error in expand_string", true);
			printf("   dins tokens_expand TOKEN_WORD/COMMAND 1\n");
			if (curr->value)
				collector_remove_ptr(collector, curr->value);
			printf("   dins tokens_expand TOKEN_WORD/COMMAND 2\n");
			curr->value = ft_strdup(expanded);
			if (!curr->value)
				exit_program(collector, "Error strdup in expansion", true);
			collector_append(collector, curr->value);
			// if (curr->type == TOKEN_COMMAND && is_assignment(curr->value))
			// {
			// 	curr->type = TOKEN_WORD;
			// }
			tokens_print(head);
			printf(" OUT tokens_expand TOKEN_WORD/COMMAND\n\n");
		}
		// if (joined)
		// 	freer(joined);
		// if (tmp)
		// 	freer(tmp);
		printf("   dins while IN tokens_expand\n");
		curr = curr->next;
	}
	printf("OUT tokens_expand\n");
}
