#include "../minishell.h"


int	len(char **arr)
{
	int	i;
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

char	*get_var_name(char *env_var)
{
	int		i;
	char	*name;

	i = 0;
	while (env_var[i] && env_var[i] != '=')
		i++;
	name = malloc(sizeof(char) * (i + 1));
	if (!name)
		return (NULL);

	i = 0;
	while (env_var[i] && env_var[i] != '=')
	{
		name[i] = env_var[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

int	should_delete(char *env_var, char **cmds)
{
	int		j;
	char	*var_name;
	int		result;

	var_name = get_var_name(env_var);
	if (!var_name)
		return (0);

	result = 0;
	j = 1;
	while (cmds[j])
	{
		if (ft_strcmp(var_name, cmds[j]) == 0)
		{
			result = 1;
			break;
		}
		j++;
	}
	free(var_name);
	return (result);
}


void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

//void	process_export(t_constructor *node);
void	unset(t_constructor *node)
{
	char	**new_env;
	int		env_len;
	int		i;
	int		j;

	env_len = len(node->shell->env);
	new_env = malloc(sizeof(char *) * (env_len + 1));
	if (!new_env)
		return;
	i = 0;
	j = 0;
	while (node->shell->env[i])
	{
		if (!should_delete(node->shell->env[i], node->executable))
		{
			new_env[j] = ft_strdup(node->shell->env[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	free_env(node->shell->env);
	node->shell->env = new_env;
//	process_export(node);
}
