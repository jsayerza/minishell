#include "../minishell.h"


void	free_env(char **env, int count)
{
	int i;

	for (i = 0; i < count; i++)
	{
		free(env[i]);
	}
	free(env);
}

char	**allocate_env(int env_count)
{
	char **env;

	env = malloc(sizeof(char *) * (env_count + 1));
	if (!env)
		return (NULL);
	return (env);
}

int	copy_env_variable(char **env, char *envv, int index)
{
	env[index] = malloc(sizeof(char) * (ft_strlen(envv) + 1));
	if (!env[index])
		return (0);
	if (ft_strlcpy(env[index], envv, ft_strlen(envv) + 1) != ft_strlen(envv))
	{
		free(env[index]);
		return (0);
	}
	return (1);
}

void copy_env_to_shell(t_shell *shell, char **envv) {
    int i;
    int env_count;
    char **env;

    env_count = 0;
    while (envv && envv[env_count])
        env_count++;
    if (env_count == 0) {
        fprintf(stderr, "Error: El entorno está vacío.\n");
        return;
    }
    env = allocate_env(env_count);
    if (!env)
        return;
    i = 0;
    while (i < env_count) {
        if (!copy_env_variable(env, envv[i], i)) {
            free_env(env, i);
            return;
        }
        i++;
    }
    env[i] = NULL;
    shell->env = env;
}
