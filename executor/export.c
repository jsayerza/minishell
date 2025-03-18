#include "../minishell.h"

void	add_to_env(char ***env, char *var)
{
	int		len;
	int		index;
	char	**new_env;

	if (var_value(var) == -1)
		return ;

	len = 0;
	while ((*env)[len])
		len++;

	index = find_in_env(*env, var);
	if (update_existing_var(env, var, index))
		return ;

	new_env = create_new_env(len);
	if (!new_env)
		return ;

	copy_elements(new_env, *env, len);
	add_new_element(new_env, var, len);

	free(*env);
	*env = new_env;
}

void	update_env_var(t_constructor *node, char *arg, int index_env)
{
	if (index_env != -1)
	{
		free(node->shell->env[index_env]);
		node->shell->env[index_env] = strdup(arg);
	}
	else
	{
		add_to_env(&(node->shell->env), arg);
	}
}

void	update_export_var(t_constructor *node, char *new_var, int index_export)
{
	if (index_export != -1)
	{
		free(node->shell->export[index_export]);
		node->shell->export[index_export] = malloc(ft_strlen("declare -x ") +
								ft_strlen(new_var) + 1);
		if (node->shell->export[index_export])
		{
			ft_strlcpy(node->shell->export[index_export], "declare -x ",
					ft_strlen("declare -x ") + 1);
			ft_strlcat(node->shell->export[index_export], new_var,
					ft_strlen("declare -x ") + ft_strlen(new_var) + 1);
		}
	}
	else
	{
		add_to_export(&(node->shell->export), new_var);
	}
}

void	process_var_with_value(t_constructor *node, char *arg,
						int index_env, int index_export)
{
	char	*new_var;

	new_var = NULL;
	update_env_var(node, arg, index_env);
	new_var = process_value(new_var, arg);
	update_export_var(node, new_var, index_export);
	free(new_var);
}

void	process_export_var(t_constructor *node, char *arg)
{
	int		index_env;
	int		index_export;
	char	*var_name;

	var_name = extract_var_name(arg);
	index_env = find_in_env(node->shell->env, var_name);
	index_export = find_in_export(node->shell->export, var_name);

	if (var_value(arg) == 1)
	{
		process_var_with_value(node, arg, index_env, index_export);
	}
	else if (index_export == -1)
	{
		add_to_export(&(node->shell->export), arg);
	}

	free(var_name);
	sort_export(node->shell);
}

void	export(t_constructor *node)
{
	int	j;

	if (node->shell->node_size == 1 && !node->executable[1])
		print_export(node->shell);
	else
	{
		j = 1;
		while (j < node->size_exec)
		{
			if (!(ft_isalpha(node->executable[j][0]) ||
				node->executable[j][0] == '_'))
			{
				printf("minishell: export: `%s': not a valid identifier\n",
					node->executable[j]);
				node->error = ERROR_SYNTAX_ERROR;
			}
			else
			{
				process_export_var(node, node->executable[j]);
			}
			j++;
		}
	}
}
