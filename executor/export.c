#include "../minishell.h"

int get_export_length(char **export)
{
	int len = 0;

	while (export[len])
		len++;

	return len;
}

void add_to_export(char ***export, char *var)
{
	int len = 0;
	char **new_export;
	int i = 0;

	len = get_export_length(*export);
	new_export = malloc((len + 2) * sizeof(char *));
	if (!new_export)
	{
		perror("malloc");
		return;
	}
	while (i < len)
	{
		new_export[i] = (*export)[i];
		i++;
	}
	new_export[len] = ft_strdup(var);
	if (!new_export[len])
	{
		perror("strdup");
		free(new_export);
		return;
	}
	new_export[len + 1] = NULL;
	free(*export);
	*export = new_export;
}

int find_in_env(char **env, char *var)
{
	int i;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, ft_strlen(var)) == 0 && env[i][ft_strlen(var)] == '=')
			return (i);
		i++;
	}
	if (env[i] == NULL)
		i = -1;
	return (i);
}

void process_export_var(t_constructor *node, char *arg)
{
	int index;
	index = find_in_env(node->shell->env, arg);
	if (index == -1)
	{
		add_to_export(&(node->shell->export), arg);
	}
	else
		add_to_export(&(node->shell->export), node->shell->env[index]);
}

void export(t_constructor *node)
{
	printf("Dins de export\n");
	int i;
	if (node->size == 0 && !node->executable[1])
		print_export(node->shell);
	else
	{
		printf("mes de un exec\n");
		i = 0;
		while (i <= node->size && node->executable[i + 1])
		{
			if (!(ft_isalpha(node->executable[i][0]) || node->executable[i + 1][0] == '_'))
			{
				printf("minishell: export: `%s': not a valid identifier\n",
					   node->executable[i]);
				node->error = ERROR_SYNTAX_ERROR;
			}
			else
			{
				process_export_var(node, node->executable[i + 1]);
			}
			i++;
		}
	}
}
