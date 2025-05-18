/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsayerza <jsayerza@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:30:00 by jsayerza          #+#    #+#             */
/*   Updated: 2024/11/19 18:47:36 by jsayerza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Busca una clave en un array tipo env (key=val)
char	*get_env_key(char *env_str)
{
	int		i;
	char	*key;

	if (!env_str)
		return (NULL);
	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	key = malloc(i + 1);
	if (!key)
		return (NULL);
	ft_strlcpy(key, env_str, i + 1);
	return (key);
}

// Compara claves de dos cadenas key=value
int	env_key_cmp(char *a, char *b)
{
	char	*ka;
	char	*kb;
	int		cmp;

	ka = get_env_key(a);
	kb = get_env_key(b);
	cmp = ft_strcmp(ka, kb);
	freer(ka);
	freer(kb);
	return (cmp);
}

// Fusiona dos arrays env: node_env sobrescribe shell_env
char	**merge_envp(char **shell_env, char **node_env, t_collector **collector)
{
	char	**merged;
	int		i;
	int		count;
	int		size;

	i = 0;
	count = 0;
	size = 0;
	// Contar total de posibles vars (shell + node)
	while (shell_env && shell_env[size])
		size++;
	while (node_env && node_env[size - count])
		size++;

	merged = malloc(sizeof(char *) * (size + 1));
	if (!merged)
		exit_program(collector, "Error malloc merge_envp", EXIT_FAILURE);
	collector_append(collector, merged);

	// Copiar shell env, evitando sobrescritos
	i = 0;
	for (int j = 0; shell_env && shell_env[j]; j++)
	{
		int overwrite = 0;
		for (int k = 0; node_env && node_env[k]; k++)
		{
			if (env_key_cmp(shell_env[j], node_env[k]) == 0)
			{
				overwrite = 1;
				break;
			}
		}
		if (!overwrite)
			merged[i++] = ft_strdup(shell_env[j]);
	}

	// Añadir node env
	for (int k = 0; node_env && node_env[k]; k++)
		merged[i++] = ft_strdup(node_env[k]);

	merged[i] = NULL;
	for (int m = 0; m < i; m++)
		collector_append(collector, merged[m]);

	return (merged);
}


///////////////////////////////////////////////////////////////////////

// ✅ 2. Parte del executor
// Supongamos que tienes una función execute_node():

void	execute_node(t_ast *node, t_shell *shell)
{
	pid_t	pid;
	char	**envp;

	if (!node || node->type != TOKEN_COMMAND || !node->args || !node->args[0])
		return ;

	envp = merge_envp(shell->env, node->envp, &shell->collector);
	pid = fork();
	if (pid == 0)
	{
		// child
		execve(node->args[0], node->args, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, NULL, 0);
	}
	else
		perror("fork");
}
