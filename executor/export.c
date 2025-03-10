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
	new_export[len] = malloc(ft_strlen("declare -x ") + ft_strlen(var) + 1);
	if (!new_export[len])
	{
		perror("malloc");
		free(new_export);
		return;
	}
	ft_strlcpy(new_export[len], "declare -x ", ft_strlen("declare -x ") + 1);
	ft_strlcat(new_export[len], var, ft_strlen("declare -x ") + ft_strlen(var) + 1);
	new_export[len + 1] = NULL;
	free(*export);
	*export = new_export;
}

int var_value(char *var)
{
	int i = 0;

	while (var[i] && var[i] != '=')
		i++;

	if (var[i] == '=')
		return 1;
	else
		return -1;
}

char *process_value(char *new_var, char *var)
{
	int len;
	int i = 0;
	int j = 0;

	len = ft_strlen(var);
	new_var = (char *)malloc(sizeof(char) * (len + 3));
	if (!new_var)
		return NULL;
	while (var[i] && var[i] != '=')
		new_var[j++] = var[i++];
	if (var[i] == '=')
	{
		new_var[j++] = var[i++];
		new_var[j++] = '\"';
		while (var[i])
			new_var[j++] = var[i++];
		new_var[j++] = '\"';
	}
	new_var[j] = '\0';
	return new_var;
}
int find_in_env(char **env, char *var)
{
	int i = 0;
	size_t var_len = ft_strlen(var);

	while (env[i])
	{
		if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
			return i;
		i++;
	}
	return -1;
}
void add_to_env(char ***env, char *var)
{
	int len = 0;
	int i = 0;
	char **new_env;

	while ((*env)[len])
		len++;
	int index = find_in_env(*env, var);
	if (index != -1)
	{
		free((*env)[index]);
		(*env)[index] = strdup(var);
		return;
	}
	new_env = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_env)
	{
		perror("malloc");
		return;
	}
	while (i < len)
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[i++] = strdup(var);
	new_env[i] = NULL;
	free(*env);
	*env = new_env;
}



void process_export_var(t_constructor *node, char *arg)
{
	int index;
	char *new_var = NULL;

	// Buscar en env en vez de export (mejor sincronización)
	index = find_in_env(node->shell->env, arg);

	if (index == -1)
	{
		if (var_value(arg))
		{
			new_var = process_value(new_var, arg);
			add_to_env(&(node->shell->env), new_var);
			add_to_export(&(node->shell->export), new_var);
			free(new_var);
		}
		else
			add_to_export(&(node->shell->export), arg);
		sort_export(node->shell);
	}
	else
	{
		// Si la variable ya existe, reemplazar en export
		free(node->shell->export[index]);
		node->shell->export[index] = strdup(arg);
	}
}


void export(t_constructor *node)
{
	printf("Dins de export\n");
	int i;
	int j;

	if (node->shell->node_size == 1 && !node->executable[1])
		print_export(node->shell);
	else
	{
		printf("mes de un exec\n");
		i = 0;
		printf("size->node_list : %d\n", node->shell->node_size);
		while (i < node->shell->node_size )
		{
			j = 1;
			while (j < node->size_exec)
			{
				if (!(ft_isalpha(node->executable[i][0]) || node->executable[j][0] == '_'))
				{
					printf("minishell: export: `%s': not a valid identifier\n",
						   node->executable[i]);
					node->error = ERROR_SYNTAX_ERROR;
				}
				else
				{
					process_export_var(node, node->executable[j]);
				}
				j++;
			}
			i++;
		}
	}
}
