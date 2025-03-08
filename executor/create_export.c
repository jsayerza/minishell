#include "../minishell.h"

char **swap_str(char **export, int i)
{
	char *temp;
	temp = export[i + 1];
	export[i + 1] = export[i];
	export[i] = temp;
	return (export);
}

void sort_export(t_shell *shell)
{
	int i;
	int swapped;
	char **export;

	export = shell->export;
	swapped = 1;

	while (swapped)
	{
		swapped = 0;
		i = 0;

		while (export[i] && export[i + 1])
		{
			if (ft_strcmp(export[i], export[i + 1]) > 0)
			{
				export = swap_str(export, i);
				swapped = 1;
			}
			i++;
		}
	}
}

char *concatenate_export(char *export, char *temp, char *declare)
{
	int i;
	int j;
	int equal_found;


	i = 0;
	j = 0;
	equal_found = 0;
	while (declare[i])
	{
		export[i] = declare[i];
		i++;
	}
	j = 0;
	while (temp[j])
	{
		if (temp[j] == '=' && !equal_found)
		{
			export[i++] = temp[j++];
			export[i++] = '"';
			equal_found = 1;
			continue;
		}
		export[i++] = temp[j++];
	}
	if (equal_found)
	{
		export[i++] = '"';
	}
	export[i] = '\0';
	return export;
}

void finish_export(t_shell *shell)
{
	char *declare;
	char *temp;
	int len;
	int i;


	declare = "declare -x ";
	i = 0;
	while (shell->export[i])
	{
		temp = ft_strdup(shell->export[i]);
		if (!temp)
			return;
		free(shell->export[i]);
		len = ft_strlen(temp) + ft_strlen(declare) + 3;
		shell->export[i] = malloc(len * sizeof(char));
		if (!shell->export[i])
		{
			free(temp);
			return;
		}
		concatenate_export(shell->export[i], temp, declare);
		free(temp);
		i++;
	}
}

void create_export(t_shell *shell)
{
	sort_export(shell);
	finish_export(shell);
}
