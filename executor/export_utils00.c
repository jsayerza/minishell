#include "../minishell.h"

int	find_in_env(char **env, char *var_name)
{
	int		i;
	size_t	var_len;

	i = 0;
	var_len = ft_strlen(var_name);

	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, var_len) == 0 &&
			(env[i][var_len] == '=' || env[i][var_len] == '\0'))
			return (i);
		i++;
	}

	return (-1);
}

int	find_in_export(char **export, char *var_name)
{
	int		i;
	char	*export_var;

	i = 0;
	while (export[i])
	{
		export_var = export[i] + ft_strlen("declare -x ");

		if (ft_strncmp(export_var, var_name, ft_strlen(var_name)) == 0 &&
			(export_var[ft_strlen(var_name)] == '=' ||
			export_var[ft_strlen(var_name)] == '\0'))
		{
			return (i);
		}
		i++;
	}

	return (-1);
}

int	update_existing_var(char ***env, char *var, int index)
{
	if (index != -1)
	{
		free((*env)[index]);
		(*env)[index] = strdup(var);
		return (1);
	}
	return (0);
}

char	**create_new_env(int len)
{
	char	**new_env;

	new_env = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_env)
	{
		perror("malloc");
		return (NULL);
	}
	return (new_env);
}

void	add_new_element(char **new_env, char *var, int len)
{
	new_env[len] = strdup(var);
	new_env[len + 1] = NULL;
}
