#include "../minishell.h"

void clean_constructor(t_constructor *constructor)
{
	t_constructor *current;
	t_constructor *next;

	current = constructor;
	while (current)
	{
		next = current->next;

		int i = 0;
		if (current->paths)
		{
			while (current->paths[i])
			{
				free(current->paths[i]);
				i++;
			}
			free(current->paths);
		}
		if (current->executable)
		{
			i = 0;
			while (current->executable[i])
			{
				free(current->executable[i]);
				i++;
			}
			free(current->executable);
		}

		free(current);
		current = next;
	}
}

void clean_shell(t_shell *shell)
{
	int i;

	if (!shell)
		return;
	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
		{
			free(shell->env[i]);
			i++;
		}
		free(shell->env);
	}
	if (shell->export)
	{
		i = 0;
		while (shell->export[i])
		{
			free(shell->export[i]);
			i++;
		}
		free(shell->export);
	}
	free(shell->pwd);
	free(shell->oldpwd);
	free(shell->output);
	clean_constructor(shell->constructor);
	free(shell);
}
